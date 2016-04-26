
/*
* CS:APP Data Lab
*
* <Jason ZHANG>
*
* bits.c - Source file with your solutions to the Lab.
*          This is the file you will hand in to your instructor.
*
* WARNING: Do not include the <stdio.h> header; it confuses the dlc
* compiler. You can still use printf for debugging without including
* <stdio.h>, although you might get a compiler warning. In general,
* it's not good practice to ignore compiler warnings, but in this
* case it's OK.
*/

#if 0
/*
* Instructions to Students:
*
* STEP 1: Read the following instructions carefully.
*/

You will provide your solution to the Data Lab by
editing the collection of functions in this source file.

INTEGER CODING RULES :

Replace the "return" statement in each function with one
or more lines of C code that implements the function.Your code
must conform to the following style :

int Funct(arg1, arg2, ...) {
	/* brief description of how your implementation works */
	int var1 = Expr1;
	...
		int varM = ExprM;

	varJ = ExprJ;
	...
		varN = ExprN;
	return ExprR;
}

Each "Expr" is an expression using ONLY the following :
1. Integer constants 0 through 255 (0xFF), inclusive.You are
not allowed to use big constants such as 0xffffffff.
2. Function arguments and local variables(no global variables).
3. Unary integer operations !~
4. Binary integer operations & ^ | +<< >>

Some of the problems restrict the set of allowed operators even further.
Each "Expr" may consist of multiple operators.You are not restricted to
one operator per line.

You are expressly forbidden to :
1. Use any control constructs such as if, do, while, for, switch, etc.
2. Define or use any macros.
3. Define any additional functions in this file.
4. Call any functions.
5. Use any other operations, such as &&, || , -, or ? :
	6. Use any form of casting.
	7. Use any data type other than int.This implies that you
	cannot use arrays, structs, or unions.


	You may assume that your machine :
1. Uses 2s complement, 32 - bit representations of integers.
2. Performs right shifts arithmetically.
3. Has unpredictable behavior when shifting an integer by more
than the word size.

EXAMPLES OF ACCEPTABLE CODING STYLE :
/*
* pow2plus1 - returns 2^x + 1, where 0 <= x <= 31
*/
int pow2plus1(int x) {
	/* exploit ability of shifts to compute powers of 2 */
	return (1 << x) + 1;
}

/*
* pow2plus4 - returns 2^x + 4, where 0 <= x <= 31
*/
int pow2plus4(int x) {
	/* exploit ability of shifts to compute powers of 2 */
	int result = (1 << x);
	result += 4;
	return result;
}

FLOATING POINT CODING RULES

For the problems that require you to implent floating - point operations,
the coding rules are less strict.You are allowed to use looping and
conditional control.You are allowed to use both ints and unsigneds.
You can use arbitrary integer and unsigned constants.

You are expressly forbidden to :
1. Define or use any macros.
2. Define any additional functions in this file.
3. Call any functions.
4. Use any form of casting.
5. Use any data type other than int or unsigned.This means that you
cannot use arrays, structs, or unions.
6. Use any floating point data types, operations, or constants.


NOTES:
1. Use the dlc(data lab checker) compiler(described in the handout) to
check the legality of your solutions.
2. Each function has a maximum number of operators(!~& ^ | +<< >> )
that you are allowed to use for your implementation of the function.
The max operator count is checked by dlc.Note that '=' is not
counted; you may use as many of these as you want without penalty.
3. Use the btest test harness to check your functions for correctness.
4. Use the BDD checker to formally verify your functions
5. The maximum number of ops for each function is given in the
header comment for each function.If there are any inconsistencies
between the maximum ops in the writeup and in this file, consider
this file the authoritative source.

/*
* STEP 2: Modify the following functions according the coding rules.
*
*   IMPORTANT. TO AVOID GRADING SURPRISES:
*   1. Use the dlc compiler to check that your solutions conform
*      to the coding rules.
*   2. Use the BDD checker to formally verify that your solutions produce
*      the correct answers.
*/


