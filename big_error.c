/*
    This file contains special types of big numbers to return for function when error encountered
*/
#include<stdio.h>
#include "bignum.h"
#include "big_error.h"

// Difffrent types of errors

BIG OPERATION_ERROR(){
    BIG error_1;
    init_big(&error_1);
    error_1->digits[1] = -1;
    return error_1;
}

BIG EXPRESSION_ERROR(){
    BIG error_2;
    init_big(&error_2);
    error_2->digits[1] = -2;
    return error_2;
}

BIG SUCCESS(){
    BIG success;
    init_big(&success);
    success->digits[1] = -3;
    return success; 
}

BIG EXIT_LOOP(){
    BIG exit;
    init_big(&exit);
    exit->digits[1] = -4;
    return exit;
}

// Checking functions for different types of errors

int IS_OPERATION_ERROR(BIG answer){
    if(!answer)
        return 0;
    if(answer->digits[1] == -1){
        return 1;
    }else{
        return 0;
    }
}
int IS_EXPRESSION_ERROR(BIG answer){
    if(!answer)
        return 0;
    if(answer->digits[1] == -2){
        return 1;
    }else{
        return 0;
    }
}
int IS_SUCCESS(BIG answer){
    if(!answer)
        return 0;
    if(answer->digits[1] == -3){
        return 1;
    }else{
        return 0;
    }
}
int IS_EXIT(BIG answer){
    if(!answer)
        return 0;
    if(answer->digits[1] == -4){
        return 1;
    }else{
        return 0;
    }
}