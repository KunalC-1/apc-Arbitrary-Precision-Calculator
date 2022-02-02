/*
		This File contains all function required for tokenizer and also contains tokenizer and expression evaluator
		using two stacks.
*/

#include<stdio.h>
#include<limits.h>
#include<string.h>
#include<ctype.h>
#include<stdlib.h>
#include<math.h>
#include "bignum.h"
#include "big_operations.h"
#include "big_error.h"
#include "stack.h"
#include "stack_big.h"
#include "expression_evaluate.h"

int SCALE = 2;
// This function takes input string from stdin
// It also cosiders errors like 1233  5566+ 223 i.e space between two numbers
// And also discard all other spaces in input string

char *inputString(FILE *fp, int size){
	int flag = 0; 
	int error = 0; 
    char *str;
    char ch;
    int len = 0;
    str = realloc(NULL, size);                        
    if(!str)return str;
    while(EOF != (ch = fgetc(fp)) && ch != '\n'){
        switch (ch){
			case ' ':
				flag = 1;
				break;
			default :
				if(flag == 1 && len != 0 && isdigit(str[len-1])){
					if(isdigit(ch)){
						error = 1;                                   //Setting error = 1 as error encountered in expression
					}
				}
				str[len++] = ch;
				flag = 0;
		}	
        if(len==size){
            str = realloc(str, sizeof(char)*(size+=16));             // Using realloc to change previous malloced memory
            if(!str)return str;
        }
    }
	str[len++]='\0';
	str = realloc(str, len);						// Using realloc to change previous malloced memory thus giving required memory
	if(error != 1){
		return str;
	}else{											// if there is error
		free(str);
		return NULL;
	}
}

// Given start and end position of number in expr it extracts number from it
// and stores in our BIG data type

BIG getbig_num(char *str, int start, int end, int sign){
    BIG num;
    init_big(&num);
    if(!num) return NULL;
    num->type = 'i';
    num->sign = sign;
    int j=0;

    for(j = start; j < end; j++){                  // Checking for '.' and its position
        if(str[j] == '.')
            break;
        else
            continue;
    }
    num->digits = realloc(num->digits, sizeof(int)*(j-start+1));
    for(int size = 1 , k = j - 1; k >= start; k--){
        num->digits[size++] = str[k] - '0';
    }num->digits[0] = j - start + 1;
    if(j != end && j != end - 1){
        num->type = 'f';
        num->fl = (int*)malloc(sizeof(int) * (end - j));
        for(int size = 1, k = j + 1; k < end; k++){
            num->fl[size++] = str[k]-'0';
        }
        num->fl[0] = end - j;
    }clear_zeros(num);
	return num;
}

// Checking for valid expression considering parenthesis
// Using stack method

