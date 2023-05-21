#pragma once

#include "Node/Node.h"

class BSTree
{
private:
	Node* root;

	// Utility Functions - Recursive Functions --------------------------

	// Given functions
	static void rec_insertBST(Node*& subRoot, int key);
	static void rec_inorder_traverse(Node* subRoot);
	static void rec_preorder_traverse(Node* subRoot);
	static void rec_postorder_traverse(Node* subRoot);
	static void rec_destroy_tree(Node*& subRoot);

	//Functions to be implemented by student
	static Node* rec_find_max(Node* subRoot);
	static Node* rec_find_min(Node* subRoot);
	static int rec_max_depth(Node* subRoot);

public:
	BSTree(void);
	~BSTree(void);

	// Given functions
	void insertBST(int key); // not const
	void inorder_traverse() const;
	void preorder_traverse() const;
	void postorder_traverse() const;
	void destroy_tree(); // not const.

	// Functions to be implemented by student
	Node* find_max() const; // return a pointer to the node that hold the maximum value in binary search tree.
	Node* find_min() const; // return a pointer to the node that hold the minimum value in binary search tree.
	int max_depth() const;

};

