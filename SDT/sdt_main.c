#include <stdio.h>
#include <stdlib.h>

#include "Parser.h"

FILE* sourceFile = NULL;
FILE* astFile = NULL;

static void printUsage(const char* program)
{
    fprintf(stderr, "Usage: %s <MiniC source file>\n", program);
}

int main(int argc, char* argv[])
{
    Node* root;

    if (argc != 2) {
        printUsage(argv[0]);
        return EXIT_FAILURE;
    }

    sourceFile = fopen(argv[1], "r");
    if (sourceFile == NULL) {
        perror("fopen");
        return EXIT_FAILURE;
    }
    
    astFile = stdout;

    puts("--- mini C parser start ---");
    root = parser();
    if (root != NULL) {
        puts("--- mini C parser finished (tree produced) ---");
        puts("--- AST output ---");
        printTree(root, 0);
    } else {
        puts("--- parser returned no tree ---");
    }

    fclose(sourceFile);
    return EXIT_SUCCESS;
}
