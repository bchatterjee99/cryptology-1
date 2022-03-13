#include <stdio.h>
#include <stdlib.h>
#include <string.h>


/* #define L1 7 */
/* #define L2 11 */
/* #define L3 13 */
/* #define POLY1 0b0000011       // 7 degree polynomial *\/ */
/* #define POLY2 0b00000000101   // 11 degree polynomial *\/ */
/* #define POLY3 0b0000000011011 // 13 degree polynomial *\/ */
/* #define KEY1 0x5   //           _000 0101 */
/* #define KEY2 0x89  //      _000 1000 1001 */
/* #define KEY3 0x358 // ___0 0011 0101 1000 */

/* #define L1 5 */
/* #define L2 7 */
/* #define L3 11 */
/* #define POLY1  0b00101       // 5 degree polynomial */
/* #define POLY2  0b0000011     // 7 degree polynomial */
/* #define POLY3  0b00000000101 // 11 degree polynomial */
/* #define KEY1 0x5   //      ___0 0101 */
/* #define KEY2 0x49  //      _100 1001 */
/* #define KEY3 0x358 // _011 0101 1000 */


#define L1 4
#define L2 6
#define L3 8
#define POLY1 0b0011
#define POLY2 0b000011
#define POLY3 0b00011101
#define KEY1 0x5 
#define KEY2 0x5
#define KEY3 0x5


// maximum message length in bits
// #define MAX_MESG_LEN 8059039 // key-stream-period
#define MAX_MESG_LEN 1000

#define PRINT1 1
#define FRONT 10000
#define BACK 380000

unsigned char *visited; // use every bit for marking states
/* 
   the visited array has to have 2^(L1+L2+L3) bits
   the state is calculated as [reg1 << (L2 + L3) | reg2 << (L3) | reg3 ]
   hightest state is [2^(L1 + L2 + L3) - 1]
   some might be unused as zero register states are not used
   mainly needed for non primitive polynomial
*/
unsigned long v_len;
// check pos'th  bit in the visited array
int check_visited(unsigned long pos)
{
    unsigned long x = pos/8;
    unsigned long y = pos%8;
    int ans = !!(visited[x]&(1<<y));
    return ans;
}
// set pos'th  bit in the visited array
void set_visited(unsigned long pos)
{
    unsigned long x = pos/8;
    unsigned long y = pos%8;
    visited[x] = visited[x]|(1<<y);
}


/* 

     +--------------+---+ 
     |              |   |  
     |              v   v  
     |    +---+---+---+---+
     +--->|   |   |   |   |
          +---+---+---+---+
 
run lfsr circuit(defined by poly) 1 step on reg 
and return output bit */
int lfsr(int n, int* reg, int poly)
{
    int out = (*reg) & 1;

    int bit = 0; 
    int temp = (*reg) & poly;
    for(int i = 0; i<n; i++)
	bit = bit^(!!(temp & (1<<i)));
    (*reg) = (*reg) >> 1;
    (*reg) = (*reg) | (bit<<(n-1));

    return out;
}

// show register value in bits
void show_reg(int reg, int n)
{
    int i = 1<<(n-1);
    while(i > 0)
    {
	printf("%d", !!(reg&i));
	i = i>>1;
    }
    /* !!(number) changes non-zero to 1 (keeps 0, 0) */
}

int f(int x1, int x2, int x3)
{
    int out = ((!x3) & x1) | (x3 & x2);
    return out;
}

// generate key until some (combined)register-state is repeated
int key_stream()
{
    int reg1, poly1; // L1
    int reg2, poly2; // L2
    int reg3, poly3; // L3

    // secret key
    reg1 = KEY1;
    reg2 = KEY2;
    reg3 = KEY3;

    // polynomials
    poly1 = POLY1;
    poly2 = POLY2;
    poly3 = POLY3;

    // maximum number of distinct register states
    unsigned long max_itr = ((unsigned long)1<<L1) - 1;
    max_itr = max_itr * ((((unsigned long) 1)<<L2) - 1 );
    max_itr = max_itr * ((((unsigned long) 1)<<L3) - 1 );
    unsigned long itr = 0;

    // run until some registers state is repeated
    int flag = 0;
    while(itr <= max_itr)
    {
	// state = reg1 reg2 reg3
	unsigned long state = 0;
	state = ((unsigned long)reg1)<<(L2+L3);
	state = state | ( ((unsigned long)reg2)<<L3 );
	state = state | ( (unsigned long)reg3 );

	// set flag if registers-state already seen
	if(check_visited(state)) 
	    flag = 1;
	set_visited(state); // mark state

	if(PRINT1 && (itr <= FRONT || itr >= max_itr - BACK))
	{
	    printf("reg1: "); show_reg(reg1, L1); printf("\n");
	    printf("reg2: "); show_reg(reg2, L2); printf("\n");
	    printf("reg3: "); show_reg(reg3, L3); printf("\n");
	}
	int x1 = lfsr(L1, &reg1, poly1);
	int x2 = lfsr(L2, &reg2, poly2);
	int x3 = lfsr(L3, &reg3, poly3);
	int out = f(x1, x2, x3);

	if(PRINT1 && (itr <= FRONT || itr >= max_itr - BACK))
	{
	    printf("output: %d\n", out);
	    printf("itr: %d/%d\n", itr, max_itr);
	}
	itr++;

	if(flag)
	{
	    if(PRINT1 && (itr <= FRONT || itr >= max_itr - BACK))
	    {
		printf("(repeat)\n\n");
	    }
	    break;
	}
	else
	{
	    if(PRINT1 && (itr <= FRONT || itr >= max_itr - BACK))
	    {
		printf("\n");
	    }
	}
    }
    return 0;
}

