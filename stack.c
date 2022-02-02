/*
    Functions for all operations on stack are included in this file.
*/

#include<stdio.h>
#include<stdlib.h>
#include "stack.h"

//Initializes stack to NULL.

void init(stack *s){
	*s = NULL;
    return;
}

// Checks whether stack is empty or not.
// Returns 1 if empty. Otherwise returns 0.

int isEmpty(stack s){
    if(s == NULL)
        return 1;
    else
        return 0;
}
// Pushes data into stack.

void push(stack *s,char data){
    stack_node* p;
    p = (stack_node*) malloc(sizeof(stack_node));
    if(!p){                                                 
        return;                                     // Returns if malloc fails.
    }
    p->data = data;
    p->next = *s;
    *s = p;
}

// Returns top element in stack and pop it from the stack.

char pop(stack *s){
    if(isEmpty(*s)){
        return '\0';                            // Returns Null character if stack is empty.
    }
    char element = (*s)->data;
    stack_node * p = *s;
    *s = (*s)->next;
    p->next = NULL;
    free(p);
    return element;

}

// Returns top element in the stack.

char peek(stack s){
    if(!isEmpty(s)){
        return s->data;
    }else
        return '\0';                             // Returns Null character if stack is empty. 
}

//  Displays all stack data in top to bottom order.
//  Returns if stack is empty.

void display(stack s){
    stack_node * p;
    if(s == NULL){
        return;
    }
    else{
        p = s;
        while(p){
            printf("%c\n", p->data);
            p = p->next;
        }
    }
}

// Frees all malloced memory in stack and thus deletes the stack.

void destroy(stack *s){
    stack current = *s; 
    stack next; 
  
   while (current != NULL){ 
       next = current->next; 
       free(current); 
       current = next; 
   }*s = NULL; 
}
