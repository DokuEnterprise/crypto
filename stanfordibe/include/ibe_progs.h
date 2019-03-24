/* header file for ibe.cc et al.
 * Ben Lynn
 */
/*
Copyright (C) 2001 Benjamin Lynn (blynn@cs.stanford.edu)

See LICENSE for license
*/
#ifndef IBE_PROGS_H
#define IBE_PROGS_H

#include "ibe.h"
#include "config.h"

int decrypt(int argc, char **argv);
int encrypt_sibe(int argc, char **argv);
int request(int argc, char **argv);
int combine(int argc, char **argv);
int imratio(int argc, char **argv);

int extract_share(int argc, char **argv);
int key_from_master_shares(int argc, char **argv);
int fixed_password_decrypt(int argc, char **argv);

extern CONF_CTX *cnfctx;
extern params_t params;

#endif //IBE_PROGS_H
