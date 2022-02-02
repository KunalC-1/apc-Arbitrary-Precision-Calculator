#ifndef EXPRESSION_EVALUATE_H_INCLUDED
#define EXPRESSION_EVALUATE_H_INCLUDED

#include<stdio.h>
#include "bignum.h"
#include "stack.h"
#include "stack_big.h"

#define OPERAND        100
#define OPERATOR       200
#define END            300
#define ERR            400
#define UNARY_OPERATOR 500
#define KEYWORD        600
#define QUIT           700



/*
 A token datatype that will store type of token and token itself
        (OPERAND or OPERATOR or END or ERROR or UNARY_OPERATOR or KEYWORD or QUIT)
*/

typedef struct token{
    int type;                       //OPERATOR , OPERAND, END  , ERR, UNARY_OPERATOR, KEYWORD,  QUIT

    //Used union as we have to store one of three tokens.
    
    union data{
        BIG num;                  // Stores BIG number
        char op;                  // Stores operator
        int sc;                   // For storing value of keyword like scale value
    }data;
}token;

// Different states in tokenizer

enum states{START, NUMBER, OP, UNARY, FINISH, ERROR, TRIGN, CHARACTER, ROOT};


char *inputString(FILE *fp, int size);                            // Taking input from stdin as string 

BIG getbig_num(char *str,int start,int end, int sign);            // Getting big number from string

int check_parenthesis(char *expr);                                 // Checks for parenthesis errors

token get_token(char *expr);                                      // Getting token from expression

int precedence(char c);                                           // Returns precedence of given operator

BIG perform_operation(big_stack *st_num, stack * st_op);          // Performs operations on top big numbers of stack

BIG infix_eval(char *expr);                                       // Evaluate infix expression

#endif // EXPRESSION_EVALUATE_H_INCLUDED