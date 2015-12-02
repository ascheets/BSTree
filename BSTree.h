#pragma once
#include <iostream>
#include <cmath>
#include "BSTreeExceptions.h"

using namespace std;

template <class T>
class BSTree
{
 public:
    BSTree();
    ~BSTree();

    void insert(const T& elem);
    bool remove(const T& elem);
    bool search(const T& elem);

    void printInOrder();
    void printPostOrder();
    void printPreOrder();

    int length();

    T findMax();
    T findMin();

    int heightOfTree();
    double percentLeaf();
    double percentInterior();
    void printInTreeForm();

 private:
    
    struct Node
    {
	Node* p_left;
	T data;
	Node* p_right;
	int heightLeft;
	int heightRight;
    };

    Node* p_root;
    int numElements;    

    void dtorHelper(Node*& p_node);

    void insertHelper(Node*& p_node, const T& elem);

    void removeHelper(Node*& p_node, const T& elem, bool& found);
    void deleteNode(Node*& p_node);
    void getPredecessor(Node* p_node, T& elem);
    
    void searchHelper(Node* p_node, const T& elem, bool& found);

    void InOrderHelper(Node* p_node);
    void PostOrderHelper(Node* p_node);
    void PreOrderHelper(Node* p_node);

    void findMaxHelper(Node*& p_node, T& elem);
    void findMinHelper(Node* p_node, T& elem);

    int getLeaves();
    void getLeavesHelper(int desiredLevel, int currentLevel, int& numLeaf, Node* p_node);

    int bottomTop();
    void bottomTopHelper(int desiredLevel, int currentLevel, Node* p_node);

    int topBottom();
    void topBottomHelper(int desiredLevel, int currentLevel, Node* p_node, double padding);

    bool isBalanced(Node* p_node);
    void balanceLeft(Node*& p_node);
    void balanceRight(Node*& p_node);

    void findHeights();
    void findHeightsHelper(Node*& p_node, int& counter);    
};

//ctor
template <class T>
BSTree <T> :: BSTree()
{
    //set root node to null
    p_root = 0;
    //set numElements
    numElements = 0;
}

//dtor
template <class T>
BSTree <T> :: ~BSTree()
{
    //use post-order to free up nodes
    dtorHelper(p_root);
}

//dtorHelper
template <class T>
void BSTree <T> :: dtorHelper(Node*& p_node)
{
    //p_node is null is base case
    if(p_node != 0)
    {
	//look left
	dtorHelper(p_node->p_left);
	//look right
	dtorHelper(p_node->p_right);
	//process node (delete, set to null)
	delete p_node;
	p_node = 0;
    }

}

//insert
template <class T>
void BSTree <T> :: insert(const T& elem)
{
    //if the element isn't already in the tree
    if(!(search(elem)))
    {
	//always start at root node,
	//start recursive insert process
	insertHelper(p_root, elem);

	numElements++;
    }
    //if the element is found, throw exception
    else
    {
	cout << "Duplicate eror exception thrown here" << endl;
	//BSTreeDuplicateException error;
	//throw error;
    }
}

//insertHelper
template <class T>
void BSTree <T> :: insertHelper(Node*& p_node, const T& elem)
{
    //reach a null pointer?
    if(p_node == 0)
    {
	//create a new node, assign to p_node
	p_node = new Node;	    
	//assign data to node
	p_node->data = elem;
	//set node left and right
	p_node->p_left = 0;
	p_node->p_right = 0;
	//set heights for new node
	p_node->heightLeft = 0;
	p_node->heightRight = 0;
    }
    else if(elem <= p_node->data)
    {
	//if data is less than or equal to current node data,
	//recursively go left
	insertHelper(p_node->p_left, elem);
	//increment p_node->heightLeft
	findHeights();

	//is the tree balanced from this point?
	if(!isBalanced(p_node))
	{	    
	    balanceLeft(p_node);
	    findHeights();

	    if(!isBalanced(p_node))
	    {
		balanceLeft(p_node->p_right);
		findHeights();

		balanceRight(p_node);
		findHeights();
	    }

	}
    }
    else
    {
	//if data is greater than current node data
	//recursively go right
	insertHelper(p_node->p_right, elem);
	//increment p_node->heightRight
	//p_node->heightRight++;
	findHeights();

	//is the tree balanced from this point?
	if(!isBalanced(p_node))
	{
	    balanceRight(p_node);
	    //update heights
	    findHeights();
	
	    if(!isBalanced(p_node))
	    {
		balanceRight(p_node->p_left);
		findHeights();	
		balanceLeft(p_node);
		findHeights();	    
	    }
	}
    }

}

