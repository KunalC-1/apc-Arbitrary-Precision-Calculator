/* 
    File contains functions for arithtmetic operations on bignum.
    Arithmetic operations include  + , - , * , / , %, ^, sin, cos, tan, r (square root). 
*/

#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include "bignum.h"
#include "big_operations.h"

/*
    Function    : add_big()
    Parameters  : Two numbers of ADT BIG
    Returns     : Addition of Two big numbers
    Return Type : BIG
    Algorithm   : Adding each digit from right to left and taking carry forward
    Cases       : Eg. 1) If a is + and b is - then a + (-b)    becomes subtraction of a and b i.e a - b
                      2) If a is - and b is + then (-a) + b    becomes subtraction of b and a i.e b - a
                      3) If a is - and b is - then (-a) + (-b) becomes Additions of a and b and then changing sign i.e -(a + b)
                      4) If a is + and b is + then a + b       becomes Additions of a and b and then changing sign i.e a + b
*/
BIG add_big(BIG a, BIG b){
        BIG c;                                         // Addition of a and b is stored in c
        init_big(&c);                                   
        int carry = 0;                                 // Carry is used if overflow occurs like 9 + 9 = 8 and carry = 1

        int i, k , max_size, min_size;
        
        // Case 3 and 4
        if(a->sign == b->sign) c->sign = a->sign;      // ans will have same sign as a and b if a and b have same sign
        else{
            if (a->sign == 1) {                        // Case 2
                a->sign = 0;
                delete_big(&c);
                c = sub_big(b,a);
                a->sign = 1;
            }else {                                    // Case 1
                b->sign = 0;
                delete_big(&c);
                c = sub_big(a,b);
                b->sign = 1;
            }clear_zeros(c);                 // clear zero will discard zeros in number which not affect the number eg. 0001.000 = 1.0
            return c;
        }

        // Case 3 and 4

        if(a->type == 'f' && b->type == 'f' ){
            c->type = 'f';
            // finding minimum and maximum number of floating digits from two numbers

            max_size = max(a->fl[0],b->fl[0]);                      // At zeroth postion lenght of array is stored
            c->fl = realloc(c->fl, sizeof(int)*max_size);   
            c->fl[0]= max_size;                                     // At zeroth postion lenght of array is stored
            min_size = min(a->fl[0],b->fl[0]);                     
           
            carry = 0;
            
            // Digit by digit addtion is done until minimum size.
            
            for(i = min_size-1; i > 0; i--) {
                c->fl[i] = (carry+a->fl[i] + b->fl[i]) % 10;
                carry = (carry + a->fl[i] + b->fl[i]) / 10;
            }

            // Then taking remainig digits as it is from number having more floating digits

            for (i=min_size; i < max_size; i++) {
                c->fl[i] = (a->fl[0] > b->fl[0])? a->fl[i] : b->fl[i];
            }
        }else{

            // if one of two number is float then taking its floating digits as it is in answer

            if(a->type == 'f'){
                c->type = 'f';
                c->fl = realloc(c->fl, sizeof(int)*a->fl[0]);
                c->fl[0]= a->fl[0];
                for (i=1; i < a->fl[0]; i++) {
                    c->fl[i] = a->fl[i];
                }
            }else if(b->type == 'f'){
                c->type = 'f';
                c->fl = realloc(c->fl, sizeof(int)*b->fl[0]);
                c->fl[0] =b->fl[0];
                for (i=1; i < b->fl[0]; i++) {
                    c->fl[i] = b->fl[i];
                }
            }
        }
        // Addition of other than floating part means digits which are left to the decimal point

        max_size = max(a->digits[0],b->digits[0])+1;
        c->digits = realloc(c->digits, sizeof(int)*max_size);
        c->digits[0] = max_size;
        min_size = min(a->digits[0],b->digits[0]);
       
        for (i = 1; i < min_size; i++) {
                c->digits[i] = (carry+a->digits[i]+b->digits[i]) % 10;
                carry = (carry + a->digits[i] + b->digits[i]) / 10;
        }
        // If both numbers have same digits (digits means part other than float)

        if(a->digits[0]==b->digits[0]){
            c->digits[i] = carry;
        }
        // if first number have more digits

        else if(a->digits[0] > b->digits[0]){
            for(k = i;k < a->digits[0];k++){
                c->digits[k] = a->digits[k] + carry;
                carry = 0;
            }c->digits[k] = carry;
        }
        // second number have more digits
        else{
            for(k = i;k < b->digits[0]; k++){
                c->digits[k] = b->digits[k] + carry;
                carry=0;
            }c->digits[k] = carry;
        }
        clear_zeros(c);
        return c;
}
/*
    Function    : sub_big()
    Parameters  : Two numbers of ADT BIG
    Returns     : Subtraction of Two big numbers
    Return Type : BIG
    Algorithm   : Subtracting each digit from right to left and taking borrow forward
    Cases       : Eg. 1) a) If a is - and b is + then (-a) - b    becomes addition of a and b and then changing sign i.e -(a + b)
                         b) If a is + and b is - then a - (-b)    becomes addition of a and b i.e a + b
                         c) If a is - and b is - then (-a) - (-b) becomes addition of -a and b i.e (-a) + b   
                      2) If a < b then a - b become subtraction of b -a anf after changing sign
                      3) If a = b then a- b return 0 
*/
BIG sub_big(BIG a, BIG b){

        int borrow = 0,max_size = 0,min_size = 0;             //Borrows used to represent any borrow taken from next digit
        int v = 0;                                            // v used to get subtraction of digits         
        int i = 0;                                        
        BIG c;   

        // Case 1

        if ((a->sign == 1) || (b->sign == 1)) {
                b->sign = 1 - b->sign;
                c = add_big(a, b);
                b->sign = 1 - b->sign;
                return c;
        }

        // Case 2
        if (compare_big(a,b) == 0){                         // compare function will return 0 if a < b
                c = sub_big(b, a);
                c->sign = 1;
                return c;
        }
        init_big(&c);

        // Case 3
        if(compare_big(a,b) == -1){                         // compare function will return -1 if a = b
            return c;
        }  

        // First doing subtraction of floating part if any  
        if(a->type == 'f' && b->type == 'f' ){
            c->type = 'f';
            max_size = max(a->fl[0],b->fl[0]);
            c->fl = realloc(c->fl, sizeof(int) * max_size);
            c->fl[0] = max_size;
            min_size = min(a->fl[0], b->fl[0]);
            borrow = 0;
            // Doing digit by digit subtraction
            for (i = max_size-1; i > 0; i--) {

                // if a has more floating digits 
                if((a->fl[0] > b->fl[0]) && i > min_size-1){
                    c->fl[i] = a->fl[i];   
                }
                // if b has more floating digits
                else if((a->fl[0] < b->fl[0]) && i > min_size-1){
                    v = (0 - borrow - b->fl[i]);
                    v = v + 10;
                    borrow = 1;
                    c->fl[i] = v % 10;    
                }
                // if both have same floating digits
                else{
                    v = (a->fl[i] - borrow - b->fl[i]);
                    if (v < 0) {
                            v = v + 10;
                            borrow = 1;
                    }else{
                            borrow = 0;
                    }c->fl[i] =v % 10;
                }
            }
        }else{
            //if one of two number is float
            if(a->type == 'f'){
                c->type = 'f';
                c->fl = (int*)realloc(c->fl, sizeof(int)*a->fl[0]);
                c->fl[0]= a->fl[0];
                for (i=1; i < a->fl[0]; i++) {
                    c->fl[i] = a->fl[i];
                }
            }else if(b->type == 'f'){
                c->type = 'f';
                c->fl = (int*)realloc(c->fl, sizeof(int)*b->fl[0]);
                c->fl[0] = b->fl[0];
                for (i = b->fl[0]-1; i > 0; i--) {
                    v = (0 - borrow - b->fl[i]);
                    v = v + 10;
                    borrow = 1;
                    c->fl[i] = v % 10;
                }
            }
        }
        // Digit by digit subtraction of remaining part other than float
        max_size = max(a->digits[0], b->digits[0]);
        c->digits = (int*)realloc(c->digits, sizeof(int) * max_size);
        c->digits[0] = max_size;
        min_size = min(a->digits[0],b->digits[0]);
        
        for (i = 1; i < min_size; i++) {
                v = (a->digits[i] - borrow - b->digits[i]);
                if (v < 0) {
                        v = v + 10;
                        borrow = 1;
                }else
                        borrow = 0;
                c->digits[i] =v % 10;
        }

        // if a and b have same no of digits in digits part
        if(a->digits[0]==b->digits[0]){
            clear_zeros(c);
            return c;
        }
        // b cannot have more digits than a because that is handled in case 2

        //if a have more digits in digit part
        else{
            for(;i < a->digits[0];i++){
                v = a->digits[i] - borrow;
                if (v < 0) {
                        v = v + 10;
                        borrow = 1;
                }else{
                        borrow = 0;
                }c->digits[i] =v % 10;
            }
        }clear_zeros(c);
        return c;
}
/*
    Function    : mult_big()
    Parameters  : Two numbers of ADT BIG and scale
    Returns     : Multiplication of Two big numbers
    Return Type : BIG
    Algorithm   : Doing row by row
                  eg.      123
                        x   23
                        ---------
                           369               Adding 123 ........ 3 times   = 369
                        + 2460         369 + Adding 1230........ 2 times   = 2829
                        ---------
                          2829 
                In worst case each addition will happen maximum 9 times only
                123 -> 1230 just by shifting one decimal to right by 1
                If second number is float then shift decimal of answer to right by that much postions
    
    Cases       : Eg. 1) If one of the two, one is zero then answer will be 0
                      2) a) If a is + and b is + answer will be +
                         b) If a is + and b is - answer will be -
                         c) If a is - and b is + answer will be -
                         d) If a is - and b is - answer will be +
*/
BIG mult_big(BIG a, BIG b, int scale){
	BIG c, temp;
    init_big(&c);

    // Case 1
    if(is_num_zero(a) || is_num_zero(b)){
        return c;
    }
	int a_sign, b_sign;                             //Store signs of a and b
    int no_digits;
	a_sign = a->sign;
    b_sign = b->sign;
    a->sign = 0;                                    //Make both numbers positive
    b->sign = 0;

	// If second number is float
	if(b->type == 'f'){
		no_digits = b->fl[0]-1;
		for (int i = no_digits; i > 0; i--){ 
			for (int j = 1; j <= b->fl[i]; j++){
      			temp = add_big(c, a);
				delete_big(&c);
			    c = temp;
                
			}decimal_shift_right(a, 1);                       //Multiplying a with 10 using shfting decimal by 1
		}
	}
	no_digits = b->digits[0];                                 // no_digits stores no of digits in b' digit part
	for (int i = 1; i < no_digits; i++){ 
        for (int j = 1; j <= b->digits[i]; j++){
            temp = add_big(c, a);
		    delete_big(&c);
		    c = temp;
		}
        decimal_shift_right(a, 1);                           //Multiplying a with 10 using shfting decimal by 1
	}

    //if second number is float
    if(b->type == 'f'){
        decimal_shift_left(c , b->fl[0]-1);                 // Shifting decimals of answer
    }
    a->sign = a_sign;
    b->sign = b_sign;
    
    c->sign = (a_sign + b_sign) % 2 ;                       // Case 2

    /*if given scale is greater than or equal to the scale of answer of multiplication 
                then answer remains unchanged.
      else
                scale of answer of multiplication will be maximum(scale, maximum(scale_of(a), scale_of(b))) 
    */
    if(c->type == 'f'){ 
        if(scale < scale_of_big(c)){
            scale = max(scale, max(scale_of_big(a), scale_of_big(b)));
            c->fl = realloc(c->fl, sizeof(int) * (scale + 1));
            c->fl[0] = scale + 1;
        }
    }clear_zeros(c);
    return c;
}
/*
    Function    : divide_big()
    Parameters  : Two numbers of ADT BIG and scale
    Returns     : Division of Two big numbers upto given scale
    Return Type : BIG
    Algorithm   : Doing row by row
                  eg. 123 / 5 
                    Take first digit of divident in temp i.e tmep = 1
                    while( temp < divisor)
                        Do repeated subtraction 
                        Increment corresponding digit in quotient
                    Decimal Shift temp to right by 1 i.e temp = 10
                    Then take next digit in temp i.e temp = 12
                    Do all steps upto desired scale

                In worst case each subtraction will happen maximum 9 times only
                If denominator is float then shift decimal of answer to right by that much postions and make denominator as int
    
    Cases       : Eg. 1) If is numerator is zero then answer will be 0
                      2) a) If a is + and b is + answer will be +
                         b) If a is + and b is - answer will be -
                         c) If a is - and b is + answer will be -
                         d) If a is - and b is - answer will be +
                    Other cases handled in expression evaluator.

*/

