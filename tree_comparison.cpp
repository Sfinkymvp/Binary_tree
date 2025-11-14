#include <stdio.h>
#include <assert.h>
#include <string.h>


#include "tree_comparison.h"
#include "tree.h"
#include "stack.h"


static TreeStatus findPathToNode(Node* node, Stack_t* stack, const char* data)
{
    assert(stack); assert(data);

    if (node == NULL)
        return TREE_NOT_FOUND;
    if (strcmp(node->data, data) == 0) {
        stackPush(stack, node);
        return TREE_OK;
    }

    TreeStatus status = findPathToNode(node->left, stack, data);
    if (status == TREE_OK) {
        stackPush(stack, node);
        return TREE_OK;
    }
    
    status = findPathToNode(node->right, stack, data);
    if (status == TREE_OK) {
        stackPush(stack, node);
        return TREE_OK;
    }

    return TREE_NOT_FOUND;
}

static void printNodesComparision(Stack_t* object_path_1, Stack_t* object_path_2)
{
    assert(object_path_1); assert(object_path_2);
    assert(object_path_1->data); assert(object_path_2->data);

    
    Node* node_1 = NULL;
    Node* node_2 = NULL;
    stackPop(object_path_1, &node_1);
    stackPop(object_path_2, &node_2);

    printf("What both nodes have in common:\n");
    while (node_1 == node_2) {
        printf("%s\n", node_1->data);
        if (object_path_1->size != 0 && object_path_2->size != 0) {
            stackPop(object_path_1, &node_1);
            stackPop(object_path_2, &node_2);
        } else
            return;
    }
    
    printf("\nProperties of 1 node:\n");        
    while (object_path_1->size > 1) {
        printf("%s\n", node_1->data);
        stackPop(object_path_1, &node_1);
    }
    stackPop(object_path_1, &node_1);
    printf("And finally, its name: %s\n", node_1->data);

    printf("\nProperties of 2 node:\n");
    while (object_path_2->size > 1) {
        printf("%s\n", node_2->data);
        stackPop(object_path_2, &node_2);
    }
    stackPop(object_path_2, &node_2);
    printf("And finally, its name: %s\n", node_2->data);
}


TreeStatus compareNodes(BinaryTree* tree, const char* data_1, const char* data_2)
{
    assert(tree); assert(data_1); assert(data_2);

    Stack_t stack_1 = {};
    Stack_t stack_2 = {};
    stackCtor(&stack_1, START_CAPACITY);
    stackCtor(&stack_2, START_CAPACITY);

    if (findPathToNode(tree->root, &stack_1, data_1) == TREE_NOT_FOUND ||
        findPathToNode(tree->root, &stack_2, data_2) == TREE_NOT_FOUND) {
        stackDtor(&stack_1);
        stackDtor(&stack_2);
        return TREE_NOT_FOUND;
    }

    printf("Comparison of nodes '%s' and '%s'\n", data_1, data_2);
    printNodesComparision(&stack_1, &stack_2);

    stackDtor(&stack_1);
    stackDtor(&stack_2);
    return TREE_OK;
}

