#ifndef _DATA_H_
#define _DATA_H_


typedef int DataType;


typedef enum {
    TREE_OK = 0,
    TREE_ERR
} TreeStatus;


typedef struct Node Node;
struct Node {
    DataType data;
    Node* left;
    Node* right;
};


typedef struct {
    Node* node;
    int size; 
} BinaryTree;


#endif // _DATA_H_
