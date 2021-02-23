/*  Name: Nick Dreitzler
    Username: ndreitz
    CUID: C15564443
    Class: ECE 4730
    Professor: Walt Ligon
    Semester: Fall2020
    Filename: make-list.c

    Purpose: To compress a given file using run length encoding

    Input:  -c <filename>
            -u <filename>
            -e <file extension>
            -h

    Output: .rle file if compression or uncompressed file with given extension
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "macros.h"

//#include "file_interact.h"
#include "macros.h"

#define MAXVAL 0xFF

enum Mode{none, compress, uncompress};

void rle_compress(char **filename);
void add_rle_bytes_to_file(FILE *fptr, int *count, unsigned char val);
void rle_uncompress(char *filename, char *extension);
void add_decode_to_file(FILE *fptr, unsigned char *data, unsigned char *buff);
unsigned char *read_file_data(char *filename, int *len);
char *uncompress_filename(char **filename, char *extension);
char *rle_filename(char **filename);
void check_rle(char *filename);
void print_usage(void);

int main(int argc, char **argv)
{
    enum Mode mode = none;
    char *filename = NULL;
    char *extension = NULL;
    int opt;

    if(argc == 1)
    {
        print_usage();
    }

    //Parse Inputs
    while((opt = getopt(argc, argv, "c:u:e:h")) != -1)
    {
        switch(opt)
        {
            case 'c':
                filename = strdup(optarg);
                mode = compress;
                break;
            case 'u':
                filename = strdup(optarg);
                mode = uncompress;
                break;
            case 'e':
                extension = strdup(optarg);
                break;
            case 'h':
                print_usage();
                break;
        }
    }

    if(mode == compress)
        rle_compress(&filename);
    else if(mode == uncompress)
        rle_uncompress(filename, extension);
    else
        print_usage();

    if(filename != NULL)
        FREE(filename);
    if(extension != NULL)
        FREE(extension);
}

/*Purpose: to perform run length encoding compression on a given file
  Return: Will Create a .rle file which contains the compressed data
 */
void rle_compress(
    char **filename /*IN - name of file to compress*/)
{
    FILE *fptr;         /*.rle file*/
    char *new_fn;       /*filename for .rle file*/
    unsigned char *data;/*Data buffer for uncompressed file*/
    int len;            /*Length of data*/
    int count;          /*Count of successive bytes*/
    int i;

    //Read entire file and store data
    data = read_file_data(*filename, &len);

    //Take filename and add .rle
    new_fn = rle_filename(filename);
    FOPEN(fptr, new_fn, "w");

    count = 0;
    //Loop through remaining data comparing current and next data byte
    for(i = 0; i < len-1; i++)
    {
        //If same increase count,
        //If not add bytes to file and reset count
        if(data[i] == data[i+1])
        {
            count++;
            //If count has reached max value than add bytes to file and reset count
            if(count == MAXVAL)
            {
                add_rle_bytes_to_file(fptr, &count, data[i]);
                //Increment i so that data[i+1] is not double counted
                i++;
            }
        } else {
            add_rle_bytes_to_file(fptr, &count, data[i]);
        }
    }

    //If last iteration of for loop results in a max value code than i == len and
    //last byte does not need to be added to file
    if(i != len)
        add_rle_bytes_to_file(fptr, &count, data[i]);

    FCLOSE(fptr);
    FREE(data);
}


/*Purpose: Add 2 bytes of rle encoding to file and reset count.
    count stores the number of the val character in a row
 */
void add_rle_bytes_to_file(
    FILE *fptr,      /*IN - .rle file to write to*/
    int *count,      /*IN - length of the current sequence*/
    unsigned char val/*IN - char value to store*/)
{
    unsigned char data[2]; /*data[0] stores the number of data[1] in a row*/

    //Add count and value to data
    data[0] = (unsigned char)*count;
    data[1] = val;

    fwrite(data, 1, 2, fptr);

    *count = 0;
}

/*Purpose: Read a given rle file and decode it into a new file with the same name and given extension
 */
void rle_uncompress(
    char *filename,/*IN - .rle file to be decoded*/
    char *extension/*IN - extenstion to add to filename*/ )
{
    FILE *fptr;     /*Uncompressed file*/
    char *new_fn;   /*Name of uncompressed file*/
    unsigned char *rle_file_data; /*Data from .rle file*/ 
    unsigned char *buff = (unsigned char *)my_malloc(MAXVAL + 1); /*Buffer to store uncompressed data before sending it to fptr*/ 
    int len; /*length of .rle file*/
    int i;

    //Verify that filename is a .rle file, exit if not
    check_rle(filename);

    //Read data from file
    rle_file_data = read_file_data(filename, &len);

    //Generate new filename based off of given extension
    new_fn = uncompress_filename(&filename, extension);

    FOPEN(fptr, new_fn, "w");

    //Decode data
    for(i = 0; i < len; i = i + 2)
        add_decode_to_file(fptr, &rle_file_data[i], buff);

    FCLOSE(fptr);
    FREE(rle_file_data);
    FREE(buff);
}