int check_parenthesis(char *expr){
	if(!expr){
		return 0;
	}
	stack s;
	init(&s);
	
	for(int i = 0; i < strlen(expr) ; i++){
		if(expr[i] == '('){
			push(&s, '(');
		}else if(expr[i] == ')'){
			pop(&s);
		}
	}if(isEmpty(s)){
		destroy(&s);
		return 1;
	}destroy(&s);
	return 0;
}
/*													Tokenizer

    Token function will give us one token on calling once from string expr.
	It consist of different states like START, NUMBER, OP, UNARY, FINISH, ERROR, TRIGN, CHARACTER, ROOT
	Different types of tokens are OPERAND , OPERATOR , END , ERR , UNARY_OPERATOR , KEYWORD , QUIT
	According to it can handles all errors.

*/
token get_token(char *expr){
    BIG num;
    token t;
    char trig;														// Storing trgnometric character like s, c , t
	long double f = 0.0;	
	int j = 0, k = 0, d;
	static int start_index = 0, sign = 0, key = 0,point = 0;
    static int i = 0;
    static enum states currstate = START;						   // Initial state for new expression
    char currchar;												   // Stores current character
    enum states nextstate;                                         // Stores next state
    while(1){
        currchar = expr[i];
        switch (currstate){
            case START :
                switch(currchar){
                    case '0':case '1':case '2': case '3':
					case '4':case '5':case '6': case '7':
					case '8':case '9':
						start_index = i;
						sign = 0;
						nextstate = NUMBER;
						i++;
						break;
					case '.':
						start_index = i;
						sign = 0;
						point = 1;
						nextstate = NUMBER;
						i++;
						break;
					case '-' :
						nextstate = UNARY;
						i++;
						break;
					case '(' :
						nextstate = OP;
						i++;
						break;
					case '\0':
						nextstate = FINISH;
						break;

					case ':':
						if(expr[i + 1] == '\0'){
							nextstate = ERROR;
							break;
						}
						if(expr[i + 1] == 'q' && expr[i + 2] == '\0'){
							t.type = QUIT;
							return t;
							break;
						}
					case 's': case 'c': case 't':
						nextstate = TRIGN;
						i++;
						break;
					case 'r':
						nextstate = ROOT;
						i++;
						break;
					case 'S' :
						nextstate = CHARACTER;
						i++;
						break;
					default:                            // anything else
						nextstate = ERROR;
						break;
				}
				currstate = nextstate;
				break;
			case NUMBER :
                switch(currchar){
                    case '0':case '1':case '2': case '3':
					case '4':case '5':case '6': case '7':
					case '8':case '9':
						nextstate = NUMBER; 
						i++;
						break;
					case '.':
						if(point == 0){
							nextstate = NUMBER; 
							point = 1;
							i++;
							
						}else{
							nextstate = ERROR;
							
						}break;
                    case '+': case '-': case '*': case '/': case '%': case '(': case ')':case '^':
						if(currchar== '('){
							nextstate = ERROR;
							currstate = nextstate;
							break;
						}
						nextstate = OP;
						t.type = OPERAND;
                        num = getbig_num(expr, start_index, i, sign);  
						t.data.num = num;
						currstate = nextstate;
						i++;
						point = 0;
						return t;
						break;
					case '\0':
						nextstate = FINISH;
						t.type = OPERAND;
						num = getbig_num(expr, start_index, i, sign);
                        t.data.num = num;
						currstate = nextstate;
						point = 0;
						return t;
						break;
					default: // anything else
						nextstate = ERROR;
						break;                                                                                     
				}
                currstate = nextstate;
			    break;
            case OP :
                switch(currchar) {
					case '0':case '1':case '2': case '3':
					case '4':case '5':case '6': case '7':
					case '8':case '9':case '.' :
                        start_index = i;
						sign = 0;
						nextstate = NUMBER;
						if(currchar == '.'){
							point = 1;
						}
						t.type = OPERATOR;
						t.data.op = expr[i - 1];
						currstate = nextstate;
                        i++;
						return t;
						break;
					case '+': case '-': case '*': case '/': case '%': case '(': case ')': case '^':
						if(expr[i-1] == ')'){
							if(currchar == '('){
								nextstate = ERROR;
								currstate = nextstate;
								break;	
							}nextstate = OP;
							t.type = OPERATOR;
							t.data.op = expr[i - 1];
							currstate = nextstate;
							i++;
							return t;
							break;
						}else if(currchar == '-'){
							nextstate = UNARY;
							t.type = OPERATOR;
							t.data.op = expr[i - 1];
							currstate = nextstate;
							i++;
							return t;
							break;
						}
						else if(currchar != '(') {
							nextstate = ERROR;
							currstate = nextstate;
							break;
						}
						nextstate = OP;
						t.type = OPERATOR;
						t.data.op = expr[i - 1];
						currstate = nextstate;
						i++;
						return t;
						break;
					case '\0':
						if(expr[i - 1] != ')') {
							nextstate = ERROR;
							currstate = nextstate;
							break;
						}
                        nextstate = FINISH;
						t.type = OPERATOR;
						t.data.op = expr[i - 1];
						currstate = nextstate;
						return t;
						break;
					case 's': case 'c' : case 't':
						nextstate = TRIGN;
						t.type = OPERATOR;
						t.data.op = expr[i - 1];
						currstate = nextstate;
						i++;
						return t;
						break;
					case 'r':
						nextstate = ROOT;
						t.type = OPERATOR;
						t.data.op = expr[i - 1];
						currstate = nextstate;
						i++;
						return t;
						break;
					default: // anything else
						nextstate = ERROR;
						currstate = nextstate;
					 	break;
				}
				break;
            case FINISH :
                t.type = END;
				start_index = sign = key = i = point = 0;           //Reseting statics to their start values fro next expression.
				currstate = START;
				return t;
				break;
            case ERROR :
				t.type = ERR;
				start_index = sign = key = i = point = 0;           //Reseting statics to their start values fro next expression.
				currstate = START;
				return t;
				break;
            
			case UNARY :
				switch(currchar) {
					case '0':case '1':case '2': case '3':
					case '4':case '5':case '6': case '7':
					case '8':case '9':case '.':
                        start_index = i;
						sign = 1;
						if(currchar == '.'){
							point = 1;
						}
						nextstate = NUMBER;
						currstate = nextstate;
                        i++;
						break;
					case '+': case '-': case '*': case '/': case '%': case '(': case ')': case '^':
						if(currchar != '(') {
							nextstate = ERROR;
							currstate = nextstate;
							break;
						}
						nextstate = OP;
						t.type = UNARY_OPERATOR;

						//Replaced unary minus symbol with '@'
						
						t.data.op = '@';
						currstate = nextstate;
						i++;
						return t;
						break;
					case 'r':
						nextstate = ROOT;
						t.type = UNARY_OPERATOR;
						//Replaced unary minus symbol with '@'
						t.data.op = '@';
						currstate = nextstate;
						i++;
						return t;
						break;
					case 's': case 'c' : case 't':
						nextstate = TRIGN;
						t.type = UNARY_OPERATOR;
						//Replaced unary minus symbol with '@'
						t.data.op = '@';
						currstate = nextstate;
						i++;
						return t;
						break;
					default: 							// anything else
						nextstate = ERROR;
						currstate = nextstate;
					 	break;
				}
				break;
            case TRIGN :
				switch(currchar) {
					case '.' :
						if(point == 0){
							j = 1;
							nextstate = TRIGN; 
							point = 1;
							i++;
							
						}else{
							nextstate = ERROR;
							
						}break;	
					case '0':case '1':case '2': case '3':
					case '4':case '5':case '6': case '7':
					case '8':case '9':
						d = currchar - '0';
						if(j == 1)
							k++;
						f = f * 10 + d;
						nextstate = TRIGN;
						i++;
						break;
					case '(':
						trig =expr[i - 1]; 
						nextstate = TRIGN;
						i++;
						break;
					case ')':
						nextstate = TRIGN;
						f = f / pow(10, k);
						k = 0;
						j = 0;
						if(trig == 's') 
							num = sine(f);
						if(trig == 'c')
							num = cosine(f);
						if(trig == 't')
							num = tangent(f);
						t.type = OPERAND;
						t.data.num = num;
						i++;
						currstate = nextstate;
						return t;
						break;
					case '\0':
						if(expr[i - 1] != ')') {
							nextstate = ERROR;
							break;
						}
                        nextstate = FINISH;
						break;
					case '+': case '-': case '*': case '/': case '%': case '^':
						if(expr[i - 1] != ')') {
							nextstate = ERROR;
							break;
						}
						nextstate = OP;
						i++;
						break;
					default: 							// anything else
						nextstate = ERROR;
					 	break;
					
				}		
				currstate = nextstate;
				break;
			case CHARACTER :
				switch(currchar){
					case '=':
						
						if(k == 0){                                 // If k is already 1 means before we have encounter '='
							nextstate = CHARACTER;
							currstate = nextstate;
							key = 1;							
							k = 1;                  				//Setting k = 1 as we encounter '=' first time
							i++;
						
						}else{
							nextstate = ERROR;
							currstate = nextstate;	
						}break;
					case '0':case '1':case '2': case '3':
					case '4':case '5':case '6': case '7':
					case '8':case '9': case '.' :
						nextstate = CHARACTER;
						if(key == 1){								//key = 0 means we don't encounter '=' before in expr
							j = 1;									//Setting j = 1 for getting further number.
							key = 0;								//Reseting key as we want to set start_index only once.
							start_index = i;
							sign = 0;
							if(currchar == '.'){
								point = 1;                         // Means we have encounter dot for first time.
							}
							currstate = nextstate;
							i++;
							
						}else if(j == 1){
							if(currchar == '.' && point == 1){
								nextstate = ERROR;
								currstate = nextstate;
								break;
							}
							i++;
							currstate = nextstate;
							
						}else{
							nextstate = ERROR;
							currstate = nextstate;
							
						}break;
					case '\0' :
						if(j == 1){
							t.type = KEYWORD;
							num = getbig_num(expr, start_index, i, sign);
							t.data.sc = Btoi(num);
							start_index = sign = key = i = 0;
							currstate = START;
							return t;
						}else{
							nextstate = ERROR;
							currstate = nextstate;
						}break;
					default :
						nextstate = ERROR;
						currstate = nextstate;
					 	break;
				}
				break;
			case ROOT :
				switch(currchar){
					case '(':
						nextstate = OP;
						t.type = OPERATOR;
						t.data.op = expr[i - 1];
						i++;
						currstate = nextstate;
						return t;
						break;
					default :
						nextstate = ERROR;
						currstate = nextstate;
						break;
				}break;

        }
    }

}


