#include <iostream>
#include <fstream>
#include <string>

using namespace std;

template<typename T>
class Tree 
{
private:
	//a struct node which defines a node of the tree. It contains some data, a pointer to the left child and a pointer to the right child
	struct Node {
		T data;
		Node* leftChild;
		Node* rightChild;

		//constructor
		Node(T data) {
			this->data = data;
			leftChild = nullptr;
			rightChild = nullptr;
		}
	};

	Node* root; //root node of the tree
	size_t size; //side of the tree (total number of nodes)

	//insert method which recursively inserts a new node in the tree
	void insert(Node*& node, T data) {
		
		//if tree (the root node) is empty, then it initializes the node
		if (node == nullptr) {
			node = new Node(data);
		}

		//if the current node's data is greater than the incoming data, then it should be added to the left side of the tree
		else if (data.compare(node->data) < 0) {
			insert(node->leftChild, data);
		}

		//if the current node's data is less than the incoming data, then it should be added to the right side of the tree
		else if (data.compare(node->data) > 0) {
			insert(node->rightChild, data);
		}
	}

	//method to deallocate the memory. THis method is called by the destructor
	//it recursively deletes the left child and the right child at every level
	void deallocate(Node* node) {
		if (node != nullptr) {
			deallocate(node->leftChild);
			deallocate(node->rightChild);
			delete node;
		}
	}

public:

	//public constructor. It is called when a tree is initialized
	Tree() {
		root = nullptr;
		size = 0;
	}

	//method to insert, this calls the recursive method which inserts the node at the appropriate location
	void insert(T data) {
		insert(root, data);
		size++;
	}

	//method that finds the word in the tree.
	//this compares the data of the tree nodes with the argument and also calculates the compare counts.
	//in case the word is found in the tree, the compare count is positive.
	//in the case it is not found, the negation of the compare count is returned
	int find(T word) 
	{
		Node* node = root;
		int compareCount = 0;

		while (node != nullptr) {
			compareCount++;
			
			//in case of word found
			if (word.compare(node->data) == 0) 
				return compareCount;
			
			//to traverse the left child of the current node
			else if (word.compare(node->data) < 0) 
				node = node->leftChild;

			//to traverse the right child of the current node
			else 
				node = node->rightChild;
			
		}

		//return the negation if the word is not found
		return -compareCount;
	}

	//method to remove a node from the tree
	bool remove(Node* nodeToRemove) 
	{
		if (nodeToRemove == nullptr) {
			// the word wasn't found in the tree
			return false;
		}

		// Case 1: The node has no children
		if (nodeToRemove->leftChild == nullptr && nodeToRemove->rightChild == nullptr) {
			if (nodeToRemove == root) {
				root = nullptr;
			}
			else {
				Node* parent = nodeToRemove->parent;
				if (parent->leftChild == nodeToRemove) {
					parent->leftChild = nullptr;
				}
				else {
					parent->rightChild = nullptr;
				}
			}
			delete nodeToRemove;
		}

		// Case 2: The node has one child
		else if (nodeToRemove->leftChild == nullptr || nodeToRemove->rightChild == nullptr) {
			Node* child = (nodeToRemove->leftChild != nullptr) ? nodeToRemove->leftChild : nodeToRemove->rightChild;

			if (nodeToRemove == root) {
				root = child;
				child->parent = nullptr;
			}
			else {
				Node* parent = nodeToRemove->parent;
				if (parent->leftChild == nodeToRemove) {
					parent->leftChild = child;
				}
				else {
					parent->rightChild = child;
				}
				child->parent = parent;
			}
			delete nodeToRemove;
		}

		// Case 3: The node has two children
		else {
			// Find the successor node (i.e., the node with the smallest value in the right subtree)
			Node* successor = nodeToRemove->rightChild;
			while (successor->leftChild != nullptr) {
				successor = successor->leftChild;
			}

			// Copy the successor's data into the node to remove
			nodeToRemove->word = successor->word;

			// Recursively remove the successor node (which will be in case 1 or case 2)
			remove(successor->word);
		}

		// Update the size of the tree
		size--;

		return true;
	}

	//returns the size of the tree
	int getSize() {
		return size;
	}

	//returns if the tree is empty
	bool isEmpty() {
		return root == nullptr;
	}

	//destructor
	~Tree()
	{
		deallocate(root);
	}
};