/*Purpose: add decoded rle data to a file.
    data[0] holds the number of data[1] symbols that must be added to the file.
    In this implementation 0 means add 1, 1 means add 2, and so on
 */
void add_decode_to_file(
    FILE *fptr,         /*IN - file pointer to opened file*/
    unsigned char *data,/*IN - pointer to rle data*/
    unsigned char *buff /*IN - buffer to hold data before adding it to the file*/)
{
    unsigned int num = (unsigned int)data[0] + 1; /*Number of data[1] to add to buffer*/ 
    int i;

    //data[0] holds the number of data[1] that must be added to the buffer
    for(i = 0; i < num; i++)
        buff[i] = data[1];
        
    fwrite(buff, 1, num, fptr);
}

/*Purpose: read entire contents of given file
    Will malloc space for data buffer
  Return: data buffer containing entire contents of given file and length of data
 */
unsigned char *read_file_data(
    char *filename, /*IN  - filename to read from*/
    int *len        /*OUT  - length of data buffer*/)
{
    FILE* fptr;         /*File which will be read from*/
    unsigned char *data;/*buffer to store file data*/

    FOPEN(fptr, filename, "r");

    //Determine length of file
    fseek(fptr, 0, SEEK_END);
    *len = ftell(fptr);
    rewind(fptr);

    //Read data into buffer
    data = (unsigned char *)my_malloc(*len);
    fread(data, 1, *len, fptr);

    FCLOSE(fptr);
    return data;
}

/*Purpose: To convert the given filename to one with a .rle extension
  Return: a string with the desired .rle extension
 */
char *rle_filename(
    char **filename /*IN/OUT - file name to be converted to have a .rle extension, might be realloced*/)
{
    char *name; /*New filename*/
    char *loc;  /*location of the first period in the given filename*/
    int len = strlen(*filename); /*Length of filename*/

    //Find location of first period
    loc = strstr(*filename,".");

    //loc == null means file does not have a file extension
    // < len + 4 means file extenstion is less than four chars including the period
    if(loc == NULL || strlen(loc) < len + 4)
    {
        //add 5 bytes to filename string, 4 for ".rle" and 1 for NULL at end
        *filename = (char *)my_realloc(*filename, len + 5);
    }

    //Spilt given filename using . as a delimiter
    name = strtok(*filename, ".");

    //Add .rle to new filename
    strcat(name,".rle");

    return name;
}

//Purpose: replace .rle file extension with user given file extension
char *uncompress_filename(
    char **filename, /*IN/OUT - filename to have user given extension added*/
    char *extension /*IN - user given file extension*/)
{
    char *name;                 /*New filename*/
    int len = strlen(extension);/*Length of extension*/

    //Find location of first period
    name = strtok(*filename, ".");

    //If given extension does not have period add one
    if(strstr(extension, ".") != NULL)
    {
        // .rle file has room for an extension with 3 chars, if it has more filename must be realloced
        if(len > 4)
            *filename = (char *)my_realloc(*filename, strlen(*filename) + len + 1);
        strcat(name, extension);
    } else {
        // .rle file has room for an extension with 3 chars, if it has more filename must be realloced
        if(len > 3)
            *filename = (char *)my_realloc(*filename, strlen(*filename) + len + 1);
        strcat(name, ".");
        strcat(name, extension);
    }

    return name;
}

/*Purpose: inputed filename is a .rle file
 */
void check_rle(
    char *filename/*IN - given filename*/)
{
    int len = strlen(filename); /*length of filename*/ 
    
    if(len < 4 || filename[len-4] != '.' 
               || filename[len-3] != 'r'
               || filename[len-2] != 'l'
               || filename[len-1] != 'e')
    {
        printf("For uncompression a .rle file is required\n");
        exit(0);
    }
    
}

/*Print the usage for the program*/
void print_usage(void)
{
    printf("Usage:\n");
    printf("\t-c <filename>, compress given file\n");
    printf("\t-r <filename>, uncompress given file\n");
    printf("\t-e <file extension>, desired file extension for uncompressed file\n");
    printf("\t-h, print usage\n");
    printf("Only one file can be compressed or uncompressed per execution of program\n");
    exit(1);
}