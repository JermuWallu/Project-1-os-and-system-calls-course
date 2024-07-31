#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// for some reason intellisense cries about ssize_t without this
#include <sys/types.h>

// Max size for file name
#define MAX 100

typedef struct fileline
{
    char *rivi; // TODO: use linked list
    struct fileline *pNext;
} fileLine;

char *append(const char *orig, char c)
{
    size_t sz = strlen(orig);
    char *str = malloc(sz + 2);
    strcpy(str, orig);
    str[sz] = c;
    str[sz + 1] = '\0';
    return str;
}

fileLine *lueTiedosto(fileLine *lista, char nimi[]) {

    FILE *tiedosto;
    fileLine *pAlku = NULL, *pLoppu = NULL, *pUusi = NULL;
    char *line = NULL;
    size_t len = 0;
    ssize_t nread;

    // Tiedoston avaus ja virheenkäsittely
    if ((tiedosto = fopen(nimi, "r")) == NULL) {
        fprintf(stderr, "Tiedoston avaaminen epäonnistui, lopetetaan\n");
        exit(1);
    }
    
    // DEBUG
    printf("Reading the file '%s'...\n", nimi); 
    
    // Tiedoston luku
    while ((nread = getline(&line, &len, tiedosto)) != -1) {
        if ( (pUusi = (fileLine*)malloc(sizeof(fileLine))) == NULL) {
            fprintf(stderr, "malloc failed\n");
            exit(1);
        }
        printf("%s",line);
        // strcpy(pUusi->rivi, line);
        pUusi->rivi = line;
        pUusi->pNext = NULL;

        if (pAlku == NULL) {
            pAlku = pUusi;
            pLoppu = pUusi;
            // pAlku->rivi[strlen(pAlku->rivi)-1] = '\0'; 
        } else {
            pLoppu->pNext = pUusi;
            pLoppu = pUusi;
        }
        // printf("listassa: %s\n", pUusi->rivi);
        line = NULL;
    }
    // making the last line have a newline 
    pLoppu->rivi = append(pLoppu->rivi, '\n');
    // free(line);

    fclose(tiedosto);
    
    return pAlku;
}

void kirjoitaTiedosto(fileLine *dLista, char nimi[]) {
    FILE *tiedosto;

    // Tiedoston avaus ja virheenkäsittely
    if ((tiedosto = fopen(nimi, "w")) == NULL) {
        fprintf(stderr, "Tiedoston avaaminen epäonnistui, lopetetaan\n");
        exit(0);
    }
    // DEBUG
    printf("\nStaring to write on file '%s'...\n", nimi); 
    
    fileLine *pNext = dLista;
    while (pNext != NULL) {
        fprintf(tiedosto, pNext->rivi);
        printf(pNext->rivi);
        pNext = pNext->pNext;
    }
    

    fclose(tiedosto);
    return;
}

void vapautaMuisti(fileLine *lista) {
    
    if (lista == NULL) {
        return;
    }

    fileLine *ptr = lista;
    while (ptr->pNext != NULL)
    {
        lista = ptr->pNext;
        // free(ptr->rivi);
        free(ptr);
        ptr = lista;
    }

    return;
}

void kaannaTiedosto(fileLine *pTiedosto) {
    fileLine *pAlku = pTiedosto, *pLoppu = pTiedosto, *pNext = NULL;
    char *string = NULL;
    size_t len = 0;
    ssize_t nread;
    while (pLoppu->pNext != NULL)
    {
        pLoppu = pLoppu->pNext;
    }
    
    while (pAlku != pLoppu) {
        string = pAlku->rivi;
        pAlku->rivi = pLoppu->rivi;
        pLoppu->rivi = string;

        if(pAlku->pNext == pLoppu) {
            break;
        }
        pNext = pAlku;
        while (pNext->pNext != pLoppu)
        {
            pNext = pNext->pNext;
        }
        pLoppu = pNext;
        pAlku = pAlku->pNext;
    }
    return;
}

int main(int argc, char *argv[])
{
    fileLine *pTiedosto;
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
    
    pTiedosto = lueTiedosto(pTiedosto, argv[1]);
    kaannaTiedosto(pTiedosto);
    kirjoitaTiedosto(pTiedosto, argv[2]);

    vapautaMuisti(pTiedosto);
    exit(0);
}
