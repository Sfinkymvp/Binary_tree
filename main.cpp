#include <stdio.h>


#include "tree.h"


int main()
{
    TREE_INIT(tree);
    treeConstructor(&tree);

    addElement(&tree, 4);

    addElement(&tree, 3);
    addElement(&tree, 2);
    addElement(&tree, 1);
    
    addElement(&tree, 5);
    addElement(&tree, 6);
    addElement(&tree, 7);

    printTree(&tree);

    treeDestructor(&tree);
    return 0;
}
