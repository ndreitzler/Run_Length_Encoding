//Guard
#ifndef MAC_H
#define MAC_H

//Macro to free allocated data and set pointer to null
#define FREE(x)                                         \
do {                                                    \
    free(x);                                            \
    x = NULL;                                           \
} while(0)

/*Macro to open a file and print error/exit if it fails
    Inputs:
        x - OUT file pointer
        y - IN  filename
        c - IN  mode
 */
#define FOPEN(x, y, c)                                  \
do {                                                    \
    x = fopen(y, c);                                    \
    if(x == NULL)                                       \
    {                                                   \
        printf("%s failed to open for %s\n", y, c);     \
        exit(-1);                                       \
    }                                                   \
} while(0)

//Macro to close file and set pointer to null
#define FCLOSE(x)                                       \
do {                                                    \
    fclose(x);                                          \
    x = NULL;                                           \
} while(0)

/*
#define MALLOC(x, bytes)                                            \
do{                                                                 \
    x = typeof(x)malloc(bytes);                                     \
    if(x == NULL)                                                   \
    {                                                               \
        printf("Unable to allocate %d bytes of memory\n", bytes);   \
        exit(0);                                                    \
    }                                                               \
} while(0)*/

void *my_malloc(int bytes);
void *my_realloc(void *ptr, int bytes);

#endif