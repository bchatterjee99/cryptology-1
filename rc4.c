#include <stdio.h>
#include <stdlib.h>
#include <string.h>


int S[256];
int key[16];
int key_len;


void swap(int *a, int *b)
{
    int tmp = *a;
    *a = *b;
    *b = tmp;
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
int I = 0;
int J = 0;
void pseudo()
{
    int k = 2;
    while(k--)
    {
	I = I + 1;     I = I & 0xff;
	J = J + S[I];  J = J & 0xff;
	swap(&S[I], &S[J]);
	Z[k] = S[(S[I] + S[J]) & 0xff];
    }
}

int main()
{
    key_len = 16;
    for(int i=0; i<key_len; i++)
	key[i] = rand()%256;

    key_scheduling();

    int z1_count[256];
    int z2_count[256];
    memset(z1_count, 0, 256*sizeof(int));
    memset(z2_count, 0, 256*sizeof(int));

    int N = 256*100;
    // N = 100;
    for(int i=0; i<N; i++)
    {
	pseudo(); // outputs 2 random bytes in Z[0] and Z[1]

	// printf("Z1 = %d   Z2 = %d\n", Z[0], Z[1]);
	z1_count[Z[0]]++;
	z2_count[Z[1]]++;
    }

    for(int i=0; i<256; i++)
	printf("Z1_count[%3d] = %3d    Z2_count[%3d] = %3d\n", i, z1_count[i], i, z2_count[i]);

    for(int i=0; i<256; i++)
	printf("Z1_prob[%3d] = %2.2lf/256    Z2_prob[%3d] = %2.2lf/256\n",
	       i, (float)z1_count[i]/100.0, i, (float )z2_count[i]/100.0);


    return 0;
}
