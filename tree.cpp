#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>


#include "tree.h"
#include "data.h"


const char* UNKNOWN_STRING = "unknown";


const char* status_messages[] = {
    GENERATE_STATUS_MESSAGE(TREE_OK, "Tree is valid"),
    GENERATE_STATUS_MESSAGE(TREE_RESTART, "Akinator will be restarted"),
    GENERATE_STATUS_MESSAGE(TREE_ERR, "Tree is not valid"),
    GENERATE_STATUS_MESSAGE(TREE_NULL_DATA_POINTER, "Node data is NULL"),
    GENERATE_STATUS_MESSAGE(TREE_ROOT_HAS_PARENT, "Root node has parent (must be NULL)"),
    GENERATE_STATUS_MESSAGE(TREE_MISSING_PARENT, "Node has no parent (must have)"),
    GENERATE_STATUS_MESSAGE(TREE_PARENT_CHILD_MISMATCH, "Parent-child link mismatch"),
    GENERATE_STATUS_MESSAGE(TREE_INVALID_BRANCH_STRUCTURE, "Node has one child (must be 0 or 2)"),
    GENERATE_STATUS_MESSAGE(TREE_OUT_OF_MEMORY, "Failed to allocate memory"),
    GENERATE_STATUS_MESSAGE(TREE_INPUT_READ_ERROR, "Failed to read user input"),
    GENERATE_STATUS_MESSAGE(TREE_INPUT_FILE_OPEN_ERROR, "Failed to open input file"),
    GENERATE_STATUS_MESSAGE(TREE_INPUT_FILE_READ_ERROR, "Failed to read data from input file"),
    GENERATE_STATUS_MESSAGE(TREE_INPUT_FILE_CLOSE_ERROR, "Failed to close input file"),
    GENERATE_STATUS_MESSAGE(TREE_OUTPUT_FILE_OPEN_ERROR, "Failed to open output file"),
    GENERATE_STATUS_MESSAGE(TREE_OUTPUT_FILE_WRITE_ERROR, "Failed to write to output file"),
    GENERATE_STATUS_MESSAGE(TREE_OUTPUT_FILE_CLOSE_ERROR, "Failed to close output file"),
    GENERATE_STATUS_MESSAGE(TREE_INVALID_COUNT, "Invalid objects count")
};


void printStatusMessage(TreeStatus status)
{
    fprintf(stderr, "%s\n", status_messages[status]);
}


static void openDumpFile(BinaryTree* tree)
{
    assert(tree);

    static int dump_counter = 1; 

    snprintf(tree->debug.dump.directory, BUFFER_SIZE, "%s/tree_dump_%03d",
             DUMP_DIRECTORY, dump_counter);

    char command[BUFFER_SIZE * 3] = {};
    snprintf(command, BUFFER_SIZE * 3, "rm -rf %s && mkdir -p %s",
             tree->debug.dump.directory, tree->debug.dump.directory);
    system(command);

    char filename[BUFFER_SIZE * 2] = {};
    snprintf(filename, BUFFER_SIZE * 2, "%s/tree_dump_%03d.html",
             tree->debug.dump.directory, dump_counter);

    tree->debug.dump.file = fopen(filename, "w");
    assert(tree->debug.dump.file);

    dump_counter++;
}


static TreeStatus nodeVerify(BinaryTree* tree, Node* node)
{
    assert(node);

    if (node->data == NULL)
        return TREE_NULL_DATA_POINTER;
    if (node->parent == NULL)
        return TREE_MISSING_PARENT;
    if (node->parent->left != node && node->parent->right != node)
        return TREE_PARENT_CHILD_MISMATCH;

    TreeStatus status = TREE_OK;
    if (node->left == NULL && node->right == NULL) {
        return TREE_OK;
    } else if (node->left != NULL && node->right != NULL) {
        status = nodeVerify(tree, node->left);
        if (status != TREE_OK)
            return status;
        status = nodeVerify(tree, node->right);
        if (status != TREE_OK)
            return status;
        return TREE_OK;
    }
    
    return TREE_INVALID_BRANCH_STRUCTURE;
}


TreeStatus treeVerify(BinaryTree* tree)
{
    assert(tree);
    assert(tree->root);

    TreeStatus status = TREE_OK;
    if (tree->root->parent != NULL)
        return TREE_ROOT_HAS_PARENT;
    if (tree->root->data == NULL)
        return TREE_NULL_DATA_POINTER;

    if (tree->root->left) {
        status = nodeVerify(tree, tree->root->left);
        if (status != TREE_OK)
            return status;
    }
    if (tree->root->right) {
        status = nodeVerify(tree, tree->root->right);
        if (status != TREE_OK)
            return status;
    }

    return TREE_OK;
}


