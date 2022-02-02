#include<stdio.h>
#include<stdlib.h>
#include<limits.h>
#include<unistd.h>
#include<sys/stat.h>
#include <fcntl.h>
#include "expression_evaluate.h"
#include "bignum.h"
//Initialize function will initialize big number with 0 and as integer type

void init_big(BIG *num){
    *num = (node*)malloc(sizeof(node));
    (*num)->digits = (int*)calloc(2, sizeof(int));
    (*num)->digits[0] = 2;
    (*num)->fl = NULL;
    (*num)->sign= 0;
    (*num)->type = 'i';
}

// Returns maximum of two integers

int max(int a, int b){
    return (a>b)?a:b;
}

// Returns minimum of two integers

int min(int a, int b){
    return a<b?a:b;
}

 // Deletes big number by free all malloced memory

void delete_big(BIG *num){
    if(!(*num)){
        return;
    }
    free((*num)->digits);
    if((*num)->type == 'f')
        free((*num)->fl);
    free(*num);
    *num = NULL;
}

// Reverses array element
// Only reverses digits in big number so i start from 1

void reverse_array(int *arr){
    
    int size = arr[0];
    int temp;
    for(int i = 1, j = size - 1; i < j ; i++, j--){
        temp = arr[i];
        arr[i] = arr[j];
        arr[j] = temp;
    }
}

// Checks whether big number is zero or not
// Returns 1 if number is zero else returns 0

int is_num_zero(BIG num){
    if(num->type == 'f'){
        if(num->fl[0] > 2){
            return 0;
        }if(num->fl[0] == 2 && num->fl[1] !=0){
            return 0;
        }
    }
    if(num->digits[0] == 2 && num->digits[1] == 0){
        return 1;
    }else{
        return 0; 
    }
}

// Prints big number

void display_big(BIG num){
    if(!num){
        return;
    }
    if(num->sign == 1){                               // If number is negative
        printf("-");
    }
    for(int j = num->digits[0]-1; j > 0;j--){
        printf("%d", num->digits[j]);
    }
    
    if(num->type == 'f'){                            // If number is float
        printf(".");
        for(int j = 1; j < num->fl[0]; j++){
            printf("%d", num->fl[j]);
        }printf("\n");
    }else{
        printf("\n");
    }
}

// clear zero will discard zeros in number which not affect the number eg. 0001.000 = 1.0

void clear_zeros(BIG num){
    int no_digits = num->digits[0] - 1;
    int i;
    for(i = no_digits; i > 1 ; i--){
        if(num->digits[i] == 0){                       //checks for zeros from MSD in digits part
            continue;
        }else{
            break;
        }
    }
    if( i != no_digits){
        num->digits = realloc(num->digits, sizeof(int) * (i + 1));
        num->digits[0] = i + 1;
    }if(num->type == 'f'){
        no_digits = num->fl[0] - 1;
        for(i = no_digits; i > 1 ; i--){
            if(num->fl[i] == 0){                     //checks for zeros in fl part    
                continue;
            }else{
                break;
            }
        }
        if( i != no_digits){
            num->fl = realloc(num->fl, sizeof(int) * i + 1);
            num->fl[0] = i + 1;
        }
    }
}

//Shift decimal point to left by given positions

void decimal_shift_left(BIG num, int d){
    if(d < 1){                                                   // if d < 1 then invalid
        return;
    }
    int no_digits;

    // If number is of type 'i'
    if(num->type == 'i'){           
        no_digits = 1;
        num->type = 'f';
    }else{
        no_digits= num->fl[0];
        reverse_array(num->fl);
    }
    num->fl = realloc(num->fl,sizeof(int) *(no_digits + d));
    num->fl[0] = no_digits + d;
    if(num->digits[0] <= d ){
        for(int i = 1; i < num->digits[0]; i++, no_digits++){
            num->fl[no_digits] = num->digits[i];
        }
        for(int i = no_digits; i < num->fl[0]; i++){
            num->fl[i] = 0;
        } 
    }
    else{
        for(int i = 1; i <= d; i++, no_digits++){
            num->fl[no_digits] = num->digits[i];
        }
    }reverse_array(num->fl);

    // If less number of digits in digits part but we want shift more
    if(num->digits[0] <= d + 1 ){
        num->digits = realloc(num->digits,sizeof(int) * 2);
        num->digits[0] = 2;
        num->digits[1] = 0;
        clear_zeros(num);
        return; 
    }

    reverse_array(num->digits);
    num->digits = realloc(num->digits, sizeof(int) *(num->digits[0] - d));

    num->digits[0] = num->digits[0] - d;
    reverse_array(num->digits);
}

//Shift decimal point to right by given positions

