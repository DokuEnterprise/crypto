
#pragma once

#include <pbc/pbc.h>
#include <gmp.h>
#include <stdio.h>
#include <string.h>
//#include "sha1.h"
#include <stdlib.h>
#include "utils.hpp"

void get_private_key_f(char* ID, pairing_t pairing, element_t s, element_t Sid);
void get_public_key_f(char* ID, pairing_t pairing, element_t Qid);
void rand_n(char* sigma);
void encryption_f(char* shamessage,char* ID, element_t P,element_t Ppub,element_t U,char* V,char* W, pairing_t pairing);
void decryption_f(element_t Sid,pairing_t pairing,element_t P,element_t U,char* V,char* W,element_t U_receiver,char* shamessage_receiver);
void setup_sys_f(int rbits,int qbits,element_t P,element_t Ppub,pairing_t pairing,element_t s );
