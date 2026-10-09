#pragma once
#include <iostream>
//2023202040 qstone@kw.ac.kr
//이진 탐색 트리의 각 노드에 해당하는 클래스

//bs_tree 전방 선언 후 friend.
template <class A, class B>
class bs_tree;

template <class A, class B>
class bst_node {
	friend class bs_tree<A, B>;
	//below under the line is private members. 명시하지 X.

	A key;//key값이 string인지 integer인지 모름!
	B data; //data 값이 string인지 integer인지 모름!

	bst_node<A, B>* left = nullptr;//inherit 초기화
	bst_node<A, B>* right = nullptr;

};

//이진 탐색 트리
template <class A, class B>
class bs_tree {
private:
	bst_node <A, B>* root = nullptr;//root node
	void inorder(const bst_node<A, B>* current) const;//오버로딩
	void clear(bst_node<A, B>* current);//오버로딩
public:
	bs_tree() {

	};
	~bs_tree() {
		clear();
	}

	bool isEmpty() const {
		return root == nullptr; //root == null이면 empty
	}
	const bst_node <A, B>* get_node(const A& key);
	void insert(const A& key, const B& value);
	bool delete_node(const A& key, const B& value);
	void inorder() const;
	void clear();
};

template <class A, class B>
const bst_node <A, B>* bs_tree <A, B> ::get_node(const A& key) {//key에 따른 노드 가져오기. const ref로 복사비용최소
	const bst_node <A, B>* current = root;
	//이 밑에는 사실상 linked list 순회하는 것 마냥 하면 됨. 거의 똑같음
	while (current != nullptr) {
		if (key < current->key) {//current key보다 찾으려는 노드의 key가 더 작으면 left로 탐색 시도
			current = current->left;
		}
		else if (key > current->key) {//찾으려는 노드의 key가 더 크면 right로 탐색
			current = current->right;
		}
		else {//현재 current의 key가 찾으려는 노드의 key와 동일!
			return current;
		}
	}
	//노드의 가장 아래까지 가면 nullptr이므로 while 끝. -> 데이터 없음
	return nullptr;
}

template <class A, class B>
void bs_tree <A, B> :: insert(const A& key, const B& value) {
	//삽입할 때 부모의 key보다 작으면 좌측으로, 크면 우측으로 이동
	//만약 current가 없다 (nullptr) -> 그러면 삽입.
	bst_node<A, B>* current = root;
	bst_node<A, B>* parent = nullptr;

	while (current != nullptr) {
		parent = current;
		if (current->key > key) {//삽입 노드의 key가 더 작으므로 왼쪽으로
			current = current->left;
		} else if (current->key < key) {// 더 크므로 오른쪽으로
			current = current->right;
		} else {
			//동일하면 파사삭
			throw "동일한 key value를 가진 노드를 추가하고자 합니다.";
			//과제에서는 동일한 경우 1회 추가한다고 했으니 따로 추가하는 멤버 펑션을 만들어서 실행시키면 된다.
		}
	}
	//최종 key == nullptr인 지점까지 왔으므로 여기에 데이터를 추가한다.
	bst_node<A, B>* node = new bst_node<A, B>;
	node->key = key;
	node->data = value;
	if (parent == nullptr) {
		root = node;
	} else if (key < parent->key) {//부모의 자식 노드 지칭을 결정.
		parent->left = node;
	} else {
		parent->right = node;
	}
}

template <class A, class B>
bool bs_tree <A, B> :: delete_node(const A& key, const B& value) {
	//key와 value 일치를 동시에 확인해서 삭제하는 함수가 되겠다.
	//만약 어느 한쪽만 일치가 되어야 한다면 value를 넣을 때 |를 넣는다던가 아니면 절대 넣을 수 없는 값을 넣으면 되겠지.
	bst_node <A, B>* current = root;
	bst_node <A, B>* parent = nullptr;
	while (current != nullptr && current->key != key) {
		//동일한 경우 break. 못찾으면 current = nullptr이니깐 펑
		parent = current;
		if (key < current->key) {
			current = current->left;
		}
		else {
			current = current->right;
		}
	}
	if (current == nullptr || current->data != value) {//두 요소의 일치를 모두 확인
		return false;
	}
	//삭제할 노드 기준으로 자식이 1개인 경우와 2개인 경우를 나누어야 한다.
	//먼저 자식이 모두 nullptr이 아닌 경우
	//현재 current는 지우고자 하는 노드이다.
	if (current->left != nullptr && current->right != nullptr) {
		bst_node<A, B>* parent_succesor = current;
		bst_node<A, B>* successor = current -> right;
		while (successor->left != nullptr) {
			parent_succesor = successor;
			successor = successor->left; //가장 작은 우측 노드의 값을 찾자. -> left 탐지
		}
		//이떄 successor는 left node가 없다. 즉, 우측 자식이 있거나 아예 없는 경우인 것이다.
		//successor와 current의 값을 바꾼다고 하였을 때 
		current->key = successor->key;//현재 노드의 키 값과 데이터를 계승자의 것으로 바꿈
		current->data = successor->data;

		current = successor;//그리고 지우고자 한 노드를 successor의 것으로 변경하자. 그 이유는 진짜 current 값은 이미 successor의 것으로 갈아끼워졌으니 successor를 처리하고자 하는 것.
		parent = parent_succesor;//parent를 successor의 parent로 바꾸자.
	}
	bst_node<A, B>* child;
	if (current->left != nullptr) {//즉 successor의 좌측 노드는 없을텐데 저절로 right 노드가 child로 잡힌다.
		child = current->left;
	}
	else {
		child = current->right;
	}

	if (parent == nullptr) {
		root = child;
	}
	else if (parent->left == current) {//현재 간선을 생각해보자. 여전히 successor의 부모노드는 successor를 가리키고 있는 기이한 구조이다.
		parent->left = child;//바로 좌측 노드로 이어주자.
	}
	else {
		parent->right = child;//후계자가 바로 오른쪽 자식인 경우에.
	}
	delete current; //실제로는 successor가 사라진 것이다.

	return true;
}

//외부 호출 함수: 루트부터
template<class A, class B>
void bs_tree<A, B>::inorder() const {
	inorder(root);
}

//실제 재귀 순회
template<class A, class B>
void bs_tree<A, B>::inorder(const bst_node<A, B>* current) const {
	if (current == nullptr) {
		return;
	}
	inorder(current->left);//왼쪽 전체 처리
	std::cout << current->key << " : " << current->data << '\n';
	inorder(current->right);//오른쪽 전체 처리
}

//외부 호출 함수: 전체 트리 삭제
template<class A, class B>
void bs_tree<A, B>::clear() {
	clear(root);
	root = nullptr; //삭제가 끝났으므로 빈 트리로 설정
}

//실제 재귀 삭제
template<class A, class B>
void bs_tree<A, B>::clear(bst_node<A, B>* current) {
	if (current == nullptr) {
		return;
	}
	clear(current->left);//왼 서브트리 삭제
	clear(current->right);//오른 서브트리 삭제

	delete current;//자식 처리 끝 -> 본인 삭제
}