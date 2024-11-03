//Logan Current, project 4, this file contains all the logic in each method initalized in the header file "AVLTree.h"

#include "AVLTree.h"

using namespace std;

Node::Node(int key, string value)
{
    //constructor for the node class
    parent = nullptr;
    leftChild = nullptr;
    rightChild = nullptr;
    height = 0;
    balance = 0;
    this->key = key;
    this->value = value;
}

Node::~Node()
{
}

AVLTree::AVLTree()
{
    //constructor for the tree
    root = nullptr;
    size = 0;
}

AVLTree::AVLTree(AVLTree* other)
{
    //creates a copy of the tree
    root = nullptr;
    size = 0;
    Node* nodeToAdd = other->root;
    copyHelper(other->root);
}

AVLTree &AVLTree::operator=(const AVLTree other)
{
    //creates a deep copy of the tree 

    //clear() has a segfault in it
    clear();
    root= nullptr;
    size = 0;
    Node* nodeToAdd = other.root;
    copyHelper(other.root);
    return *this;
}

void AVLTree::copyHelper(Node* node)
{
    //helps copy each node over from the old tree to the new one recursively 
    if(node != nullptr)
    {
        insert(node->key, node->value);
    }
    //copies left child
    if(node->leftChild != nullptr)
    {   
        insert(node->leftChild->key, node->leftChild->value);
        copyHelper(node->leftChild);
    }
    //copies right child
    if (node->rightChild != nullptr)
    {
        insert(node->rightChild->key, node->rightChild->value);
        copyHelper(node->rightChild);
    }
}

AVLTree::~AVLTree()
{
    //clear() has a segfault in it
    clear();
}

void AVLTree::clear()
{
    //continuously loops 
    root->deleteMe();
    size = 0;
    delete root;
    root = nullptr;
    return;
}

void Node::deleteMe()
{
    //tries to delete something that isn't there and can never return
    if(leftChild != nullptr){
        leftChild->deleteMe();
        delete leftChild;
    }
    if (rightChild != nullptr)
    {
        rightChild->deleteMe();
        delete rightChild;
    }
    return;
}

int AVLTree::getHeight()
{
    //returns the height
    return root->height;
}

int AVLTree::getSize()
{
    //returns the size
    return size;
}

bool AVLTree::findHelper(Node *current, int keyToFind)
{
    //finds the node with the associated key 
    while (current != nullptr)
    {
        if (current->key == keyToFind)
        {
            return true;
        }
        else if (current->key > keyToFind)
        {
            current = current->leftChild;
        }
        else
        {
            current = current->rightChild;
        }
    }
    return false;
}

bool AVLTree::find(int key, string &value)
{
    //helper
    return findHelper(root, key);
}

bool AVLTree::insert(int key, string value)
{
    //starting the tree, root
    if (root == nullptr)
    {
        root = new Node(key, value);
        root->parent = nullptr;
        root->rightChild = nullptr;
        root->leftChild = nullptr;
        root->height = 0;
        size = 1;
    }
    //checks to see if it's already been added
    else if (find(key, value))
    {
        return false;
    }
    //new node
    else
    {
        Node *newNode = new Node(key, value);
        Node *currentNode = root;
        //iterates thru and adds the node to the right spot in the tree
        while (currentNode != nullptr)
        {
            //greater than the root / node
            if (currentNode->key > key)
            {
                if (currentNode->leftChild == nullptr)
                {
                    newNode->parent = currentNode;
                    currentNode->leftChild = newNode;
                    //rebalance
                    checkHeightAndBalance(newNode);
                    size ++;
                    return true;
                }
                else
                {
                    currentNode = currentNode->leftChild;
                }
            }
            //less than the root / node
            else
            {
                if (currentNode->rightChild == nullptr)
                {
                    newNode->parent = currentNode;
                    currentNode->rightChild = newNode;
                    //rebalances
                    checkHeightAndBalance(newNode);
                    size ++;
                    return true;
                }
                else
                {
                    currentNode = currentNode->rightChild;
                }
            }
        }
    }
}

void AVLTree::checkHeightAndBalance(Node *nodeAdded)
{
    Node *currentNode = nodeAdded;
    //helps with off by 1 error for the height of each node since a node with no children's height is 0
    if (nodeAdded->parent == nullptr)
    {
        currentNode = root;
    }
    else
    {
        currentNode = nodeAdded->parent;
    }

    //incase the child is null
    int leftChildHeight = 0;
    int rightChildHeight = 0;
    //helps with keeping the balance in check
    int leftBalance = -1;
    int rightBalance = -1;

    //goes from the node added up to rebalance and put the right height to the right node
    while (currentNode != nullptr)
    {
        //checks to see if left child is null
        if (currentNode->leftChild != nullptr)
        {
            leftChildHeight = currentNode->leftChild->height;
            leftBalance = leftChildHeight;
        }
        //checks if right child is null
        if (currentNode->rightChild != nullptr)
        {
            rightChildHeight = currentNode->rightChild->height;
            rightBalance = rightChildHeight;
        }
        //fixes height
        currentNode->height = max(leftChildHeight, rightChildHeight) + 1;
        //fixes balance
        currentNode->balance = (leftBalance - rightBalance);
        //if the balance is off then rotate it
        if (currentNode->balance == 2 || currentNode->balance == -2)
        {
            rebalanceTree(currentNode);
        }
        currentNode = currentNode->parent;
    }
}

