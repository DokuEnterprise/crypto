#pragma once

#include <stdio.h>
#include <string.h>
#include "sha1.hpp"
#include <stdlib.h>


void sha_fun(char target_string[], char* sha_result);
int htoi(char a);
void xor_operation(const char a, const char b, char* xor_result);


