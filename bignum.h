/* 
	Header file contains prototypes for functions of big numbers
    And also contains ADT BIG 
	
*/
#ifndef BIG_NUM_H_INCLUDED
#define BIG_NUM_H_INCLUDED

/*                                          Data Type for our bignum
        1. BIG can store integers as well as floats
        2. BIG can store negative numbers as well
        3. In digits , digits which are left to decimal point are stored in reversed order
        4. In fl     , digits which are right to decimal point are strored(not in reversed order)
        5. In both 0th index will contains lenght of array( i.e no of digits + 1)
        eg. -123456.789 will be stored as 
            type   = 'f'
            sign   = 1
            digits ->  | 6 | 5 | 4 | 3 | 2 | 1 |
            fl     ->  | 7 | 8 | 9 |
*/
typedef struct node{
    char type;                              // Stores type of number ( 'i'  or   'f')
    int sign;                               // Stores sign of number ( 0 for Positive or 1 for Negative)
    int *digits;                            // Stores digits in part other than float from number in reverse order
    int *fl;                                // Stores floating digits from number
}node;

typedef struct node* BIG;                   // Typedef for pointer of node


void init_big(BIG *num);                    // Initializes big number

int max(int a, int b);                      // Find maximum of two integer

int min(int a, int b);                      // Find minimum of two integer

void delete_big(BIG *num);                  // Deletes big number

void reverse_array(int *arr);               // Reverses array element

int is_num_zero(BIG num);                   // Checks whether big number is zero or not

void display_big(BIG num);                  // Prints big number

void clear_zeros(BIG num);                  // clear zero will discard zeros in number which not affect the number eg. 0001.000 = 1.0

void decimal_shift_left(BIG num, int d);    // Shift decimal point of big number to left

void decimal_shift_right(BIG num, int d);   // Shift decimal point of big number to right

int compare_big(BIG a, BIG b);              // Compares two big number

int scale_of_big(BIG num);                  // Returns scale of big number

BIG clone_big(BIG num);                     // Copies big numbers and return it

int Btoi(BIG num);                          // Convert big number to integer

BIG ftoB(long double x);                    // Conbert long double to big number

void History(BIG answer, int fd);           // Saves big number into file given by file descriptor

#endif // BIG_NUM_H_INCLUDED