/*
    									  Infix Evaluator using Two stacks

*       Take one token at a time and put it in respective stack or does action accordingly
*       Also then evaluation.
*       Also gives error according to invalid expression
*		Cosider all types of invalid expressions

*/

// This function gives precedence of each operator 

int precedence(char c){
    switch(c){
        case '+':
        case '-':
            return 1;
            break;
        case '*':
        case '/':
        case '%':
            return 2;
            break;
        case '^':
            return 3;
            break;
        case '@':                              // '@' is unary minus
            return 4;
            break;
        case 'r' :							  // 'r is square root
            return 5;
            break;
        default:
            return 0;
    }
}

/* This function will perform respective operation on top two big numbers of stack according to top operator in operator stack.
    Return type : Returns number of type BIG.
*/

BIG perform_operation(big_stack *st_num, stack * st_op){
    BIG first , second;
    BIG ret_num;
    second = pop_stack_big(st_num);                           // Getting two top big numbers in stack
    first = pop_stack_big(st_num);
    char op = pop(st_op);									 // Getting top operator from operator stack
    
	// Switching according to operator

	switch(op){
        case '@':
            push_stack_big(st_num, first);
            second->sign = (second->sign + 1) % 2;
            ret_num =  second; 
            break;
        case '+':
            ret_num =  add_big(first, second);
            delete_big(&first);
            delete_big(&second);
            break;
        case '-':
            ret_num =  sub_big(first, second);
            delete_big(&first);
            delete_big(&second);
            break;
        case '*':
            ret_num =  mult_big(first, second, SCALE);
            delete_big(&first);
            delete_big(&second);
            break;
        case '/':
            if (is_num_zero(second)){                              // Handling errors
                delete_big(&first);
                delete_big(&second);                             
                destroy_stack_big(st_num);
                destroy(st_op);
                printf("ERROR :: Division by zero.\n");
                return NULL;
            }ret_num =  divide_big(first, second, SCALE);
            delete_big(&first);
            delete_big(&second);
            break;
        case '%' :
            ret_num = mod_big(first, second, SCALE);
            delete_big(&first);
            delete_big(&second);
            break;
        case '^':
            if(second->type == 'f'){                           
                printf("WARNING :: non-zero scale in exponent\n");
            }
            int exp = 0;
            if((exp = Btoi(second)) != INT_MIN){
                ret_num = pow_big(first, exp, SCALE);
                delete_big(&first);
            }else{                                                           // Handling errors
                printf("ERROR :: exponent too large in raise.\n");
                delete_big(&first);
                return NULL;
            }
            break;
        case 'r':
            push_stack_big(st_num, first);
            if(second->sign == 1){                                           // Handling errors
                printf("ERROR :: Square root of negative number.\n");
                delete_big(&second);
                return NULL;
            }ret_num = sqrt_big(second, SCALE);
    }return ret_num;
}

