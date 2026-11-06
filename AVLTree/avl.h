#pragma once

class Node {
	friend class avl;

private:
	int key;
	int height = 1;
	int balance_factor = 0;

	Node* left = nullptr;
	Node* right = nullptr;

public:
	explicit Node(int value) : key(value) {}//node가 한개도 없다면 parameter init으로

	~Node() {
		delete left;
		delete right;
	}
};

class avl
{
private:
	Node* root = nullptr;

	int height(const Node* p) const;
	void update(Node* p);

	Node* rr(Node* p);
	Node* rl(Node* p);

	Node* insert(Node* p, int key);

public:
	avl() {}
	~avl() {
		delete root;
	}
	void insert(int key);
	bool search(int key) const;
	void inorder() ;
	bool isEmpty() const {
		return root == nullptr;
	}
};

