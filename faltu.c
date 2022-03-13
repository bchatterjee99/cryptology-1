#include <stdio.h>

void test1()
{
    long x = ((unsigned int)1) << 31;
    long y = 1 << 31;
    long z = ((unsigned int)1) << 32;

    printf("%ld\n", x);
    printf("%ld\n", y);
    printf("%ld\n", z);


    long count = ((unsigned long)1) << 23;
    while(count--)
    {
	printf("%ld\n", count);
    }
}

unsigned long gcd(unsigned long a, unsigned long b)
{
    if(b == 0) return a;
    if(a == 0) return b;
    return gcd(b, a%b);
}

void test2()
{
    int i,j;
    for(i = 0; i<=20; i++)
    {
	for(j=i+1; j<=20; j++)
	{
	    unsigned long x = (((unsigned long) 1)<<i) - 1;
	    unsigned long y = (((unsigned long) 1)<<j) - 1;
	    unsigned long g = gcd(x, y);
	    printf("%d %d: %d\n", x, y, g);
	}
    }
}

int main()
{
    test2();
    return 0;
}
