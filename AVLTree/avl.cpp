#include "avl.h"
#include <iostream>

int avl::height(const Node* p) const {
	return p ? p->height : 0; //p가 nullptr이 아니라면 height 출력
}

void avl::update(Node* p) {
	int left_height = height(p->left);
	int right_height = height(p->right);

	p->height = 1 + (left_height > right_height ? left_height : right_height);//노드 시점에서 좌우 중 긴 height를 가지도록 함.
	p->balance_factor = left_height - right_height;
}

Node* avl::rr(Node* p) {//rotate right
//	         30 <-p
//          /  |
// child-> 20  40
//        /  |
//      10   25
//     /
//    5
	//움직이려는 노드 기준으로 좌측 아래 자식이 부모가 된다.
	Node* child = p->left;
	p->left = child->right;
	child->right = p;
	
	update(p);//선택 노드 update
	update(child);

	return child;//child가 선택 노드가 되기 때문에
}

Node* avl::rl(Node* p) {//rotate left, rr 정반대로 생각하면 됨
	Node* child = p->right;
	p->right = child->left;
	child->left = p;

	update(p);
	update(child);

	return child;
}

Node* avl::insert(Node* p, int key) {
	if (!p) {
		return new Node(key);//param init
	}
	if (key < p->key) {
		//삽입 노드의 key보다 선택노드의 키가 크다면 left. (이때 재귀 처리해서 상위에서 하위로)
		p->left = insert(p->left, key);
	}
	else if (key > p->key) {
		p->right = insert(p->right, key); //right. (반대)
	}
	else {
		return p; //duplicate 되는 경우는 배제하자.
	}

	update(p);

	if (p->balance_factor > 1) {
		//p의 bf가 2일 때, p의 left child의 bf가 음수를 가진다면(-1)
		//bf = L - R임을 감안하면 LR임이 자명하다.
		//따라서 LR -> RR꼴로 만들어주기 위해 rotate left를 거친다.
		if (p->left->balance_factor < 0) {
			p->left = rl(p->left);
		}
		return rr(p);//p에 대한 rotate right -> LL(왼쪽에서 오른쪽으로)
	}
	else if (p->balance_factor < -1) {
		if (p->right->balance_factor > 0) {
			p->right = rr(p->right);
		}
		return rl(p);
	}
	return p;
	//selected된 node를 ret
}

void avl::insert(int key) {
	root = insert(root, key);//만약 node가 nullptr인 상태라면 param이 2개로 overloading된 함수로 가서 allocate된다.
}

bool avl::search(int key) const {
	const Node* current = root;
	while (current) {//탐색은 bst 처럼.
		if (key < current -> key) {
			current = current->left;
		}
		else if (key > current -> key) {
			current = current->right;
		}
		else {
			return true;//found!
		}
	}
	return false;
}

void avl::inorder() {
	//morris 중위 순회
	//커서가 20을 향한다고 해보자
	//    20
	//   /  |
	//  10  30
	//일때 10의 right child로 20을 연결한다.
	//이후 본래 20의 left child였던 10부터 right -> right ->... 진행하면 전체 중위 순회가 가능하다
	//확장해서 보면
	//	         30
	//          / |
	//        20  40
	//       / |
	//     10  25
	//     /
	//    5
	// root에서 left로 내려가면서 right node를 계속 left child의 right node로 이어붙인다고 생각하면 된다.
	// 따라서 최종적으로 5 10 20 25 30 40이 5의 right child 부터 쭉 이어져 있을 것이다.
	Node* current = root;
	while (current != nullptr) {
		if (current->left == nullptr) {
			std::cout << current->key << ' ';
			current = current->right;
		}
		else {
			Node* before = current->left;
			while (before->right != nullptr && before->right != current) {
				before = before->right;
			}
			if (before->right == nullptr) {
				before->right = current;
				current = current->left;
			}
			else {
				//임시적으로 만들어진 right path를 삭제
				before->right = nullptr;
				std::cout << current->key << ' ';
				current = current->right;
			}
		}
	}
	std::cout << '\n';
}