static TreeStatus akinatorAddElement(BinaryTree* tree, Node* node, char* new_data, char* different_data)
{
    assert(tree); assert(node); assert(new_data); assert(different_data);

    TREE_DUMP(tree, TREE_OK, "Before add new object");

    TreeStatus status = createNode(&node->left);
    RETURN_IF_NOT_OK(status);
    node->left->data = strdup(new_data);
    if (node->left->data == NULL)
        return TREE_OUT_OF_MEMORY;

    TREE_DUMP(tree, TREE_OK, "After add new object");

    status = createNode(&node->right);
    RETURN_IF_NOT_OK(status);
    node->right->data = node->data;
    node->right->is_dynamic = node->is_dynamic;

    TREE_DUMP(tree, TREE_OK, "After move old object");

    node->data = strdup(different_data);
    node->is_dynamic = true;
    if (node->data == NULL)
        return TREE_OUT_OF_MEMORY;

#ifdef DEBUG
    node->left->parent = node;
    node->right->parent = node;
#endif // DEBUG

    return TREE_OK;
}


static TreeStatus readUserAnswer(char* buffer, int size)
{
    assert(buffer);

    if (fgets(buffer, size, stdin) == NULL)
        return TREE_INPUT_READ_ERROR;
    buffer[strcspn(buffer, "\n")] = '\0';

    return TREE_OK;
}


static TreeStatus processWrongGuess(BinaryTree* tree, Node* node)
{
    assert(tree); assert(node);

    char answer_buffer[BUFFER_SIZE] = {};
    char difference_buffer[BUFFER_SIZE] = {};

    printf("What is it?\n");
    TreeStatus status = readUserAnswer(answer_buffer, BUFFER_SIZE);
    RETURN_IF_NOT_OK(status);

    if (strcmp(node->data, UNKNOWN_STRING) == 0) {
        if (node->is_dynamic)
            free(node->data);
        node->data = strdup(answer_buffer);
        if (node->data == NULL)
            return TREE_OUT_OF_MEMORY;

        return TREE_RESTART;
    }        

    printf("How is '%s' different from '%s'?\n", answer_buffer, node->data);
    status = readUserAnswer(difference_buffer, BUFFER_SIZE);
    RETURN_IF_NOT_OK(status);

    status = akinatorAddElement(tree, node, answer_buffer, difference_buffer);
    RETURN_IF_NOT_OK(status);
    
    return TREE_RESTART;    
}


static TreeStatus akinatorGuessing(BinaryTree* tree, Node* node)
{
    assert(tree); assert(node); assert(node->data);

    char answer_buffer[BUFFER_SIZE] = {};

    printf("Maybe it is %s?\n", node->data);
    TreeStatus status = readUserAnswer(answer_buffer, BUFFER_SIZE);
    RETURN_IF_NOT_OK(status);
    
    if (strcmp(answer_buffer, "yes") == 0) {
        if (node->left == NULL)  return TREE_OK;
        else                     return akinatorGuessing(tree, node->left);
    } else {
        if (node->right == NULL) return processWrongGuess(tree, node);
        else                     return akinatorGuessing(tree, node->right);
    }
}


static void deleteBranch(Node* node)
{
    assert(node); assert(node->data);

    if (node->left) {
        deleteBranch(node->left);
        node->left = NULL;
    }
    if (node->right) {
        deleteBranch(node->right);
        node->right = NULL;
    }
    if (node->is_dynamic)
        free(node->data);
    free(node);
}


TreeStatus akinatorStart(BinaryTree* tree)
{
    assert(tree);

    TreeStatus status = TREE_OK;
    if (tree->root == NULL) {
        status = createNode(&tree->root);
        RETURN_IF_NOT_OK(status);
        
        tree->root->parent = NULL;   
        tree->root->data = strdup(UNKNOWN_STRING);
        if (tree->root->data == NULL)
            return TREE_OUT_OF_MEMORY;
    }

    TREE_VERIFY(tree, "Before launching akinatorGuessing");

    status = akinatorGuessing(tree, tree->root);
    if (status == TREE_RESTART)
        return akinatorStart(tree);
    else
        return status;
}


TreeStatus createNode(Node** node)
{
    assert(node);

    *node = (Node*)calloc(1, sizeof(Node));
    if (*node == NULL)
        return TREE_OUT_OF_MEMORY;
    (*node)->is_dynamic = true;

    return TREE_OK;
}


TreeStatus treeConstructor(BinaryTree* tree)
{
    assert(tree != NULL);

    tree->root = NULL;

    TreeStatus status = treeLoadFromDisk(tree);
    RETURN_IF_NOT_OK(status);

#ifdef DEBUG
    tree->debug.dump.image_counter = 1;
    openDumpFile(tree);
#endif // DEBUG

    return TREE_OK;
}


void treeDestructor(BinaryTree* tree)
{
    assert(tree); assert(tree->buffer);
    
    treeWriteToDisk(tree);

    if (tree->root == NULL)
        return;

    free(tree->buffer); 
    deleteBranch(tree->root);
    tree->root = NULL;
}