int key_string[MAX_MESG_LEN];
int plain_text[MAX_MESG_LEN];
int cipher_text[MAX_MESG_LEN];
void encrypt(char* message, unsigned long len)
{
    len = 8*len; // need no. of bits in message

    int reg1, poly1; // L1
    int reg2, poly2; // L2
    int reg3, poly3; // L3
    
    // secret key
    reg1 = KEY1;
    reg2 = KEY2;
    reg3 = KEY3;

    // polynomials
    poly1 = POLY1;
    poly2 = POLY2;
    poly3 = POLY3;

    // maximum number of distinct register states
    unsigned long max_itr = ((unsigned long)1<<L1) - 1;
    max_itr = max_itr * ((((unsigned long) 1)<<L2) - 1 );
    max_itr = max_itr * ((((unsigned long) 1)<<L3) - 1 );
    unsigned long itr = 0;

    // run until some registers state is repeated
    int flag = 0; unsigned long i = 0;
    while(itr <= max_itr && i < len)
    {
	// state = reg1 reg2 reg3
	unsigned long state = 0;
	state = ((unsigned long)reg1)<<(L2+L3);
	state = state | ( ((unsigned long)reg2)<<L3 );
	state = state | ( (unsigned long)reg3 );

	// set flag if registers state already seen
	if(check_visited(state)) 
	    flag = 1;
	set_visited(state); // mark state

	/* if(PRINT1) */
	/* { */
	/*     printf("reg1: "); show_reg(reg1, L1); printf("\n"); */
	/*     printf("reg2: "); show_reg(reg2, L2); printf("\n"); */
	/*     printf("reg3: "); show_reg(reg3, L3); printf("\n"); */
	/* } */
	int x1 = lfsr(L1, &reg1, poly1);
	int x2 = lfsr(L2, &reg2, poly2);
	int x3 = lfsr(L3, &reg3, poly3);
	int key_bit = f(x1, x2, x3);

	/* printf("output: %d\n", key_bit); */
	/* printf("itr: %d/%d\n", itr, max_itr); */
	itr++;

	// pad message
	int curr_bit = !!(message[i/8] & (1<< (7 - (i%8)) ));
        int out_bit = curr_bit ^ key_bit;
	if(i < MAX_MESG_LEN)
	{
	    plain_text[i] = curr_bit;
	    key_string[i] = key_bit;
	    cipher_text[i] = out_bit;
	}
	/* printf("%d", out_bit); */
	/* if(i%4 == 3) printf(" "); // half byte */
	i++;

	if(flag)
	{
	    // printf("(repeat)\n\n");
	    if(i < len)
		printf("message longer than key stream\n");
	    break;
	}
    }
    // printf("\n\n");
}


void test1()
{
    key_stream();
}

void test2()
{
    char* message = "ABC"; // 0100 0001 0100 0010 0100 0011
    int len = 3;

    if(len > MAX_MESG_LEN)
    {
	printf("message length > MAX_MESG_LEN(%ld)\n\n", MAX_MESG_LEN);
	return;
    }

    encrypt(message, len);
    printf("message:    ");
    for(int i=0; i<len; i++)
	printf("         %c", message[i]);
    printf("\n");
    printf("plain-text:  ");
    for(int i=0; i<len*8; i++)
    {
	printf("%d", plain_text[i]);
	if(i%4 == 3) printf(" ");
    }
    printf("\n");
    printf("key-stream:  ");
    for(int i=0; i<len*8; i++)
    {
	printf("%d", key_string[i]);
	if(i%4 == 3) printf(" ");
    }
    printf("\n");
    printf("cipher-text: ");
    for(int i=0; i<len*8; i++)
    {
	printf("%d", cipher_text[i]);
	if(i%4 == 3) printf(" ");
    }
    printf("\n\n");
}

int main()
{
    // visited array on heap of size (no. of states)/8 [use every bit for marking]
    visited=(unsigned char*)malloc( ((unsigned long)1)<<(L1+L2+L3-3) );
    v_len = ((unsigned long)1)<<(L1+L2+L3-3);
    memset(visited, 0, v_len);
    unsigned long num_register_states = ((unsigned long)1<<L1) - 1;
    num_register_states = num_register_states * ((((unsigned long) 1)<<L2) - 1 );
    num_register_states = num_register_states * ((((unsigned long) 1)<<L3) - 1 );

    printf("No of distinct non-zero (combined)register states: %lu\n\n", num_register_states);
    test1();

    // test2();
    
    free(visited);
    return 0;
}