void decimal_shift_right(BIG num, int d){
    if(d < 1){                                       // if d < 1 then invalid
        return;
    }
    int no_digits;
    no_digits= num->digits[0];

    // If number is of type 'i'
    if(num->type == 'i'){
        reverse_array(num->digits);
        num->digits = realloc(num->digits, sizeof(int) * (no_digits + d));    
        num->digits[0] = no_digits + d;
        for(int i = no_digits ; i < no_digits + d; i++){
            num->digits[i] = 0;
        }
        reverse_array(num->digits);
        clear_zeros(num);
        return;
    }
    reverse_array(num->digits);
    num->digits = realloc(num->digits,sizeof(int) *(no_digits + d));
    num->digits[0] =no_digits + d;
    if(num->fl[0] <= d){
        for(int i = 1; i < num->fl[0]; i++, no_digits++){
            num->digits[no_digits] = num->fl[i];
        }
        for(int i = no_digits; i < num->digits[0]; i++){
            num->digits[i] = 0;
        }
    }else{
        for(int i = 1; i <= d; i++, no_digits++)
            num->digits[no_digits] = num->fl[i];
    }
    reverse_array(num->digits);

    // If less number of digits in fl but we want shift more
    if(num->fl[0] <= d + 1){
        free(num->fl);
        num->fl = NULL;
        num->type = 'i';
        clear_zeros(num);
        return; 
    }

    reverse_array(num->fl);
    num->fl = realloc(num->fl,sizeof(int) *(num->fl[0] - d));
    num->fl[0] -= d;
    reverse_array(num->fl);
    clear_zeros(num);
}
/*
    Compares two big nummbers
    Returns : 1      if  a > b
              0      if  a < b
             -1      if  a = b
*/
int compare_big(BIG a, BIG b){
        int i;                          /* counter */
        clear_zeros(a);
        clear_zeros(b);
        
        //Comparing according to sign

        if ((a->sign==1) && (b->sign==0)) return 0;
        if ((a->sign==0) && (b->sign==1)) return 1;
       
       //Comaparing according to number of digits 
        if (b->digits[0] > a->digits[0]) return 0 + a->sign;
        if (a->digits[0] > b->digits[0]) return 1- a->sign;
        
        //if number of digits are same then
        for (i = a->digits[0] - 1 ; i > 0 ; i--) {
                if (a->digits[i] > b->digits[i]){
                        return 1 - a->sign;
                }if (b->digits[i] > a->digits[i]){
                         return 0 + a->sign;
                }
        }
        
        // If numbers are float and digit part is already same

        if(a->type =='f' && b->type !='f'){
            if(a->fl[0] == 2 && a->fl[1] == 0){
                return -1;
            }else{
                return 1 - a->sign;
            }
        }else if(a->type !='f' && b->type =='f'){
            if(b->fl[0] == 2 && b->fl[1] == 0){
                return -1;
            }else{
                return 0 + a->sign;
            }
        }else if(a->type !='f' && b->type !='f'){
            return -1;
        }else{
            int min_size = min(a->fl[0], b->fl[0]);
            for (i = 1 ; i < min_size ; i++) {
                if (a->fl[i] > b->fl[i]){
                        return 1 - a->sign;
                }if (b->fl[i] > a->fl[i]){
                        return 0 + a->sign;
                }
            }if(min_size == b->fl[0]){
                return 1 - a->sign; 
            }else{
                return 0 + a->sign;
            }
        }
}

// Returns scale of big number ( number of digits in float part)

int scale_of_big(BIG num){
     if(num->type == 'i'){
         return 0;
     }else{
         return num->fl[0] - 1;
     }
}

// Copies big numbers and return it

BIG clone_big(BIG num){
    BIG copy_num;
    init_big(&copy_num);
    copy_num->type = num->type;
    copy_num->sign = num->sign;
    copy_num->digits = realloc(copy_num->digits, sizeof(int) * num->digits[0]);
    for(int i = 0; i < num->digits[0]; i++){
        copy_num->digits[i] = num->digits[i];
    }if(num->type == 'f'){
        copy_num->fl = realloc(copy_num->fl, sizeof(int) * num->fl[0]);
        for(int i = 0; i < num->fl[0]; i++){
            copy_num->fl[i] = num->fl[i];
        }   
    }return copy_num;
}

 // Convert big number to integer
 // Also deletes big number after converting

int Btoi(BIG num){
    int sum = 0, d;
    if(num->digits[0] > 10){
        return INT_MIN;
    }
    for(int i = 1, k = 1; i < num->digits[0] ; i++){
        d = num->digits[i];
        sum += k * d;
        k *= 10; 
    }
    if(num->sign == 1){
        sum = (-1)* sum ;
    }
    delete_big(&num);
    return sum;
}

// Conbert long double to big number

BIG ftoB(long double x){
    char str[50];
	sprintf(str, "%.15Lf", x);                         // Storinf upto 15 decimals of long double in buffer
	BIG num;
    int i = 0, len = 0,sign = 0;
	
    if(str[0] == '-'){
        sign = 1;
        i = 1;
    }
    len = i;
    while(str[len] != '\0'){
        len++;
    }
    num = getbig_num(str, i, len, sign);
	return num;
}

  // Saves big number into file given by file descriptor

void History(BIG answer, int fd){
	char c[3];
    write(fd , "\n", 1);
	if(answer->sign == 1)
		write(fd, "-", 1);
	for(int j = answer->digits[0]-1; j > 0;j--){
        sprintf(c, "%d", answer->digits[j]);
        write(fd, c, 1);
    }
    if(answer->type == 'f'){ 
        write(fd, ".", 1);
        for(int j = 1; j < answer->fl[0]; j++){
            sprintf(c, "%d", answer->fl[j]);
            write(fd, c, 1);
        }
    }
    write(fd , "\n", 1);
}