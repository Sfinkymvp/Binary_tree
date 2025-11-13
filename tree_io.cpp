#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <assert.h>


#include "tree_io.h"
#include "data.h"
#include "tree.h"
#include "html_builder.h"


static void printTree(Node* node, FILE* output_file)
{
    assert(node);
    assert(output_file);

    fprintf(output_file, "(");

    fprintf(output_file, "\"%s\"", node->data);
    if (node->left)
        printTree(node->left, output_file);
    else
        fprintf(output_file, " nil");
    if (node->right)
        printTree(node->right, output_file);
    else
        fprintf(output_file, " nil");
        
    fprintf(output_file, ")");
}


TreeStatus treeWriteToDisk(BinaryTree* tree)
{
    TREE_VERIFY(tree, "Before write tree to disk");

    FILE* output_file = fopen("tree_inout", "w");
    if (output_file == NULL)
        return TREE_OUTPUT_FILE_OPEN_ERROR;

    printTree(tree->root, output_file);
    fprintf(output_file, "\n");

    if (fclose(output_file) != 0)
        return TREE_OUTPUT_FILE_CLOSE_ERROR;

    return TREE_OK;
}


static size_t getFileSize(const char* input_filename)
{
    assert(input_filename != NULL);

    struct stat buf = {};

    if (stat(input_filename, &buf) == -1)
        return 0;

    return (size_t)buf.st_size;
}


TreeStatus treeLoadFromDisk(BinaryTree* tree)
{
    assert(tree);

    FILE* input_file = fopen("tree_inout", "r");
    if (input_file == NULL)
        return TREE_INPUT_FILE_OPEN_ERROR; 

    size_t file_size = getFileSize("tree_inout"); 
    if (file_size == 0) {
        fclose(input_file);
        return TREE_INPUT_FILE_OPEN_ERROR;
    }

    tree->buffer = (char*)calloc(file_size + 1, 1);
    if (tree->buffer == NULL) {
        fclose(input_file);
        return TREE_OUT_OF_MEMORY;
    }

    size_t read_size = fread(tree->buffer, 1, file_size, input_file);
    if (read_size != file_size) {
        fclose(input_file);
        free(tree->buffer);
        return TREE_INPUT_FILE_READ_ERROR;
    }

    printf("BUFFER DUMP\n");
    int position = 0;    
    TreeStatus status = readNode(tree, &tree->root, &position);
    if (status != TREE_OK) {
        fclose(input_file);
        free(tree->buffer);
        return status;
    }

    if (fclose(input_file) != 0) {
        free(tree->buffer);
        return TREE_INPUT_FILE_CLOSE_ERROR;
    }

    return TREE_OK;
}


static inline void skipWhitespaces(char* buffer, int* position)
{
    assert(buffer), assert(position);

    while (buffer[*position] == ' '  || buffer[*position] == '\t' ||
           buffer[*position] == '\n' || buffer[*position] == '\r')
        (*position)++;
}


static TreeStatus readTitle(char** destination, char* buffer, int* position)
{
    assert(destination); assert(buffer); assert(position);

    int read_len = 0;
    int result = sscanf(buffer + *position, "\"%*[^\"]\"%n", &read_len);
    if (result != 0)
        return TREE_ERR;

    buffer[*position + read_len - 1] = '\0';
    *destination = buffer + *position + 1;
    *position += read_len;

    return TREE_OK;
}


TreeStatus readNode(BinaryTree* tree, Node** node, int* position)
{
    assert(tree); assert(tree->buffer); assert(node); assert(position);
    assert(*node == NULL);

    skipWhitespaces(tree->buffer, position);

    for (int index = 0; index < 60; index++)
        printf("_");
    printf("\n");

    printf("%s", tree->buffer + *position);

    TreeStatus status = TREE_OK;
    if (tree->buffer[*position] == '(') {
        status = createNode(node);
        RETURN_IF_NOT_OK(status);
        (*position)++;

        skipWhitespaces(tree->buffer, position);

        status = readTitle(&(*node)->data, tree->buffer, position);
        (*node)->is_dynamic = false;
        RETURN_IF_NOT_OK(status);

        skipWhitespaces(tree->buffer, position);
        status = readNode(tree, &(*node)->left, position);
        RETURN_IF_NOT_OK(status);
        if ((*node)->left != NULL)
            (*node)->left->parent = *node;

        skipWhitespaces(tree->buffer, position);
        status = readNode(tree, &(*node)->right, position);
        RETURN_IF_NOT_OK(status);
        if ((*node)->right != NULL)
            (*node)->right->parent = *node;

        skipWhitespaces(tree->buffer, position);
        (*position)++;

        return TREE_OK;
    } else if (strncmp(&tree->buffer[*position], "nil", 3) == 0) {
        (*position) += 3;
        *node = NULL;
        return TREE_OK;
    }
 
    return TREE_ERR;
}
