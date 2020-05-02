#include <mcl/bls12_381.hpp>
#include "utils.hpp"
#include <tuple>
#include <vector>
#include <sodium.h>
#include <chrono>
#include <sodium/crypto_secretbox.h>
#include <iostream>

using namespace mcl::bn;

struct master_pub_k {
    G1 g1;
};

struct master_pri_k {
    Fr s;
};

struct id_pri_key {
    G2 d;
    G2 q;
};

struct ciphertext{
    G1 U;
    unsigned char V[crypto_secretbox_MACBYTES+10000];
    unsigned char nonce[crypto_box_NONCEBYTES];
};

typedef struct ciphertext ciphertext;
typedef struct master_pub_k m_pub_k;
typedef struct master_pri_k m_pri_k;
typedef struct id_pri_key id_pri_key;
G1 generator;

auto setup(){

    G1 pub;
    Fr s; s.setByCSPRNG();
    
    G1::mul(pub,generator, s);

    m_pub_k key{pub};
    m_pri_k sk{s};
    
	return std::make_tuple(key, sk);
}

auto extract(m_pri_k* pri, char* id){
    G2 q, d;
    hashAndMapToG2(q, id, strlen(id));
    G2::mul(d, q, pri->s);
    id_pri_key k {d,q};
    return k;
}

auto encrypt(m_pub_k* key, char* id, char* msg){
    G2 q; GT g, er;
    hashAndMapToG2(q, id, strlen(id));
    pairing(g, key->g1, q);
    
    G1 rp;
    Fr r; r.setByCSPRNG();

    // Curve point generator

    G1::mul(rp, generator, r);
    GT::pow(er, g, r);


    int CIPHERTEXT_LEN = crypto_secretbox_MACBYTES + strlen(msg);
    unsigned char nonce[crypto_secretbox_NONCEBYTES];
    unsigned char ciphertexttmp[CIPHERTEXT_LEN];
    auto sk = serialize(q, rp, er);

    //printf("THIS IS COOL\n");
    char buffer[65];
    //std::cout << sk << std::endl;
    sha256(&sk[0], buffer);
    //printf("END\n");


    randombytes_buf(nonce, sizeof(nonce));
    crypto_secretbox_easy(ciphertexttmp, reinterpret_cast<unsigned char*>(msg), strlen(msg),
     nonce, reinterpret_cast<unsigned char*>(buffer));

    ciphertext ctx;
    ctx.U = rp;
    memcpy(ctx.V, ciphertexttmp, sizeof(ciphertexttmp));
    memcpy(ctx.nonce, nonce, sizeof(nonce));
    return ctx;    
}

auto decrypt(id_pri_key k, ciphertext c){
    GT e;
    pairing(e, c.U, k.d);

    auto key = serialize(k.q, c.U, e);
    unsigned char decrypted[100000];

    //printf("THIS IS COOL\n");
    char buffer[65];
    //std::cout << key << std::endl;
    sha256(&key[0], buffer);
    //printf("END\n");


    if (crypto_secretbox_open_easy(decrypted, c.V, strlen((char*) c.V),
     c.nonce, reinterpret_cast<unsigned char*>(buffer)) != 0) {
        printf("FAILED TO DECRYPT MESSAGE\n");
    }else{
        std::cout << (char*)decrypted << std::endl;
    }
    
}

auto test_correctness(){
     // Test
    auto PKG = setup();
    m_pub_k m_pub = std::get<0>(PKG);
    m_pri_k m_pri = std::get<1>(PKG);

    // Get Friedrich's private key
    auto fred = extract(&m_pri, "friedrichdoku@gmail.com");
    // Get Ethan's private key
	auto ethan = extract(&m_pri, "ethandoku@gmail.com");

    // Encrypt a message for Friedrich
    auto ciphertext = encrypt(&m_pub, "friedrichdoku@gmail.com", "I like to live in china");
    //auto ciphertext2 = encrypt(&m_pub, "friedrichdoku@gmail.com", "I like to live in china");

    // Open Friedrich's message
    decrypt(fred, ciphertext);


    // Let Ethan try to open Friedrich's message
     decrypt(ethan, ciphertext);
   // printf("decrypted: %s\n", dec2);
}

auto test_speed(){
     // Test
    auto PKG = setup();
    m_pub_k m_pub = std::get<0>(PKG);
    m_pri_k m_pri = std::get<1>(PKG);

    // Get Friedrich's private key
    auto fred = extract(&m_pri, "friedrichdoku@gmail.com");
    // Get Ethan's private key
	//auto ethan = extract(&m_pri, "ethandoku@gmail.com");

    // Encrypt a message for Friedrich
    std::vector<ciphertext> ctxs;
    for(int i = 0; i < 1000; i++){
        auto ciphertext = encrypt(&m_pub, "friedrichdoku@gmail.com", "I like to live in china");
        ctxs.push_back(ciphertext);
    }
    
    auto start = std::chrono::high_resolution_clock::now(); 
    for(int i = 0; i < 1000; i++){
        decrypt(fred, ctxs[i]);
    }
    auto stop = std::chrono::high_resolution_clock::now();

    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start); 
  
    std::cout << "Time taken by function: "
         << duration.count() << " milliseconds" << std::endl; 
}

auto main() -> int
{
	initPairing(mcl::BLS12_381);

    char* gen = "abc";
    hashAndMapToG1(generator, &gen, strlen(gen));
    test_speed();

}
