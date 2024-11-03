//Logan Current, project 4, this file contains all the classes, AVLTree and class Node as well as the methods to each. 
#include <vector>
#include <iostream>
#include <string>

using namespace std;
class Node {
	public:
        Node* parent;
		Node* leftChild;
		Node* rightChild;
		int key;
        int height;
        int balance;
        string value;

        Node(int key, string value);
		~Node();

        //deletes the nodes
        void deleteMe();

        //prints the nodes
        friend ostream& printMe(Node* node, ostream& os, int depth);
};
class AVLTree
{
private:
	Node* root;
    int size;

public:
    
    AVLTree();

    AVLTree(AVLTree *other);

    ~AVLTree();

    void copyHelper(Node* node);

    void clear();

    /// @brief The time complexity for getHeight should be O(1).
    /// @return return the height of the AVL tree. 
    
    int getHeight();

    /// @brief  The time complexity for getSize should be O(1).
    /// @return return the total number of nodes (key/value pairs) in the AVL tree.
    int getSize();

    /// @brief if the given key is found in the AVL tree, this function should return true and place the corresponding value in value. Otherwise this function should return false (and the value in value can be anything). The time complexity for find should be O(log2 n). 
    /// @return correctly finds and returns key/value pairs in the tree in Θ(log n) time, and returns false when no matching key is found
    bool find(int key, string& value);

    ///@brief stores key/value pairs in the correct locations in the AVLTree, and correctly rejects duplicate keys
    ///@return Insert a new key/value pair into the tree. For this assignment the duplicate keys are not allowed. This function should return true if the key/value pair is successfully inserted into the map, and false if the pair could not be inserted (for example, due to a duplicate key already found in the map). The time complexity for insert should be O(log2 n).
    bool insert(int key, string value);

    /// @brief  correctly returns a C++ vector of strings matching keys in the specified range
    ///@return this function should return a C++ vector of strings containing all of the values in the tree with keys ≥ lowkey and ≤ highkey. For each key found in the given range, there will be one value in the vector. If no matching key/value pairs are found, the function should return an empty vector.

    void checkHeightAndBalance(Node* node);

    void changeHeightAfterBal(Node *node);

    void rebalanceTree(Node* currentNode);

    bool findHelper(Node* current, int keyToFind);

    void SRR(Node* prob);

    void SLR(Node* prob);

    vector<string> findRange(int lowkey, int highkey);

    void findRangeHelper(int low, int high, Node* node, vector<string>& vector);

    /// @brief correctly creates an independent copy of an AVL tree
    AVLTree& operator=(const AVLTree other);

    /// @brief prints the tree in a neat and readable manner, using indentation or some other appropriate mechanism to clearly show the structure of the tree
    friend ostream& operator<<(ostream& os, const AVLTree& me);

};