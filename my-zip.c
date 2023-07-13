#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    FILE *inputFile;
    char charNow;
    char charCheck;
    int counter = 1;

    if (argc < 2) {
        printf("my-zip: file1 [file2 ...] \n");
        exit(1);
    }
    

    for (int i = 1; i < argc; i++) {
    inputFile = fopen(argv[i], "r");
    if (inputFile == NULL) {
        printf("my-zip: cannot open file \n");
        exit(1);
    }

    charNow = getc(inputFile);
    //I used EOF to check that it isn't the end of the file
    while (charNow != EOF) {
        charCheck = getc(inputFile);

        if (charCheck == charNow) {
            counter = counter + 1;

        } else {
            //stdout writes into the zip file (stdout could be replaced be an output file in which case it would go there)
            fwrite(&counter, sizeof(int), 1, stdout);
            fwrite(&charNow, sizeof(char), 1, stdout);
            counter = 1;
        }

        charNow = charCheck;
    }
    fclose(inputFile);
    }
    return 0;
}