BIG divide_big(BIG a, BIG b, int scale){
    BIG temp, remainder, c;
    init_big(&c);
    // Case 1
    if(is_num_zero(a)){
        return c;
    }
    int decimal_places;                                        // It will store decimal places in denominator for shifting
    init_big(&remainder);
    c->sign = (a->sign + b->sign) % 2;                         //For sign of quotient   
    a->sign = b->sign = 0;                                     // Making both number as positive
    
    // if denominator is floating
    if(b->type == 'f'){
        decimal_places= b->fl[0] - 1 ;
        decimal_shift_right(b, decimal_places);                //Removing decimal point from denominator and
        decimal_shift_right(a, decimal_places);                //Shifting that much of decimals in numerator
        
    }
    for(int i = a->digits[0] - 1, k = 1; i > 0; i--){
        decimal_shift_right(remainder, 1);                     //Multiplying remainder with 10 (i.e. Shifting to right by 1)
        remainder->digits[1] = a->digits[i];
        
        c->digits = realloc(c->digits, sizeof(int) * (k + 1));
        
        c->digits[0] = k + 1;                                 // Storing size of array in first position
        c->digits[k] = 0;                                     // Initializing digit in quotient with zero
        
        while(compare_big(remainder , b)){                    //Comparing remainder with b and doing repetitive subtraction
            
            c->digits[k] ++;                                  // if remainder >= b
            temp = sub_big(remainder, b);                     // Runs maximum 9 times in worst case
            delete_big(&remainder);
            remainder = temp;
        }k++;                                                 // k is size of digits array
    }
    reverse_array(c->digits);
    clear_zeros(c);

    // Doing above step of floating part and according to scale
    if(scale > 0){
        c->type = 'f' ;
        for(int i = 1, k = 1; i <= scale  ; i++){
            decimal_shift_right(remainder, 1);                //Multiplying remainder with 10 (i.e. Shifting to right by 1)
            
            // if floating part in a ends then taking 0
            if(a->type == 'f' && i < a->fl[0])    
                remainder->digits[1] = a->fl[i];
            else
                remainder->digits[1] = 0;
            
            c->fl = realloc(c->fl, sizeof(int) * (k + 1));
            c->fl[0] = k + 1;
            c->fl[k] = 0;
            while(compare_big(remainder , b)){              //Comparing remainder with b and doing repetitive subtraction
                c->fl[k] ++;
                temp = sub_big(remainder, b);
                delete_big(&remainder);                     // deleting number which will not be used further
                remainder = temp;
            }k++;                                           // k is size of floating digits array
        }
    }
    delete_big(&remainder);
    return c;
}
/*
    Function    : mod_big()
    Parameters  : Two numbers of ADT BIG and scale
    Returns     : Modulo of Two big numbers cosidering given scale
    Return Type : BIG
    Algorithm   : a % b = a - b * (a / b)
                  c = (a / b) is done with given scale
                  and then b * c is done with new scale
                  new scale = maximum (scale + scale(b) , scale(a))
                  Sign will be as per first expression
                If scale is set to zero and both expressions are integers
                this expression is the integer remainder function.

*/
BIG mod_big(BIG a,BIG b, int scale){
    
    BIG mod, temp;
    BIG copy_a, copy_b;                                     // Creating copy as our multi and divide function may modify given numbers
    int new_scale;

    // Finding new scale
    new_scale = max(scale + scale_of_big(b), scale_of_big(a));
    
    copy_a = clone_big(a);                                  // Clone function will create copy of number and returns it
    copy_b = clone_big(b);

    mod = divide_big(copy_a, copy_b, scale);                // a / b
    delete_big(&copy_a);
    delete_big(&copy_b);

    temp = mult_big(mod, b, new_scale);                    // (a / b) * b
    delete_big(&mod);
    
    mod = temp;
    temp = sub_big(a, mod);                                // a - (a / b) * b
    delete_big(&mod);
    
    mod = temp;
    return mod;
}
/*
    Function    : pow_big()
    Parameters  : Two numbers of ADT BIG and scale
    Returns     : Answer of a raised to b
    Return Type : BIG
    Algorithm   : Using Recursive algorithm
                 a ^ b =(a ^ (b / 2)) * (a ^ (b / 2)) * (a ^ (b mod 2))
                 Base condition if b = 0 then answer = 1

    Cases       : 1) If b = 0 then answer = 1
                  2) Negative powers
*/
BIG pow_big(BIG a , int b, int scale){
    BIG temp, temp1;
    
    //Case 1
    if(b == 0 ){
        BIG one_big;                                    // Creating 1 in BIG type
        init_big(&one_big);
        one_big->digits[1] = 1;
        return one_big;
    }

    // Calculating a ^ (b / 2)
    temp = pow_big(a, b/2, scale);
    BIG copy_temp = clone_big(temp);                    // Cloning temp
    
    // Calculating temp * temp
    temp1 = mult_big(temp, copy_temp, scale);
    
    delete_big(&copy_temp);
    delete_big(&temp);

    if (b % 2 == 0){
        return  temp1; 
    }else{ 
        if(b > 0){
            temp = mult_big(temp1, a, scale);
            delete_big(&temp1);
            return temp; 
        }else{
            //case 2
            temp = divide_big(temp1 , a, scale);
            delete_big(&temp1);
            return temp; 
        } 
    }
}
/*
    Functions   : sine() , cosine(), tangent()
    Parameters  : Angle in radians of type long double
    Returns     : sine , cosine, tangent of given angle
    Return Type : BIG
    Algorithm   : Value of long double ranges from 4E-4932 to 1.1E+4932
                  So taking input directly as long double
                  and calculating
*/
BIG sine(long double x){
    long double ans;
	ans = sinl(x);
	BIG num;
	num = ftoB(ans);                                // Converts long double to BIG
	return num;

}                
BIG cosine(long double x){
    long double ans;
	ans = cos(x);
	BIG num;
	num = ftoB(ans);                               // Converts long double to BIG
	return num;
}
BIG tangent(long double x){
    long double ans;
	ans = tan(x);
	BIG num;
	num = ftoB(ans);                               // Converts long double to BIG
	return num;
}
/*
    Function    : sqrt_big()
    Parameters  : A numbers of ADT BIG and scale
    Returns     : Square root of number
    Return Type : BIG
    Algorithm   : Xn = number / 2
                  Xn+1 = (1 / 2) * (Xn + number / Xn)   Where Xn is the initial guess of what the square root may be
                  Xn+2 = (1 / 2) * (Xn+1 + number / Xn+1)
                  This is continued until Xr+1 == Xr
                  Because when Xr+1 = Xr = sqrt then :
                        sqrt = (1 / 2) * (sqrt + number / sqrt)
                        sqrt = (number) ** (1 / 2)
    Cases       : 1) If number = 1 then answer = 1
                  2) If number is negative (Invalid case)
*/

