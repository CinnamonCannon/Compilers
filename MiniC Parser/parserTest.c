#include <stdio.h>
#include <stdlib.h>
#include "Parser.h"

extern FILE* source;  // Declare the external source file pointer

// main function to drive the parser
int main(int argc, char* argv[])
{
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <source file>\n", argv[0]);
        return 1;
    }

    // Open the source file
    source = fopen(argv[1], "r");
    if (source == NULL) {
        fprintf(stderr, "Error: Cannot open source file '%s'\n", argv[1]);
        return 1;
    }

    printf("--- Start Parsing ---\n");
    parser(); // Call the parser
    printf("--- Parsing Finished ---\n");

    fclose(source); // Close the file
    return 0;
}