#ifndef _TREE_H_
#define _TREE_H_


#include "data.h"
#include "tree_io.h"
#include "tree_comparison.h"


#ifdef DEBUG

#include "html_builder.h"


#define TREE_INIT(name)                          \
    BinaryTree name = {};                        \
    name.debug.creation = (TreeCreationInfo) {   \
        #name, __FILE__, __func__, __LINE__      \
    }


#define TREE_DUMP(tree, __status, format, ...)   \
    treeDump(tree, __status, __FILE__, __func__, __LINE__, format, ##__VA_ARGS__)


#define TREE_VERIFY(tree, format, ...)                       \
    do {                                                     \
        TreeStatus _status = treeVerify(tree);               \
        TREE_DUMP(tree, _status, format, ##__VA_ARGS__);     \
        if (_status != TREE_OK) {                            \
            return _status;                                  \
        }                                                    \
    } while (0)

#endif // DEBUG


#define GENERATE_STATUS_MESSAGE(_status, message)      \
    ("[" #_status "] " message)


#define RETURN_IF_NOT_OK(_status) if (_status != TREE_OK) return _status


#define REPORT_IF_NOT_OK(_status)                      \
    do {                                               \
        if (_status != TREE_OK) {                      \
            printStatusMessage(_status);               \
            return _status;                            \
        }                                              \
    } while (0)


void printStatusMessage(TreeStatus status);


TreeStatus treeVerify(BinaryTree* tree);


TreeStatus akinatorStart(BinaryTree* tree);


TreeStatus createNode(Node** node);


TreeStatus treeConstructor(BinaryTree* tree, const int argc, const char** argv);


TreeStatus treeDestructor(BinaryTree* tree);


#endif // _TREE_H_
