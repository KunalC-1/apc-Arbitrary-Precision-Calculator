/*
	ADT for stack and prototypes of basic stack operations are included in this header file.

*/
#ifndef STACK_H_INCLUDED
#define STACK_H_INCLUDED

// Data Type for stack

typedef struct stack_node{
    	char data;                             	// Contains character data.
    	struct stack_node *next;               	// Pointer to the next node.
}stack_node;

typedef stack_node* stack;                     	// Typedef for pointer of stack_node.

void init(stack *s);                           	// Initializes the stack.

int isEmpty(stack s);                          	// Return true if stack is empty.

void push(stack *s,char data); 					// Pushes data into the stack.

char pop(stack *s);								// Returns popped element from stack.

char peek(stack s);								// Returns top element in the stack.

void display(stack s);							// Displays complete stack.

void destroy(stack *s);							// Free every malloced memory thus deleting the stack.

#endif // STACK_H_INCLUDED