//isBalanced
template <class T>
bool BSTree <T> :: isBalanced(Node* p_node)
{
    bool retVal = true;

    if( abs(p_node->heightLeft - p_node->heightRight) > 1)
    {
	retVal = false;
    }

    return retVal;

}

//remove
template <class T>
bool BSTree <T> :: remove(const T& elem)
{
    bool retVal = false;

    //should check to see if in tree before trying to delete?
    if(search(elem))
    {
	//start at the root node
	removeHelper(p_root, elem, retVal);

	numElements--;
    }

    return retVal;
}

//removeHelper
template <class T>
void BSTree <T> :: removeHelper(Node*& p_node, const T& elem, bool& found)
{
    if(p_node == 0)
    {
	//node must not be in the tree
	found = false;
    }
    else if(elem < p_node->data)
    {
	//need to keep searching left
	removeHelper(p_node->p_left, elem, found);
	//update heights
	findHeights();
    }
    else if(elem > p_node->data)
    {
	//need to keep searching right
	removeHelper(p_node->p_right, elem, found);
	//update heights
	findHeights();
    }
    else
    {
	//node has been found
	deleteNode(p_node);
	found = true;
	//decrement numElements
	numElements--;
    }

}

//deleteNode
template <class T>
void BSTree <T> :: deleteNode(Node*& p_node)
{
    //need to figure out how many children node has

    //no children
    if(p_node->p_left == 0 &&
       p_node->p_right == 0)
    {
	//delete the node
	delete p_node;
	//set the node equal to zero
	p_node = 0;

    }
    //one child
    else if(p_node->p_left == 0 &&
	    p_node->p_right != 0)
    {
	//temp node
	Node* toDelete;
	//set temp to node we want to delete
	toDelete = p_node;
	//bypass node about to delete
	p_node = p_node->p_right;
	//delete node as desired
	delete toDelete;

    }
    else if(p_node->p_left != 0 &&
	    p_node->p_right == 0)
    {
	//temp node
	Node* toDelete;
	//set temp to node we want to delete
	toDelete = p_node;
	//bypass node about to delete
	p_node = p_node->p_left;
	//delete node as desired
	delete toDelete;

    }
    //two children
    else
    {
	bool found = false;
	T predInfo;
	//find elem associated with predecessor node, start at left child...
	getPredecessor(p_node->p_left, predInfo);
	//set info of p_node to predInfo
	p_node->data = predInfo;
	//delete the node we stole info from using removeHelper,
	//starting at left child of p_node
	removeHelper(p_node->p_left, predInfo, found);
    }

}

//getPredecessor
template <class T>
void BSTree <T> :: getPredecessor(Node* p_node, T& elem)
{
    //base cases, node is null
    if(p_node == 0)
    {
	//essentially do nothing, start returning,
	//elem should be info we want
    }
    //otherwise, recursively go right, general case
    else
    {
	elem = p_node->data;
	getPredecessor(p_node->p_right, elem);
    }
}

//search
template <class T>
bool BSTree <T> :: search(const T& elem)
{
    bool retVal = false;

    //start at the root node
    searchHelper(p_root, elem, retVal);

    return retVal;
}

//searchHelper
template <class T>
void BSTree <T> :: searchHelper(Node* p_node, const T& elem, bool& found)
{
    //base cases, node is null
    if(p_node == 0)
    {
	//element was not found
	found = false;
    }
    //node data is equal to elem
    else if(p_node->data == elem)
    {
	//switch retVal
	found = true;

    }
    //general case
    else
    {
	//if elem is less than current node data
	if(p_node->data > elem)
	{
	    //go left
	    searchHelper(p_node->p_left, elem, found);
	}
	else
	{
	    //go right
	    searchHelper(p_node->p_right, elem, found);
	}
    }
}

//printInOrder
template <class T>
void BSTree <T> :: printInOrder()
{
    //always start at the root node
    InOrderHelper(p_root);
    cout << endl;
}

//InOrderHelper
template <class T>
void BSTree <T> :: InOrderHelper(Node* p_node)
{
    //p_node is null is base case
    if(p_node != 0)
    {
	//look left
	InOrderHelper(p_node->p_left);
	//process node
	cout << p_node->data << " ";
	//look right
	InOrderHelper(p_node->p_right);
    }

}

