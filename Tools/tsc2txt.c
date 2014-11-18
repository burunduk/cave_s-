#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXFILESIZE 65534
char base[MAXFILESIZE];

unsigned int readfile(char* infile)
{
    unsigned int i = 0, rc = 0 ;
    FILE* fin = fopen(infile, "rb");
        if(fin == NULL) {
        printf("Error open file to read: %s\n", infile);
        return 0;
    }
    for (i = 0; (rc = getc(fin)) != EOF; )
    {
        if ( i> MAXFILESIZE )
        {
            printf("Error read file: %s.\n", infile);
            return 0;
        }
        base[i++] = rc;
    }
    fclose(fin);
    return i;
}
unsigned int writefile(char *outfile, unsigned int count)
{
    if (count == 0 && count > MAXFILESIZE) return 0;
    unsigned int i = 0, rc = 0;
    FILE* fout = fopen(outfile, "wb");
            if(fout == NULL) {
        printf("Error open file to write: %s.\n", outfile);
        return 0;
        }
    for (i = 0; i < count; rc = putc(base[i++], fout));
    fclose(fout);
    return i;
}

unsigned int convert_tsc2txt(unsigned int count, unsigned int key)
{
    if (count == 0 ) return 0;
    unsigned int i = 0, rc = 0;
    for (i = 0; i < count; i++)
    {
        rc = base[i];
        base[i] = (( 256 + rc - key) & 0xFF );
    }
    return i;
}

int main(int argc, char *argv[])
{
    if (argc < 2) {
        printf("usage: tsc2txt.exe file.tsc\n");
        return 0;
    }
    unsigned int n = 0;
    char *inputfile = argv[1];
    unsigned int cin = readfile(inputfile);
    if (cin)
    {
    unsigned int keyposition = (( cin & 0xFFFFFFE ) / 2 );
    unsigned int key  = base[keyposition];
        convert_tsc2txt(cin, key);
        base[keyposition] = key;
    while ( (inputfile[n] != '.')  &&  (inputfile[n] != '\0') )
        n ++;
    if ( inputfile[n] == '.' )
        strcpy ( inputfile + n, ".txt" );
    writefile(inputfile, cin);
    } else printf("Error loading file: %s.\n", inputfile);
    return 0;
}
