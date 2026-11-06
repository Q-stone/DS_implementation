#pragma once
#include "linked_list.h"

struct item_data {
	const char* itemId;
	int price;
};
class bpt;

class bpt_node
{
	friend bpt;
protected:
	//2-3 tree 꼴이므로
	int key[3] = { 0 };
	int count = 0;
	bpt_node* parent = nullptr;
public:
	virtual ~bpt_node() {

	}
	virtual bool isLeaf() const = 0;
};

class bpt_index : public bpt_node {
	//일반적인 2-3 tree node.
	friend class bpt;
private:
	bpt_node* child[4] = {};
public:
	bool isLeaf() const override {
		return false;
	}
	~bpt_index() {
		for (int i = 0; i <= count; i++) {
			delete child[i];
		}
	}
};

class bpt_data : public bpt_node {
	//leaf nodes. bidirectional linked list가 있는 부분.
	friend class bpt;
private:
	List<item_data*>* data[3] = {};
	bpt_data* before = nullptr;
	bpt_data* next = nullptr;
public:
	bool isLeaf() const override {
		return true;
		//leaf node니깐.
	}
	~bpt_data() {
		for (int i = 0; i < count; i++) {
			delete data[i];
		}
	}
};

class bpt {
	bpt_node* root = nullptr;
	bpt_data* search(int value) const;
	void split(bpt_node* ndoe);
public:
	bpt() {

	}
	~bpt() {
		delete root;
	}
	void Insert(int value, item_data* item);
	void Delete(int value, item_data* item);
	void Print(int start, int end) const;
};