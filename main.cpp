#include <iostream>
#include <string>
#include "BSTree.h"

using namespace std;

int main()
{
    BSTree <int> tree;

    tree.insert(10);
    tree.insert(5);
    tree.insert(25);

    tree.printInOrder();
    tree.printPostOrder();
    tree.printPreOrder();

    int cheese = 25;
    bool val = false;
    val = tree.search(cheese);
    cout << val << endl;

    tree.remove(10);
    tree.printInOrder();

    return 0;
}