#endif
/*
* bitAnd - x&y using only ~ and |
*   Example: bitAnd(6, 5) = 4
*   Legal ops: ~ |
*   Max ops: 8
*   Rating: 1
*/
int bitAnd(int x, int y) {
	return ~((~x) | (~y));
}
// Tip: DeMorgan's Law

/*
* getByte - Extract byte n from word x
*   Bytes numbered from 0 (LSB) to 3 (MSB)
*   Examples: getByte(0x12345678,1) = 0x56
*   Legal ops: ! ~ & ^ | + << >>
*   Max ops: 6
*   Rating: 2
*/
int getByte(int x, int n) {
	return (x >> (n << 3)) & 0x000000ff;
}
// Tip: a byte has 8 bits.
// Note that little-endian or big-endian does not affect the result although bytes are ordered differently.
// The compiler has already handled this difference. 
// See that C is a high-level language, which means good transportability.

/*
* logicalShift - shift x to the right by n, using a logical shift
*   Can assume that 0 <= n <= 31
*   Examples: logicalShift(0x87654321,4) = 0x08765432
*   Legal ops: ! ~ & ^ | + << >>
*   Max ops: 20
*   Rating: 3
*/
int logicalShift(int x, int n) {
	int key = (~(1 << 31)) >> n;
	key = (key << 1) + 1;
	return (x >> n)&key;
}
// "key" only maintains the bits which are not brought by shifting

/*
* bitCount - returns count of number of 1's in word
*   Examples: bitCount(5) = 2, bitCount(7) = 3
*   Legal ops: ! ~ & ^ | + << >>
*   Max ops: 40
*   Rating: 4
*/
int bitCount(int x) {
	int x0, x1, x2, x3;
	int mask;
	mask = 0x11;
	mask |= mask << 8;
	mask |= mask << 16;
	x0 = x&mask;
	x1 = (x >> 1)&mask;
	x2 = (x >> 2)&mask;
	x3 = (x >> 3)&mask;
	x = x0 + x1 + x2 + x3;
	x = x + (x >> 16);
	return (x & 0x0f) + ((x >> 4) & 0x0f) + ((x >> 8) & 0x0f) + ((x >> 12) & 0x0f);
}
// regard every 4 bits as a memorizer of how many 1's are there in this 4 bits,
// add them up into 2 bytes, and retrieve the 4 numbers in this 2 bytes.
// the sum of these 4 numbers is the answer.
// The reason why stopping at 2 bytes is that once added into 1 single byte, 
// every 4 bits is potentially memorizing how many 1's are there in 16 bits,
// which has overranged 4 bits' capacity.

/*
* bang - Compute !x without using !
*   Examples: bang(3) = 0, bang(0) = 1
*   Legal ops: ~ & ^ | + << >>
*   Max ops: 12
*   Rating: 4
*/
int bang(int x) {
	x = (x >> 16) | x;
	x = (x >> 8) | x;
	x = (x >> 4) | x;
	x = (x >> 2) | x;
	x = (x >> 1) | x;
	return ~x & 0x0000001;
}
// find the conjunction among every bit of x by bisecting x step-by-step
// if the final result is 1, it means there exists 1 in x, thus return 0; otherwise return 1

/*
* tmin - return minimum two's complement integer
*   Legal ops: ! ~ & ^ | + << >>
*   Max ops: 4
*   Rating: 1
*/
int tmin(void) {
	return 1 << 31;
}
// Tip: Tmin equals 0x80000000

