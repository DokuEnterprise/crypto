#pragma once

#include <openssl/sha.h>
#include <stdio.h>
#include <asm/errno.h>
#include <string.h>
#include <stdlib.h>
#include <dclxvi/fp2e.h>

using byte = unsigned char;

// SHA-256

std::string sha256(std::string string)
{
    const char * str = string.c_str();
    char outputBuffer[65];
    unsigned char hash[SHA256_DIGEST_LENGTH];
    SHA256_CTX sha256;
    SHA256_Init(&sha256);
    SHA256_Update(&sha256, str, strlen(str));
    SHA256_Final(hash, &sha256);
    int i = 0;
    for(i = 0; i < SHA256_DIGEST_LENGTH; i++)
    {
        sprintf(outputBuffer + (i * 2), "%02x", hash[i]);
    }
    outputBuffer[64] = 0;
    std::string output(outputBuffer);
    return output;
}

void sha256_hash_string(unsigned char hash[SHA256_DIGEST_LENGTH], char outputBuffer[65])
{
     for (int i = 0; i < SHA256_DIGEST_LENGTH; i++)
     {
         sprintf(outputBuffer + (i * 2), "%02x", hash[i]);
     }

     outputBuffer[64] = 0;
}

int sha256_file(char *path, char outputBuffer[65])
{
    FILE *file = fopen(path, "rb");
    if(!file) return -534;

    byte hash[SHA256_DIGEST_LENGTH];
    SHA256_CTX sha256;
    SHA256_Init(&sha256);
    const int bufSize = 32768;
    char *buffer = new char[bufSize];
    int bytesRead = 0;
    if(!buffer) return ENOMEM;
    while((bytesRead = fread(buffer, 1, bufSize, file)))
    {
        SHA256_Update(&sha256, buffer, bytesRead);
    }
    SHA256_Final(hash, &sha256);

    sha256_hash_string(hash, outputBuffer);
    fclose(file);
    free(buffer);
    return 0;
}

// Set X and Y fp2e

void fp2e_set_x(fp2e_t *a, const double *words) {
	memcpy(&a[0]->v[0], words, sizeof(double) * 24);
}
void fp2e_set_y(fp2e_t *a, const double *words) {
	memcpy(&a[0]->v[0], words, sizeof(double) * 24);
}

