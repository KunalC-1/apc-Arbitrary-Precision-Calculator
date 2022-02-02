/* 
	Header file contains prototypes for functions of arithmetic operations on big numbers
    Arithmetic operations include  + , - , * , / , %, ^, sin, cos, tan, r (square root). 
	
*/
#ifndef BIG_OPERATIONS_H_INCLUDED
#define BIG_OPERATIONS_H_INCLUDED
#include "bignum.h"

/*
    Multiplication , Division , Power , Modulo, Square Root functions are 
                      dependent on scale provided
*/

BIG add_big(BIG a, BIG b);                    // Adds two big numbers and returns answer of addtion.

BIG sub_big(BIG a, BIG b);                    // Subtract two big numbers and returns answer of subtraction.

BIG mult_big(BIG a, BIG b, int scale);        // Multiply two big numbers and returns answer of multiplication. 

BIG divide_big(BIG a, BIG b, int scale);      // Divide two big numbers and returns answer of division.

BIG mod_big(BIG a,BIG b, int scale);          // Returns modulo of two big numbers.

BIG pow_big(BIG a , int b, int scale);        // Calculate fist bignum raised to second bignum and returns it.

BIG sine(long double x);                      // Returns sine of given angle in radians.

BIG cosine(long double x);                    // Returns cosine of given angle in radians.

BIG tangent(long double x);                   // Returns tangent of given angle in radians.

BIG sqrt_big(BIG num, int scale);             // Returns square root of bignum.

#endif // BIG_OPERATIONS_H_INCLUDED