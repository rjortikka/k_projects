#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>

int main( int argc, char *argv[] ){
    int size_of_buff = 1000;
    FILE *file;
    int i;
    char buffer[size_of_buff];

    if (argc < 2) {
        printf("No files put in \n");
        exit(0);
    }

	for (i = 1; i < argc; i++)
		{
		file = fopen(argv[i], "r");
        if (file == NULL) {
            printf("my-cat: cannot open file \n");
            exit(1);
        }

        while (fgets(buffer, size_of_buff, file) != NULL) {
            printf("%s", buffer);
        }
        printf("\n");
        fclose(file);
		}
    return(0);
}