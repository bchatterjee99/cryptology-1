#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>


int S[256];
int key[16];
int key_len;


void swap(int *a, int *b)
{
    int tmp = *a;
    *a = *b;
    *b = tmp;
}

void key_gen()
{
    key_len = 16;
    for(int i=0; i<key_len; i++)
	key[i] = rand()%256;
}

void key_scheduling()
{
    for(int i=0; i<256; i++)
	S[i] = i;

    int j = 0;
    for(int i=0; i<256; i++)
    {
	j = (j + S[i] + key[i % key_len]);
	j = j & 0xff;

	swap(&S[i], &S[j]);
    }
    
}

int Z[2];
void pseudo()
{
int I = 0;
int J = 0;
    int k = 2;
    while(k--)
    {
	I = I + 1;     I = I & 0xff;
	J = J + S[I];  J = J & 0xff;
	swap(&S[I], &S[J]);
	Z[1 - k] = S[(S[I] + S[J]) & 0xff];
    }
}

void test1()
{
    int z1_count[256];
    int z2_count[256];
    memset(z1_count, 0, 256*sizeof(int));
    memset(z2_count, 0, 256*sizeof(int));

    int N = 256*100;
    // N = 100;
    for(int i=0; i<N; i++)
    {
	key_gen();
	key_scheduling();
	
	pseudo(); // outputs 2 random bytes in Z[0] and Z[1]

	// printf("Z1 = %d   Z2 = %d\n", Z[0], Z[1]);
	z1_count[Z[0]]++;
	z2_count[Z[1]]++;
    }

    /* for(int i=0; i<256; i++) */
    /* 	printf("S[%d] = %d\n", i, S[i]); */

    for(int i=0; i<256; i++)
	printf("Z1_count[%3d] = %3d    Z2_count[%3d] = %3d\n", i, z1_count[i], i, z2_count[i]);

    FILE* fp1 = fopen("rc4_z1_data.txt", "w");
    FILE* fp2 = fopen("rc4_z2_data.txt", "w");
    for(int i=0; i<256; i++)
    {
	printf("Z1_prob[%3d] = %2.2lf/256    Z2_prob[%3d] = %2.2lf/256\n",
	       i, (float)z1_count[i]/100.0, i, (float )z2_count[i]/100.0);

	fprintf(fp1, "%d %lf\n", i, (float)z1_count[i]/100.0);
	fprintf(fp2, "%d %lf\n", i, (float)z2_count[i]/100.0);
    }
    fclose(fp1);
    fclose(fp2);

}

void test2()
{
    
}

int main()
{
    srand(time(0));
    test1();
    return 0;
}