//printPostOrder
template <class T>
void BSTree <T> :: printPostOrder()
{
    //start at root node
    PostOrderHelper(p_root);
    cout << endl;
}

//PostOrderHelper
template <class T>
void BSTree <T> :: PostOrderHelper(Node* p_node)
{
    //p_node is null is base case
    if(p_node != 0)
    {
	//look left
	PostOrderHelper(p_node->p_left);
	//look right
	PostOrderHelper(p_node->p_right);
	//process node
	cout << p_node->data << " ";
    }
}

//printPreOrder
template <class T>
void BSTree <T> :: printPreOrder()
{
    //start at root node
    PreOrderHelper(p_root);
    cout << endl;
}

//PreOrderHelper
template <class T>
void BSTree <T> :: PreOrderHelper(Node* p_node)
{
    //base case, node is null
    if(p_node != 0)
    {
	//process node
	cout << p_node->data << " ";
	//look left
	PreOrderHelper(p_node->p_left);
	//look right
	PreOrderHelper(p_node->p_right);
    }
}

//length
template <class T>
int BSTree <T> :: length()
{
    return numElements;
}

//findMax
template <class T>
T BSTree <T> :: findMax()
{
    T max;

    //start at the root node
    findMaxHelper(p_root, max);

    return max;
}

//findMaxHelper
template <class T>
void BSTree <T> :: findMaxHelper(Node*& p_node, T& elem)
{
    //base case
    if(p_node == 0)
    {
	//start returning...
    }
    else
    {
	elem = p_node->data;
	findMaxHelper(p_node->p_right, elem);
    }

}

//findMin
template <class T>
T BSTree <T> :: findMin()
{
    T min;

    //start at the root node
    findMinHelper(p_root, min);

    return min;
}

//findMinHelper
template <class T>
void BSTree <T> :: findMinHelper(Node* p_node, T& elem)
{
    //base case
    if(p_node == 0)
    {
	//start returning...
    }
    //general case
    else
    {
	//set element
	elem = p_node->data;
	//go left
	findMinHelper(p_node->p_left, elem);
    }

}

//heightOfTree
template <class T>
int BSTree <T> :: heightOfTree()
{
    int retVal = 0;

    if(p_root == 0)
    {
	//cout << "tree is empty, no height to be found" << endl;
    }
    else
    {
	if(p_root->heightLeft > p_root->heightRight)
	{
	    retVal = p_root->heightLeft;
	}
	else
	{
	    retVal = p_root->heightRight;
	}
    }

    return retVal;

}

//percentLeaf
template <class T>
double BSTree <T> :: percentLeaf()
{
    double percent;
    double numLeaf = (double) getLeaves();
    
    percent = numLeaf/((double) numElements);

    return percent;
}

//percentInterior
template <class T>
double BSTree <T> :: percentInterior()
{
    double percent;
    double numLeaf = (double) getLeaves();
    double total = (double) numElements;

    percent = (total-numLeaf)/total;

    return percent;
}

//getLeaves
template <class T>
int BSTree <T> :: getLeaves()
{
    //getLeaves traversal starting at root

    int numLeaf = 0;

    int height = heightOfTree();

    for(int level = 1; level <= height + 1; level++)
    {
	int currentLevel = 0;

	getLeavesHelper(level, currentLevel, numLeaf, p_root);
    }

    return numLeaf;

}

//getLeavesHelper
template <class T>
void BSTree <T> :: getLeavesHelper(int desiredLevel, int currentLevel, int& numLeaf, Node* p_node)
{
    currentLevel++;

    if(desiredLevel == currentLevel)
    {
	if(p_node != 0)
	{
	    if(p_node->p_right == 0 &&
	       p_node->p_left == 0)
	    {
		numLeaf++;
	    }

	}
    }
    else
    {	
	if(p_node != 0)
	{
	    getLeavesHelper(desiredLevel, currentLevel, numLeaf, p_node->p_left);
	    getLeavesHelper(desiredLevel, currentLevel, numLeaf, p_node->p_right);

	}
    }    

}

//printInTreeForm
template <class T>
void BSTree <T> :: printInTreeForm()
{
    topBottom();
}