/*
* fitsBits - return 1 if x can be represented as an
*  n-bit, two's complement integer.
*   1 <= n <= 32
*   Examples: fitsBits(5,3) = 0, fitsBits(-4,3) = 1
*   Legal ops: ! ~ & ^ | + << >>
*   Max ops: 15
*   Rating: 2
*/
int fitsBits(int x, int n) {
	int shift = ~n + 1;
	int key = (x << shift) >> shift;
	return !(x^key);
}
// Tip: if x is a n-bit 2's-c int, by expanding it to 32 bits, the left handside of n-th bit should all equal n-th bit.
// The trick here is that when shifting a int32 number (x) by a negative value (y), 
// the value of bits shifted is actually equal to the lower 5 bits of y. (the shifting direction is the same)
// For example, when calculating 1<<(-1), -1 is interpreted as 0xffffffff, 
// then the actual value is 1<<(1111111)'b = 1<<31. (Counter-intuitively it is NOT 1>>1 !!)
// Thus when calculating x<<(-n), we get x<<(32-n). 
// By shifting x back and forth, "key" only maintains the bits not cancelled out at its original position in x,
// and meanwhile if the most siginicant bit in x is 1, its left handside will be filled with 1 (othwrwise 0).
// Compare the result with x, if they are the same, it means x can be represented as a n-bit 2's-c integer. 

/*
* divpwr2 - Compute x/(2^n), for 0 <= n <= 30
*  Round toward zero
*   Examples: divpwr2(15,1) = 7, divpwr2(-33,4) = -2
*   Legal ops: ! ~ & ^ | + << >>
*   Max ops: 15
*   Rating: 2
*/
int divpwr2(int x, int n) {
	int sign = x >> 31;
	int mask = (1 << n) + (~0);
	int bias = sign&mask;
	return (x + bias) >> n;
}
// Tip: The difference between round-to-zero and round-to-negative emerges ONLY when:
//	    the number is negative and its fractional part is not 0.
// e.g. -1.001 = -1 (round-to-zero)  -1.001 = -2 (round-to-negative)
// Simply right-shift a negative number is equivalent to round-to-negative.
// Thus for negative numbers it is necessary to first check whether the bits to be rounded are all 0.
// If it is not all 0, add 1 to the least significant bit.
// The method is to simply add them by a bias number of n bits of 1's.
// For non-negative numbers the bias number should be 0.

/*
* negate - return -x
*   Example: negate(1) = -1.
*   Legal ops: ! ~ & ^ | + << >>
*   Max ops: 5
*   Rating: 2
*/
int negate(int x) {
	return ~x + 1;
}
// Tip: 2's-c int

/*
* isPositive - return 1 if x > 0, return 0 otherwise
*   Example: isPositive(-1) = 0.
*   Legal ops: ! ~ & ^ | + << >>
*   Max ops: 8
*   Rating: 3
*/
int isPositive(int x) {
	return (!(x >> 31))&(!!(x ^ 0));
}
// MSB = 0   -> x is positive or zero
// !!(x^0)=1 -> x is not zero

/*
* isLessOrEqual - if x <= y  then return 1, else return 0
*   Example: isLessOrEqual(4,5) = 1.
*   Legal ops: ! ~ & ^ | + << >>
*   Max ops: 24
*   Rating: 3
*/
int isLessOrEqual(int x, int y) {
	int signx = (x >> 31) & 1;
	int signy = (y >> 31) & 1;
	int sign = signx&(~signy); 
	int dif = x + (~y + 1);      
	dif = ((dif >> 31) & 1)&(!(signx^signy)); 						  
	return sign | dif | (!(x^y));
}
// three cases in which this function should return 1: "(x-y<0 && signx==signy)", "(signx=1 && signy=0)", "(x==y)"
// "sign" is 1 if and only if "(signx=1 && signy==0)", in this case "x" is definitely smaller than "y".
// the sign bit of "dif" should be 1 if "(x<y)", but it could fail when "(x<0&&y>0)" or "(x>0&&y<0)" by overflow.
// "dif" is 1 if and only if "(x-y<0&&signx==signy)"
// !(x^y) judges whether x==y

