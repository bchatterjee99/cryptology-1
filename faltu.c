#include <stdio.h>



void truncate()
{
    FILE* fp1 = fopen("stream-cipher_output.txt", "r");
    long size = 0;
    while(1)
    {
	int c = fgetc(fp1);
	if(c == EOF) break;

	size++;
    }
    fclose(fp1);

    printf("hoe geche\n");

    fp1 = fopen("streamp-cipher_output.txt", "r");
    FILE* fp2 = fopen("stream-cipher_output_truncated.txt", "w");
    int i = 0;
    while(1)
    {
	int c = fgetc(fp1);
	if(c == EOF) break;
	char ch = fgetc(fp1);
	if(i <= 10000 || i >= size - 10000) 
	    fprintf(fp2, "%c", ch);

	i++;
    }
    fclose(fp1);
    fclose(fp2);
}


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

int main()
{
    truncate();
    return 0;
}