//topBottom
template <class T>
int BSTree <T> :: topBottom()
{
    //topBottom traversal starting at root
 
    if(p_root == 0)
    {
	cout << "Los sientos hombre the tree be empty" << endl;

    }

    int height = heightOfTree();
    double leftPadding = 0;

    for(int level = 1; level <= height + 1; level++)
    {
	int currentLevel = 0;
	
	leftPadding = ((pow(2,height))*2.5)/(pow(2,level-1));

	cout << endl;
	if(level != height + 1)
	{
	    //padding
	    for(int i = 0; i < (int) leftPadding; i++)
	    {
		cout << " ";
	    }
	}

	topBottomHelper(level, currentLevel, p_root, leftPadding);
    }

}

//topBottomHelper
template <class T>
void BSTree <T> :: topBottomHelper(int desiredLevel, int currentLevel, Node* p_node, double padding)
{
    currentLevel++;

    //double rightPadding = pow(2, heightOfTree() - currentLevel -1);

    //base case, node is null
    if(p_node == 0)
    {
	//start returning
	cout << "XX";

	for(int i = 0; i < padding; i++)
	{
	    cout << " ";
	}
    }
    //if node is not null and at desired level
    else if(desiredLevel == currentLevel)
    {
	if(p_node != 0)
	{
	    cout << p_node->heightLeft << ":" << p_node->data << ":" <<  p_node->heightRight;
	}

	for(int i = 0; i < padding; i++)
	{
	    cout << " ";
	}
    }
    else
    {	
	topBottomHelper(desiredLevel, currentLevel, p_node->p_left, padding);
	topBottomHelper(desiredLevel, currentLevel, p_node->p_right, padding);
    }    
}

//bottomTop
template <class T>
int BSTree <T> :: bottomTop()
{
    //getLeaves traversal starting at root

    int height = heightOfTree();

    for(int level = height; level >= 0; level--)
    {
	int currentLevel = -1;

	bottomTopHelper(level, currentLevel, p_root);
    }

}

//bottomTopHelper
template <class T>
void BSTree <T> :: bottomTopHelper(int desiredLevel, int currentLevel, Node* p_node)
{
    currentLevel++;

    if(desiredLevel == currentLevel)
    {
	if(p_node != 0)
	{
	    cout << p_node->data << " ";
	}
    }
    else
    {	
	bottomTopHelper(desiredLevel, currentLevel, p_node->p_left);
	bottomTopHelper(desiredLevel, currentLevel, p_node->p_right);
	cout << endl;
    }    

}

//balanceRight
template <class T>
void BSTree <T> :: balanceRight(Node*& p_node)
{
    cout << "Going to balanceRight now" << endl;

    //passed in node p_node is current node

    //mark p_node's right as the new root of the subtree
    Node* p_rootSub = p_node->p_right;

    //copy the new root's left pointer
    Node* p_temp = p_rootSub->p_left;

    //set the new root's left pointer to be p_curr
    p_rootSub->p_left = p_node;

    //set p_curr's right to be the new root's left
    p_node->p_right = p_temp;

    //set the p_curr to point to the new root
    p_node = p_rootSub;

}

//balanceLeft
template <class T>
void BSTree <T> :: balanceLeft(Node*& p_node)
{
    cout << "Going to balanceLeft now" << endl;

    //passed in node p_node is current node
    
    //mark p_node's right as the new root of the subtree
    Node* p_rootSub = p_node->p_left;

    //copy the new root's left pointer
    Node* p_temp = p_rootSub->p_right;

    //set the new root's left pointer to be p_curr
    p_rootSub->p_right = p_node;

    //set p_curr's right to be the new root's left
    p_node->p_left = p_temp;

    //set the p_curr to point to the new root
    p_node = p_rootSub;

}

//findHeights()
template <class T>
void BSTree <T> :: findHeights()
{
    int heightCounter = 0;
    //start at root node
    findHeightsHelper(p_root, heightCounter);
}

//findHeightsHelper()
template <class T>
void BSTree <T> :: findHeightsHelper(Node*& p_node, int& heightCounter)
{
    //p_node is null is base case
    if(p_node == 0)
    {
	//want to reset heightCounter
	heightCounter = -1;
    }
    else
    {
	//look left
	findHeightsHelper(p_node->p_left, heightCounter);
	heightCounter++;
	p_node->heightLeft = heightCounter;
	//look right
	findHeightsHelper(p_node->p_right, heightCounter);
	heightCounter++;
	p_node->heightRight = heightCounter;
	
	//if leftHeight is larger than right
	if(p_node->heightLeft > p_node->heightRight)
	{
	    heightCounter = p_node->heightLeft;
	}
    }

}
