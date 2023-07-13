#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//Basic structure of the linked list
typedef struct Node {
    char *info;
    struct Node *next;

} Node;

//Creation of new node in linked list
Node* nodeCreation(char *info) {
    Node* result = (Node*)malloc(sizeof(Node));
    if (result == NULL) {
        fprintf(stderr, "malloc failed\n");
        exit(1);
    }

    result->info = strdup(info); //copies the information from the line and puts it into the node (had to use strdup because of getline buffer)
    if (result->info == NULL) {
        fprintf(stderr, "malloc failed\n");
        exit(1);
    }

    result->next = NULL;
    return result;
}

//Frees the memory from the linked list
void freeMemoryList(Node *result) {

    while (result != NULL) {
        Node *temp = result;
        result = result->next;
        free(temp->info); //frees memory of string
        free(temp); //frees memory of the node structure
    }
}

int main(int argc, char *argv[]) {
    //Make sure the correct number of arguments have been supplied
    if (argc != 3) {
        fprintf(stderr, "usage: reverse <input> <output>\n");
        exit(1);
    }

    if (argv[1] == argv[2]) {
        fprintf(stderr, "Input and output file must differ\n");
        exit(1);
    }

    //Open the input file
    FILE *inputFile = fopen(argv[1], "r");
    if (inputFile == NULL) {
        fprintf(stderr, "error: cannot open file '%s'\n", argv[1]);
        exit(1);
    }

    Node *ptrhead = NULL;
    size_t bufferSize = 0; //used size_t in case of large size
    char *line = NULL;

    printf("Here are the lines in the input file: \n");

    //Using getline we read every line in the file and create the linked list
    while (getline(&line, &bufferSize, inputFile) != -1) {
        printf("%s", line); //prints input file lines

        //Had the problem of having extra spaces in the outputFile (except the first line) so this fixes it
        if (line[strlen(line) - 1] == '\n') {
            line[strlen(line) - 1] = '\0';
        }

        Node *node = nodeCreation(line);
        node->next = ptrhead;
        ptrhead = node;
    }
    printf("\n");

    fclose(inputFile);
    free(line); //frees the line buffer

    //Open the output file
    FILE *outputFile = fopen(argv[2], "w");
    if (outputFile == NULL) {
        fprintf(stderr, "error: cannot open file '%s'\n", argv[2]);
        freeMemoryList(ptrhead);
        exit(1);
    }

    printf("Here are the lines in the output file: \n");

    //Writing the linked list into the output file
    Node *placement = ptrhead;
    while (placement != NULL) {
        fputs(placement->info, outputFile); //puts line to output file
        printf("%s \n", placement->info); //prints output file lines

        //This makes sure that every line from the input file actually stays on their own line by adding \n if it's missing
        if (placement->info[strlen(placement->info) - 1] != '\n' || placement->next != NULL) {
            fputc('\n', outputFile);
        }

        placement = placement->next;
    }

    fclose(outputFile);
    freeMemoryList(ptrhead);

    return 0;
}