BIG sqrt_big(BIG num, int scale){
    BIG one_big;
    init_big(&one_big);                                         // Creating 1 int type BIG
    one_big->digits[1] = 1;
    if(compare_big(num, one_big) == -1){                        // Case 1
        return one_big;
    }delete_big(&one_big);

    // Case 2 
    if(num->sign == 1){
        return NULL;
    }
    BIG two_big, sqrt;
    BIG tolerence;
    init_big(&tolerence);
    tolerence->type = 'f';
    tolerence->fl =(int*) calloc(8, sizeof(int));
    tolerence->fl[0] = 8;
    tolerence->fl[7] = 1;                                       // Tolerence = 0.0000001
    init_big(&two_big);
    two_big->digits[1] = 2;

    // Assuming the sqrt of n as n/2 only 
    BIG guess, copy_num;
    copy_num = clone_big(num); 
    guess = divide_big(copy_num, two_big, scale);
    delete_big(&copy_num);
    
    while(1){
        copy_num = clone_big(num);
        BIG copy_guess = clone_big(guess);
        
        BIG quotient = divide_big(copy_num, copy_guess, scale);
        delete_big(&copy_guess);
        delete_big(&copy_num);
        
        BIG addition = add_big(quotient, guess);
        delete_big(&quotient);
        
        sqrt = divide_big(addition, two_big, scale);
        delete_big(&addition);
        
        BIG subtraction = sub_big(sqrt, guess);
        
        if(subtraction->sign == 1) subtraction->sign = 0;     // Cosidering absolute difference
        
        if(compare_big(subtraction, tolerence) == 0){         // When absolute diff.  of sqrt and guess become less than tolerence 
            delete_big(&two_big);                             // we have found the square root
            delete_big(&subtraction);
            delete_big(&guess);
            break;
        }delete_big(&subtraction);
        delete_big(&guess);
        guess = sqrt;                                         //Updating the value of guess variable by the sqrt variable
    }
    delete_big(&tolerence);
    return sqrt;
}

