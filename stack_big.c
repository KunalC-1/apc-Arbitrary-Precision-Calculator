/*
        File contains functions for basic operation on stack of BIG number
*/ 

#include<stdio.h>
#include<stdlib.h>
#include "bignum.h"
#include "stack_big.h"

//Initializes stack to NULL.

void init_stack_big(big_stack *s){
	*s = NULL;
    return;
}

// Checks whether stack is empty or not.
// Returns 1 if empty. Otherwise returns 0.

int isEmpty_stack_big(big_stack s){
    return s == NULL;
}

// Pushes bignum into stack.

void push_stack_big(big_stack *s, BIG num){
    node2* p;
    p = (node2*)malloc(sizeof(node2));
    if(!p){
        return;                                      // Returns if malloc fails.
    }
    p->num =num;
    p->next = *s;
    *s = p;
}

// Returns top bignum in the stack and pop it from the stack.

BIG pop_stack_big(big_stack *s){
    if(isEmpty_stack_big(*s)){
        return NULL ;                               // Returns Null if stack is empty.
    }
    BIG ele = (*s)->num;
    node2 * p = *s;
    *s = (*s)->next;
    p->next = NULL;
    free(p);
    return ele;

}

// Returns top bignum in the stack.

BIG peek_stack_big(big_stack s){
    if(!isEmpty_stack_big(s)){
        return s->num;
    }else
        return NULL;                                // Returns Null character if stack is empty. 
}

//  Displays all bignum in stack in top to bottom order.
//  Returns if stack is empty.

void display_stack_big(big_stack s){
    node2 * p;
    if(s == NULL){
        return;
    }
    else{
        p = s;
        while(p){
            display_big(p->num);
            p = p->next;
        }
    }
}

// Frees all malloced memory in stack and thus deletes the stack.

void destroy_stack_big(big_stack *s){
    big_stack current = *s; 
    big_stack next; 
  
   while (current != NULL){ 
       next = current->next; 
       free(current); 
       current = next; 
   }*s = NULL; 
}