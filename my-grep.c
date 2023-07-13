#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main( int argc, char *argv[] ){
    char *input = NULL;
    FILE *file;
    size_t len = 0;
    size_t input_length;
    int i;

    if (argc < 2) {
        printf("my-grep: searchterm [file ...]\n");
        exit(1);
    }

    //As instructed in the details I put (file = stdin) so I can use it similarly as the code below
    if (argc == 2) {
        file = stdin;
        while ((input_length = getline(&input, &len, file)) != -1) {
            if (strstr(input, argv[1]) != NULL) {
                printf("%s", input);
            }
        }
    } else {

    for (i = 2; i < argc; i++){
		file = fopen(argv[i], "r");
        if (file == NULL) {
            printf("my-grep: cannot open file\n");
            exit(1);
        }

        //This code part goes line by line until it reaches -1, which means the end of the file
        while ((input_length = getline(&input, &len, file)) != -1) {
            //strstr() is a useful function which allows to see if the word is found in the input
            if (strstr(input, argv[1]) != NULL) {
                printf("%s", input); //if the letter combination is found, it is printed out
            }

		}
        fclose(file);

    }
    }

    free(input);
    return 0;
}