/*
* ilog2 - return floor(log base 2 of x), where x > 0
*   Example: ilog2(16) = 4
*   Legal ops: ! ~ & ^ | + << >>
*   Max ops: 90
*   Rating: 4
*/
int ilog2(int x) {
	int shift16, shift8, shift4, shift2, shift1;
	shift16 = !!(x >> 16);
	shift16 = shift16 << 4;
	x = x >> shift16;

	shift8 = !!(x >> 8);
	shift8 = shift8 << 3;
	x = x >> shift8;

	shift4 = !!(x >> 4);
	shift4 = shift4 << 2;
	x = x >> shift4;

	shift2 = !!(x >> 2);
	shift2 = shift2 << 1;
	x = x >> shift2;

	shift1 = !!(x >> 1);

	return shift1 + shift2 + shift4 + shift8 + shift16;
}
// "shift16" marks whether there is 1 in the first 16 bits. similar for "shiftxx".
// if there is, right-shift x by the bits it recorded to cancel out the low bits,
// until the precision is down to 1.

/*
* float_neg - Return bit-level equivalent of expression -f for
*   floating point argument f.
*   Both the argument and result are passed as unsigned int's, but
*   they are to be interpreted as the bit-level representations of
*   single-precision floating point values.
*   When argument is NaN, return argument.
*   Legal ops: Any integer/unsigned operations incl. ||, &&. also if, while
*   Max ops: 10
*   Rating: 2
*/
unsigned float_neg(unsigned uf) {
	int exp = !((uf & 0x7f800000) ^ 0x7f800000);
	int frac = uf & 0x007fffff;
	if (frac&&exp) return uf;
	else return uf ^ 0x80000000;
}
// Tip: IEEE754 standard of single precision floating point value.

/*
* float_i2f - Return bit-level equivalent of expression (float) x
*   Result is returned as unsigned int, but
*   it is to be interpreted as the bit-level representation of a
*   single-precision floating point values.
*   Legal ops: Any integer/unsigned operations incl. ||, &&. also if, while
*   Max ops: 30
*   Rating: 4
*/
unsigned float_i2f(int x) {
	int sign = x & 0x80000000, MSB = 0, flag = 0;
	int roundbits, fracbit, LSB;
	if (x == 0) return x;
	if (sign) x = -x;
	while (!((x << MSB) & 0x80000000)) MSB++;
	if (MSB >= 8) x = x << (MSB - 8);
	else 
	{
		fracbit = 1 << (7 - MSB);
		LSB = fracbit << 1;
		roundbits = (LSB - 1)&x;
		if (roundbits == fracbit)
		{
			if (x&LSB) flag = 1;
		}
		else if (roundbits>fracbit) flag = 1;
		if (flag) x += LSB;
		if ((x << (MSB - 1)) & 0x80000000) MSB--;
		x = x >> (8 - MSB);
	}
	x = x & 0x007fffff;
	x = x | ((158 - MSB) << 23);
	x = x | sign;
	return x;
}
// Tip: When there is lost precision, round to nearest even.
// Round-to-even is the default method because it guarantees statistically the rounding does not 
// cause any bias among sample values.
// Round-to-even is ONLY different from half-round when the integer part is even and the fractional part is 0.5
// e.g. 2.5 = 2 (round to even) 2.5 = 3 (half round) 1.5 = 2 (both round to even and half round)
// In terms of binary representation, first check whether the bits to be rounded is equal to 1000..
// If true, check whether the least significant bit is 1, if also true, add it by 1
// Or if "roundbits" is simply larger than 1000..., also we should add LSB by 1

/*
* float_twice - Return bit-level equivalent of expression 2*f for
*   floating point argument f.
*   Both the argument and result are passed as unsigned int's, but
*   they are to be interpreted as the bit-level representation of
*   single-precision floating point values.
*   When argument is NaN, return argument
*   Legal ops: Any integer/unsigned operations incl. ||, &&. also if, while
*   Max ops: 30
*   Rating: 4
*/
unsigned float_twice(unsigned uf) {
	int sign = uf & 0x80000000;
	int frac = uf & 0x007fffff;
	int exp = uf & 0x7f800000;
	if (!(exp ^ 0x7f800000)) return uf;
	if (!exp)
	{
		frac = frac << 1;
		uf = sign | frac;
		return uf;
	}
	else
	{
		uf = uf + 0x00800000;
		return uf;
	}
}
// Tip: IEEE754 for NaN: exp is all 1 but frac is not 0


