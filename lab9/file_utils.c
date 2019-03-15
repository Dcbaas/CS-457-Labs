#include <stdlib.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/stat.h>
#include "file_utils.h"

int read_file( char* filename, char **buffer ){
    int size;
    FILE* input_file = fopen(filename, "r");

    //Check if the file exist
    if(input_file ){

        //determine file size.
        struct stat st;
        stat(filename, &st);
        size = st.st_size;

        *buffer = (char*) malloc(size * sizeof(char));

        fread(*buffer, sizeof(char), size, input_file);

        fclose(input_file);
    }
    //If the file doesn't exist.
    else{
        size = -1;
    }

    return size;
}

int write_file( char* filename, char *buffer, int size){
    FILE* output_file = fopen(filename + 1, "w");
    int status = 1;

    printf("%s\n", filename);
    //Ensure that the out file didn't fail.
    if(output_file){
        fwrite(buffer, sizeof(char), size, output_file);
    }
    else{
        perror("File failed: ");
        status = -1;
        return -1;
    }

    fclose(output_file);
    return status;
}
