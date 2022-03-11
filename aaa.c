#include <stdio.h>
#include <time.h>

#define PRINT1 0



// show register value in bits
void show_reg(int reg, int n)
{
    int i = 1<<(n-1);
    while(i > 0)
    {
	printf("%d", !!(reg&i));
	i = i>>1;
    }
}

// run register circuit for a polynomial
int shift(int n, int polynomial)
{
    int reg = 1;
    // reg = 2; // 101
    
    int start = reg;
    int position = n-1;

    int h = 1<<n;
    int visited[h];
    for(int i=0; i<h; i++) visited[i] = 0;

    int count = 0;
    while(1)
    {
	if(visited[reg])
	{
	    if(PRINT1)
	    {
		printf("reg = "); show_reg(reg, n);
		printf("  "); printf("repeat\n");
	    }
	    /* if(reg != start) */
	    /* { */
	    /* 	printf("non-start end -"); show_reg(reg, n); printf("  "); */
	    /* } */
	    /* else */
	    /* { */
	    /* 	printf("start-end      "); */
	    /* } */
	    break;   
	}
	visited[reg] = 1; count++;
	if(PRINT1)
	{
	    printf("reg = "); show_reg(reg, n);
	    printf("  ");
	}

	int bit = 0;
	for(int i=0; i<n; i++)
	    bit = bit ^ (!!((reg & polynomial) & (1<<i)));
	if(PRINT1)
	{
	    printf("bit = %d\n", bit);
	}

	reg = reg >> 1;
	reg = reg | (bit << position);
    }
    return count;
}

// loop over all polynomials of degree n
int polynomials(int n)
{
    int U = 1<<n;
    int polynomial;
    int count = 0;
    for(polynomial = 0; polynomial<U; polynomial++)
    {
	int x = shift(n, polynomial);
	if(x == ((1<<n)-1))
	{
	    printf("1");   
	    show_reg(polynomial, n);
	    printf("\n");
	    count++;
	}
	/* if(x == (1<<n)) */
	/* { */
	/*     printf("2^n cycle\n"); */
	/* } */
    }
    return count;
}


int main()
{
    // int ans = shift(4, 3);
    clock_t clock1 = clock();
    for(int i=4; i<=16; i++)
    {
	printf("degree %d:\n", i);
	int ans = polynomials(i);
	printf("count = %d\n", ans);
	printf("\n\n");
    }
    clock1 = clock() - clock1;
    printf("time: %lf seconds\n\n", ((float)clock1)/CLOCKS_PER_SEC);

    // int ans = polynomials(3);
    return 0;
}
/* 57.8 seconds */

