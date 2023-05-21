#include "BSTree.h"
#include <iostream>
using namespace std;
void BSTree::rec_insertBST(Node*& subRoot, int key)
{
	if (subRoot == nullptr)
		subRoot = new Node(key);
	else if (subRoot->getdata() < key)
	{
		rec_insertBST(subRoot->getright(), key);
	}
	else
	{
		// recursive call on left
		rec_insertBST(subRoot->getleft(), key);
	}
}

//* print the tree with inorder traversal.
void BSTree::rec_inorder_traverse(Node* subRoot)
{
	if (subRoot == nullptr) return;
	rec_inorder_traverse(subRoot->getleft());
	cout << subRoot->getdata() << " ";
	rec_inorder_traverse(subRoot->getright());
}

//* print the tree with preorder traversal.
void BSTree::rec_preorder_traverse(Node* subRoot)
{

	if (subRoot == nullptr) return;
	cout << subRoot->getdata() << "  ";
	rec_preorder_traverse(subRoot->getleft());
	rec_preorder_traverse(subRoot->getright());
}

//* print the tree with postorder traversal.
void BSTree::rec_postorder_traverse(Node* subRoot)
{
	if (subRoot == nullptr) return;
	rec_postorder_traverse(subRoot->getleft());
	rec_postorder_traverse(subRoot->getright());
	cout << subRoot->getdata() << "  ";
}

void BSTree::rec_destroy_tree(Node*& subRoot)
{
	if (subRoot != nullptr)
	{
		// recursive call on left
		rec_destroy_tree(subRoot->getleft());

		// recursive call on right
		rec_destroy_tree(subRoot->getright());

		delete subRoot;
		subRoot = nullptr; // this line is why we pass subRoot by reference (subRoot = something)
	}
}
Node* BSTree::rec_find_max(Node* subRoot)
{
	//TO DO
	return nullptr; // should be changed
}
//* return a pointer to the node that hold the minimum value in binary search tree of the passed subRoot.
Node* BSTree::rec_find_min(Node* subRoot)
{
	//TO DO
	return nullptr; // should be changed
}
//* return the depth of binary search tree of the passed subRoot.
int BSTree::rec_max_depth(Node* subRoot)
{
	//TO DO
	return 0; // should be changed
}


BSTree::BSTree()
{
	root = nullptr;
}


BSTree::~BSTree()
{
	destroy_tree();
}

// Public Functions ---------------------------------

// Given functions
void BSTree::insertBST(int key) // not const
{
	// not const because rec_insertBST takes the root by reference, so may change it
	rec_insertBST(root, key);
}
void BSTree::inorder_traverse() const
{
	rec_inorder_traverse(root);
}
void BSTree::preorder_traverse() const
{
	rec_preorder_traverse(root);
}
void BSTree::postorder_traverse() const
{
	rec_postorder_traverse(root);
}
void BSTree::destroy_tree() // not const
{
	// not const because rec_destroy_tree takes the root by reference, so may change it
	rec_destroy_tree(root);
}

// Functions to be implemented by student
Node* BSTree::find_max() const // return a pointer to the node that hold the maximum value in binary search tree.
{
	//TO DO
	return nullptr; // should be changed
}
Node* BSTree::find_min() const // return a pointer to the node that hold the minimum value in binary search tree.
{
	//TO DO
	return nullptr; // should be changed
}
int BSTree::max_depth() const
{
	//TO DO
	return 0; // should be changed
}
