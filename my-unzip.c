#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    FILE *inputFile;
    int counter; //Holds the amount of times the character appears
    char character;

    if (argc < 2) {
        printf("my-unzip: file1 [file2 ...] \n");
        exit(1);
    }

    //rb is for reading binary format
    inputFile = fopen(argv[1], "rb");
    if (inputFile == NULL) {
        printf("my-unzip: cannot open file \n");
        exit(1);
    }
    
    //The while loop continues until it reaches the end of the file (fails to read an integer)
    //counter is therefore overridden by with the integer
    while (fread(&counter, sizeof(int), 1, inputFile)) {

        fread(&character, sizeof(char), 1, inputFile); //read repeating character

        for (int a = 0; a < counter; a++) {
            printf("%c", character); //And with this combo the character repeats the right amount
        }
    }

    fclose(inputFile);
    return 0;
}