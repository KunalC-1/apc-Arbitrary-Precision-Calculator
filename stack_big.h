/* 
	ADT for stack and prototypes of basic stack operations are included in this header file.
	Data in this stack is a new data type BIG (Which can contains any number containing unlimited no of digits).
*/
#ifndef STACK_BIG_H_INCLUDED
#define STACK_BIG_H_INCLUDED
#include "bignum.h"

// Data Type for stack containig bignum.

typedef struct node2{
    	BIG num;                             	// Contains big number.
    	struct node2 *next;                   	// Pointer to next node.
}node2;

typedef node2* big_stack;                    	// Typedef for pointer of node2.

void init_stack_big(big_stack *s);              // Initializes the stack.

int isEmpty_stack_big(big_stack s);            	// Return true if stack is empty.

void push_stack_big(big_stack *s,BIG data);     // Pushes bignum into the stack.

BIG pop_stack_big(big_stack *s);		        // Returns popped bignum from stack.

BIG peek_stack_big(big_stack s);			    // Returns top bignum in the stack.

void display_stack_big(big_stack s);			// Displays complete stack.

void destroy_stack_big(big_stack *s);			// Free every malloced memory thus deleting the stack.

#endif // STACK_BIG_H_INCLUDED