#include <stdio.h>
#include <stdlib.h>
#include <assert.h>


#include "tree.h"
#include "data.h"


TreeStatus treeConstructor(BinaryTree* tree)
{
    assert(tree != NULL);

    tree->node = NULL;
    tree->size = 0;

    return TREE_OK;
}


void treeDestructor(BinaryTree* tree)
{
    assert(tree);
    assert(tree->node);

    deleteNode(tree->node);
    tree->node = NULL;
}


TreeStatus addElement(BinaryTree* tree, DataType data)
{
    assert(tree);

    if (tree->node == NULL) {
        tree->node = (Node*)calloc(1, sizeof(Node));
        if (tree->node == NULL)
            return TREE_ERR;
        tree->node->data = data;
        tree->size = 1;
        return TREE_OK;
    }

    tree->size++;
    return addNode(tree->node, data);
}


TreeStatus addNode(Node* node, DataType data)
{
    assert(node);

    if (data <= node->data) {
        if (node->left) {
            addNode(node->left, data);
        } else {
            node->left = (Node*)calloc(1, sizeof(Node));
            if (node->left == NULL)
                return TREE_ERR;
            node->left->data = data;
        }
    } else {
        if (node->right) {
            addNode(node->right, data);
        } else {
            node->right = (Node*)calloc(1, sizeof(Node));
            if (node->right == NULL)
                return TREE_ERR;
            node->right->data = data;
        }
    }

    return TREE_OK;
}


TreeStatus deleteData(BinaryTree* tree, DataType data)
{
    assert(tree);
    assert(tree->node);

    if (data == tree->node->data) {
        if (tree->node->left)
            deleteNode(tree->node->left);
        if (tree->node->right)
            deleteNode(tree->node->right);
    }

    return TREE_OK;
}


TreeStatus deleteNode(Node* node)
{
    assert(node);

    if (node->left) {
        deleteNode(node->left);
        node->left = NULL;
    }
    if (node->right) {
        deleteNode(node->right);
        node->right = NULL;
    }
    free(node);

    return TREE_OK;
}


void printNode(Node* node)
{
    assert(node);

    printf("(");

    if (node->left)
        printNode(node->left);
    printf("%d", node->data);
    if (node->right)
        printNode(node->right);
        
    printf(")");
}


void printTree(BinaryTree* tree)
{
    assert(tree);

    printf("Size: %d\n", tree->size);
    printNode(tree->node);
    printf("\n");
}

