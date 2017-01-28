/* 
 * File:   2.c
 * Author: Alex Hollis
 *
 * Created on February 1, 2015, 9:09 PM
 */

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <time.h>

/*
 * 
 */
int main(int argc, char** argv) {
    
    
    int output, input, fread;
    char buf[128];
    
    // timing //
    
    clock_t start, end;
    double time_used;
    
    // timing //
    
    
    output = open("fileb.txt", O_CREAT | O_APPEND | O_RDWR);
    
    input = open("filea.txt", O_RDONLY);
    
    start = clock();
    while(( fread = read(input, &buf, 4 ) ) > 0 )   
    {
        write(output, &buf, (ssize_t)fread);
        
        
    }
    
    end = clock();
    close(input);
    close(output);
    
    time_used = ((double)(end - start)) / CLOCKS_PER_SEC;       // runtime
    printf("time: %.12f\n", time_used);        // will log these times to spreadsheet to create a graph.
    
    
    
    

    return (EXIT_SUCCESS);
}

