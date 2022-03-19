#include <iostream>
#include "binary_search_tree.hpp"

using namespace std;
using namespace BST;

int main()
{
    binary_search_tree<int> T1 = {13, 6, 15, 17, 20, 9, 4, 3, 7, 2, 18};
    binary_search_tree<int> T2 = {15, 6, 18, 3, 7, 17, 20, 2, 4, 13, 9};

    cout << "T1 inorder traversal: "; T1.inorder_traversal(); cout << "\n";
    cout << "T2 inorder traversal: "; T2.inorder_traversal(); cout << "\n" << endl;

    T1.insert(18); T1.insert(11); T1.insert(25);
    T2.insert(18); T2.insert(11); T2.insert(25);

    cout << "T1 inorder traversal after inserting 18, 11 and 25: "; T1.inorder_traversal(); cout << "\n";
    cout << "T2 inorder traversal after inserting 18, 11 and 25: "; T2.inorder_traversal(); cout << "\n" << endl;

    T1.delete_node(6); T1.delete_node(13); T1.delete_node(18);
    T2.delete_node(6); T2.delete_node(13); T2.delete_node(18);

    cout << "T1 inorder traversal after deleting 6, 13 and 18: "; T1.inorder_traversal(); cout << "\n";
    cout << "T2 inorder traversal after deleting 6, 13 and 18: "; T2.inorder_traversal(); cout << "\n" << endl;

    return 0;
}
