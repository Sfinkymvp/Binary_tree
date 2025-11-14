#include <stdio.h>
#include <locale.h>


#include "tree.h"
#include "tree_test.h"


int main(const int argc, const char** argv)
{
    TREE_INIT(tree);
    TreeStatus status = treeConstructor(&tree, argc, argv);
    REPORT_IF_NOT_OK(status);

    status = akinatorStart(&tree);
    REPORT_IF_NOT_OK(status);

    compareNodes(&tree, "Egor", "Sasha");

    status = treeDestructor(&tree);
    REPORT_IF_NOT_OK(status);

    return 0;
}
