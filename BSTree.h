#pragma once
#include <iostream>
#include "BSTreeExceptions.h"

using namespace std;

template <class T>
class BSTree
{
 public:
    BSTree();

    void insert(const T& elem);
    bool remove(const T& elem);
    
    bool search(const T& elem);

    void printInOrder();
    void printPostOrder();
    void printPreOrder();

    int length();

    T findMax();
    T findMin();

 private:
    
    struct Node
    {
	Node* p_left;
	T data;
	Node* p_right;
    };

    Node* p_root;
    int numElements;    

    void insertHelper(Node*& p_node, const T& elem);

    void removeHelper(Node*& p_node, const T& elem, bool& found);
    void deleteNode(Node*& p_node);
    void getPredecessor(Node* p_node, T& elem);
    
    void searchHelper(Node* p_node, const T& elem, bool& found);

    void InOrderHelper(Node* p_node);
    void PostOrderHelper(Node* p_node);
    void PreOrderHelper(Node* p_node);

    void findMaxHelper(Node* p_node, T& elem);
    void findMinHelper(Node* p_node, T& elem);
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
	BSTreeDuplicateException error;
	throw error;
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
    }
    else if(elem <= p_node->data)
    {
	//if data is less than or equal to current node data,
	//recursively go left
	insertHelper(p_node->p_left, elem);	
    }
    else
    {
	//if data is greater than current node data
	//recursively go right
	insertHelper(p_node->p_right, elem);
    }

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
    }
    else if(elem > p_node->data)
    {
	//need to keep searching right
	removeHelper(p_node->p_right, elem, found);
    }
    else
    {
	//node has been found
	deleteNode(p_node);
	found = true;
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
void BSTree <T> :: findMaxHelper(Node* p_node, T& elem)
{
    //base case
    if(p_node == 0)
    {
	//start returning...
    }
    else
    {
	elem = p_node->data;
	findMaxHelper(p_node->p_right);
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
