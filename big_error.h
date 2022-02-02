/*
    File contains prototypes for different error functions
*/

#ifndef BIG_ERROR_H_INCLUDED
#define BIG_ERROR_H_INCLUDED
#include "bignum.h"

BIG OPERATION_ERROR();

BIG EXPRESSION_ERROR();

BIG SUCCESS();

BIG EXIT_LOOP();

int IS_OPERATION_ERROR(BIG answer);

int IS_EXPRESSION_ERROR(BIG answer);

int IS_SUCCESS(BIG answer);

int IS_EXIT(BIG answer);

#endif // BIG_ERROR_H_INCLUDED