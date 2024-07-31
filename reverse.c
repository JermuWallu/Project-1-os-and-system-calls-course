#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// for some reason intellisense cries about ssize_t without this
#include <sys/types.h>

// Max size for file name
#define MAX 100

// struct for linked list
typedef struct fileline
{
    char *rivi;
    struct fileline *pNext;
} fileLine;

// This funktion is made to add a character at the end of a string
char *append(const char *orig, char c)
{
    size_t sz = strlen(orig);
    char *str = malloc(sz + 2);
    strcpy(str, orig);
    str[sz] = c;
    str[sz + 1] = '\0';
    return str;
}

// read a file and save it contents on the linked list
// 
// It doesen't assume the row's or file's length.
fileLine *readFile(fileLine *list, char name[]) {

    FILE *file;
    fileLine *pStart = NULL, *pEnd = NULL, *pNew = NULL;
    char *line = NULL;
    size_t len = 0;
    ssize_t nread;

    // File opening and its error handling
    if ((file = fopen(name, "r")) == NULL) {
        fprintf(stderr, "Unable to open the file, exiting..\n");
        exit(1);
    }
    
    printf("Reading the file '%s'...\n", name); 
    
    // reading the file
    while ((nread = getline(&line, &len, file)) != -1) {
        if ( (pNew = (fileLine*)malloc(sizeof(fileLine))) == NULL) {
            fprintf(stderr, "malloc failed\n");
            exit(1);
        }
        printf("%s",line);
        pNew->rivi = line;

        pNew->pNext = NULL;
        if (pStart == NULL) {
            pStart = pNew;
            pEnd = pNew;
            // the line below makes so that the output file doesen't have a extra line due to newline character
            // pStart->rivi[strlen(pStart->rivi)-1] = '\0'; 
        } else {
            pEnd->pNext = pNew;
            pEnd = pNew;
        }
        line = NULL;
    }
    // making the last line have a newline 
    pEnd->rivi = append(pEnd->rivi, '\n');

    fclose(file);
    return pStart;
}

// Used to write file based on the data in linked list
void writeFile(fileLine *dList, char name[]) {
    FILE *file;

    // File opening and its error handling
    if ((file = fopen(name, "w")) == NULL) {
        fprintf(stderr, "Unable to open the file, exiting..\n");
        exit(0);
    }
    
    printf("\nStaring to write on file '%s'...\n", name); 
    
    fileLine *pNext = dList;
    // loop through the linked list and write file
    while (pNext != NULL) {
        fprintf(file, "%s",pNext->rivi);
        printf("%s", pNext->rivi);
        pNext = pNext->pNext;
    }
    
    fclose(file);
    return;
}

// Frees the memory of the linked list
void freeMemory(fileLine *list) {
    
    // error handling
    if (list == NULL) {
        return;
    }

    // loop through the linked list and free its memory
    fileLine *ptr = list;
    while (ptr->pNext != NULL)
    {
        list = ptr->pNext;
        // free(ptr->rivi);
        free(ptr);
        ptr = list;
    }

    return;
}


/* 
this funktion is used to actually reverse the file.

This is achieved by looping the file from top nad botton and switching their contents till they meet. 
If the input file has odd amount of rows, then the loop doesen't touch the middle one and stops.
*/
void reverseFile(fileLine *pFile) {
    fileLine *pStart = pFile, *pEnd = pFile, *pNext = NULL;
    char *string = NULL;

    // find the last item on list
    while (pEnd->pNext != NULL)
    {
        pEnd = pEnd->pNext;
    }
    
    // the loop
    while (pStart != pEnd) { // this condition breaks odd files
        // switch contents
        string = pStart->rivi;
        pStart->rivi = pEnd->rivi;
        pEnd->rivi = string;


        // this breaks even files
        if(pStart->pNext == pEnd) {
            break;
        }

        // loop pNext to find previous pointer for pLast
        pNext = pStart;
        while (pNext->pNext != pEnd)
        {
            pNext = pNext->pNext;
        }
        pEnd = pNext;
        pStart = pStart->pNext;
    }
    return;
}

int main(int argc, char *argv[])
{
    fileLine *pFile = NULL;
    // Arg error handling 
    if (argc != 3) {
        fprintf(stderr, "usage: reverse <input> <output>\n");
        exit(1);
    }

    // "if same file name"-error handling
    if (strcmp(argv[1], argv[2]) == 0) {
        fprintf(stderr, "Input and output file must differ\n");
        exit(1);
    }
    
    pFile = readFile(pFile, argv[1]);
    reverseFile(pFile);
    writeFile(pFile, argv[2]);

    freeMemory(pFile);
    exit(0);
}
