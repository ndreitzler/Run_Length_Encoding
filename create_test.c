#include <stdio.h>
#include <stdlib.h>

#define SIZE 0xff+0xff+2

int main()
{
    FILE *fptr = fopen("long.ppm", "w");
    char *data = (char *)malloc(SIZE);
    int i;

    for(i = 0; i < SIZE ; i++)
    {
        data[i] = 'A';
    }
    //data[i] = 'B';
    // data[i+1] = 'B';

    fwrite(data,1,SIZE,fptr);
    fclose(fptr);
    free(data);
}