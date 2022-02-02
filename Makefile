apc : main.o bignum.o big_operations.o big_error.o stack.o stack_big.o expression_evaluate.o
		gcc -Wall -o apc main.o bignum.o big_operations.o big_error.o stack.o stack_big.o expression_evaluate.o -lm

main.o : main.c bignum.h expression_evaluate.h big_error.h
		gcc -c -Wall main.c

bignum.o : bignum.c bignum.h expression_evaluate.h
		gcc -c -Wall bignum.c

big_operations.o : big_operations.c bignum.h big_operations.h
		gcc -c -Wall big_operations.c

big_error.o : big_error.c bignum.h big_error.h
		gcc -c -Wall big_error.c

stack.o : stack.c stack.h
		gcc -c -Wall stack.c

stack_big.o : stack_big.c stack_big.h bignum.h
		gcc -c -Wall stack_big.c

expression_evaluate.o : expression_evaluate.c bignum.h big_operations.h big_error.h stack.h stack_big.h expression_evaluate.h
		gcc -c -Wall expression_evaluate.c

clean :
		rm -f apc *.o