void AVLTree::rebalanceTree(Node *currentNode)
{
    //need a left rotate
    if (currentNode->balance == -2)
    {
        if (currentNode->rightChild->balance == 1)
        {
            // needs double rotation
            SRR(currentNode->rightChild);
            changeHeightAfterBal(currentNode->rightChild->rightChild);
        }
        SLR(currentNode);
        //fix the balance & height
        changeHeightAfterBal(currentNode);
        changeHeightAfterBal(currentNode->parent->leftChild);
    }
    //need a right rotate
    else if (currentNode->balance == 2)
    {
        //needs double rotation 
        if (currentNode->leftChild->balance == -1)
        {
            SLR(currentNode->leftChild);
            changeHeightAfterBal(currentNode->leftChild->leftChild);
        }
        SRR(currentNode);
        //fix the balance & height
        changeHeightAfterBal(currentNode);
        changeHeightAfterBal(currentNode->parent->rightChild);
    }
}

void AVLTree::changeHeightAfterBal(Node *node)
{
    //I made this method because when I tried to call the "checkHeightAndBalance" method it'd result in an off by one error. so I remade it to not give me an off by one error

    Node *currentNode = node;
    int leftChildHeight = -1;
    int rightChildHeight = -1;
    int leftBalance = -2;
    int rightBalance = -2;

    //goes from the node added up to rebalance and put the right height to the right node
    while (currentNode != nullptr)
    {
        //checks to see if left child is null
        if (currentNode->leftChild != nullptr)
        {
            leftChildHeight = currentNode->leftChild->height;
            leftBalance = leftChildHeight;
        }
        //checks if right child is null
        if (currentNode->rightChild != nullptr)
        {
            rightChildHeight = currentNode->rightChild->height;
            rightBalance = rightChildHeight;
        }
        //changes height
        currentNode->height = max(leftChildHeight, rightChildHeight) + 1;
        //changes balance
        currentNode->balance = (leftBalance - rightBalance);
        //rebalances if needed
        if (currentNode->balance == 2 || currentNode->balance == -2)
        {
            rebalanceTree(currentNode);
        }
        currentNode = currentNode->parent;
    }
}

void AVLTree::SRR(Node *prob)
{
    //rotates to the right
    Node *hook = prob->leftChild;
    Node *hookRightChild = hook->rightChild;
    if (prob->parent != nullptr)
    {
        hook->parent = prob->parent;
        prob->parent = hook;
        hook->rightChild = prob;
        hook->parent->leftChild = hook;
    }
    else
    {
        root = hook;
        root->parent = nullptr;
        prob->parent = root;
        root->rightChild = prob;
    }
    prob->leftChild = hookRightChild;
}

void AVLTree::SLR(Node *prob)
{
    //rotates to the left
    Node *hook = prob->rightChild;
    Node *hookLeftChild = hook->leftChild;

    if (prob->parent != nullptr)
    {
        hook->parent = prob->parent;
        prob->parent = hook;
        hook->leftChild = prob;
        hook->parent->rightChild = hook;
    }
    else
    {
        root = hook;
        root->parent = nullptr;
        prob->parent = root;
        root->leftChild = prob;
    }
    prob->rightChild = hookLeftChild;
}

vector<string> AVLTree::findRange(int lowkey, int highkey)
{
    //makes a vector of strings from the tree between the two values
    vector<string> returnVector;

    findRangeHelper(lowkey, highkey, root, returnVector);

    return returnVector;
}

void AVLTree::findRangeHelper(int low, int high, Node* node, vector<string>& vector)
{
    /*this method is incomplete / doesn't work like intend*/

    //helps find and add the value to the vector
    if (node->key >= low && node->key <= high)
    {
        vector.push_back(node->value);
        findRangeHelper(low, high, root->leftChild, vector);
        findRangeHelper(low, high, root->rightChild, vector);
    }
    if (node->key < low || node->key > high)
    {
        return;
    }
}

ostream& operator<<(ostream& os, const AVLTree& me)
{
    //calls a "helper" function
    printMe(me.root, os, 0);
    return os;
}

ostream& printMe(Node* node, ostream& os, int depth)
{
    //recursively prints each node in the correct order
    if(node != nullptr)
    {
        printMe(node->rightChild, os, depth + 1);
        cout << string(depth*3, ' ') << node->key << ", " << node->value << endl << endl;
        printMe(node->leftChild, os, depth + 1);
    }
    return os;
}
