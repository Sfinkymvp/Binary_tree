#ifndef _TREE_H_
#define _TREE_H_


#include "data.h"


#define TREE_INIT(name) BinaryTree name = {}


TreeStatus treeConstructor(BinaryTree* tree);
void treeDestructor(BinaryTree* tree);
TreeStatus addElement(BinaryTree* tree, DataType data);
TreeStatus addNode(Node* node, DataType data);
TreeStatus deleteData(BinaryTree* tree, DataType data);
TreeStatus deleteNode(Node* node);
void printTree(BinaryTree* tree);
void printNode(Node* node);


#endif // _TREE_H_