/*
    It evaluates infix expression using following algorithm
        1. If the token is an operand, push it to the operand stack.
        2. If the token is an operator,
            A. If the operator stack is empty 
                    then push it to the operator stack.
            B. Else If the operator stack is not empty,
                I . If the token’s precedence is greater than the precedence of the stack top of the operator stack,
                         then push the token to the operator stack.
                II. If operator is '^' push it into operator stack.
 				III.If the token’s precedence is less than or equal to the precedence of the stack top of the operator stack
                         then do Process until token’s precedence is less or stack is not empty.
        3. If the token is “(“,
            then push it onto the operator stack.
        4. If the token is “)”, 
            then do Process until the corresponding “(” is encountered in operator stack. Now just pop out the “(“.
        Once the expression iteration is completed and the operator stack is not empty, 
            do Process until the operator stack is empty.

        ****The values left in the operand stack is our final result.*****

    Parameters : Takes expression as parameter.
    Return Type : Returns big number of type BIG which is answer of that expression if no error occurs
					else return special type of BIG number which indicate error accordingly.
					eg . SUCCESS() if user has chaged scale
						 EXIT_LOOP() if user wants quit program
						OPERATION_ERROR() if error occured in operation
						EXPRESSION_ERROR() if expression is invalid



*/
BIG infix_eval(char *expr){
    big_stack st_num;
    BIG num;
    stack st_op;
    init(&st_op);
    init_stack_big(&st_num);
    token t;
    while(1){
		// gets each token from expr
        t = get_token(expr);

        // if user changed scale
        if(t.type == KEYWORD){
            if(t.data.sc != INT_MIN){
                SCALE = t.data.sc;
                
            }else{                               				    // if scale value is out of range
                printf("ERROR :: scale too large, set in range of int.\n");
            }
            return SUCCESS();
        }
		//if user want to quit from program( i.e if user enters ':q')
        if(t.type == QUIT){
            return EXIT_LOOP();
        }

		// Switching According to token type
        switch (t.type){
            case OPERAND:
                push_stack_big(&st_num, t.data.num);
                break;
            case OPERATOR :
                if(isEmpty(st_op)){
                    push(&st_op, t.data.op);
                }else if(t.data.op == '('){
                    push(&st_op, t.data.op);
                }

                //Closed brace, evaluate the entire brace
                else if(t.data.op ==')') {
                    while(peek(st_op) != '('){
                        
                        num = perform_operation(&st_num , &st_op);
                        if(!num){
                            destroy_stack_big(&st_num);
                            destroy(&st_op);
                            return OPERATION_ERROR();
                        }
                        //push it back to stack
                        push_stack_big(&st_num, num);
                    }
                    pop(&st_op);
                }
                else if(t.data.op == '^'){
                    push(&st_op, t.data.op);
                }
                /*1. If current operator has higher precedence than operator on top of the stack,
                the current operator can be placed in stack
                2. else keep popping operator from stack and perform the operation in  numbers stack till
                either stack is not empty or current operator has higher precedence than operator on top of the stack
                */
                else{
                    while(!isEmpty(st_op) && precedence(t.data.op) <= precedence(peek(st_op))){
                        
                        num = perform_operation(&st_num , &st_op);
                        if(!num){
                            destroy_stack_big(&st_num);
                            destroy(&st_op);
                            return OPERATION_ERROR();
                        }
                        //push it back to stack
                        push_stack_big(&st_num, num);
                    }
                    //Now push the current operator to stack
                    push(&st_op, t.data.op);
                }
                break;
            case UNARY_OPERATOR :
                push(&st_op, t.data.op);
                break;
            case END :
                while(!isEmpty(st_op)){
                    
                    num = perform_operation(&st_num , &st_op);
                    if(!num){
                            destroy_stack_big(&st_num);
                            destroy(&st_op);
                            return OPERATION_ERROR();
                     }
                    //push it back to stack
                    push_stack_big(&st_num, num);
                }
                return pop_stack_big(&st_num);
                break;
            case ERR :
                destroy_stack_big(&st_num);
                destroy(&st_op);
                return EXPRESSION_ERROR();
                break;
        }
    }
}


