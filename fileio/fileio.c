#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main()
{
    FILE *fin, *fout;
    fin = fopen("input.txt", "r+");
    fout = fopen("output.txt", "w+");

    int i;
    fscanf(fin, "%d", &i);
    fprintf(fout, "Reading %d from input.txr \n", i);
    printf("Reading %d  from input.txt \n", i);

    fclose(fin);
    fclose(fout);

    return 0;
}
