#pragma once
//김규석 qstone@kw.ac.kr
//lecture2의 single direction linked list 골조를 이용해 양방향 리스트 구현
enum Boolean {FALSE, TRUE};
template<class T> class List;
template<class T> class ListIterator;

template <class T> class Listnode {
	friend class List<T>;//globally share를 위한 friend keyword
	friend class ListIterator<T>;
private:
	Listnode(const T& value) : data(value), next(nullptr), before(nullptr) {}
	T data;
	Listnode* next;//next node
	Listnode* before;
};

template <class T> class List {
	friend class ListIterator <T>;
private:
	Listnode <T>* first; //List의 처음을 지칭
public:
	List() {
		first = 0;//ptr = 0 -> nullptr
	}
	~List() {
		while (first != nullptr) {
			Listnode<T>* current = first;
			first = first->next;
			delete current;
		}
	}
	void Insert_first(const T& value);
	void Insert(const T& location, const T& value);
	void Delete(const T& value);
};

template <class T> class ListIterator {
private:
	const List<T>& list;
	Listnode <T>* current;
public:
	ListIterator(const List <T>& l) : list(l), current(l.first) {
	}
	Boolean NotNull();
	Boolean NextNotNull();
	Boolean BeforeNotNull();
	T* First();
	T* Next();
	T* Before();
};

template <class T>
Boolean ListIterator<T> :: NotNull() {
	if (current) {
		return TRUE;
	}
	return FALSE;
}

template <class T>
Boolean ListIterator <T> ::NextNotNull() {
	if (current && current->next) {
		return TRUE;
	}
	return FALSE;
}

template <class T>
Boolean ListIterator <T> ::BeforeNotNull() {
	if (current && current->before) {
		return TRUE;
	}
	return FALSE;
}

template <class T>
T* ListIterator <T> ::First() { //Template 자료형이므로 data가 returned
	if (list.first) {
		return &list.first->data;
	}
	else {
		return nullptr;
	}
}
template <class T>
T* ListIterator <T> ::Next() { //Template 자료형이므로 data가 returned
	if (current) {
		current = current->next;
		return current ? &current->data : nullptr;
	}
	else {
		return nullptr;
	}
}
template <class T>
T* ListIterator <T> ::Before() { //Template 자료형이므로 data가 returned
	if (current) {
		current = current->before;
		return current ? &current->data : nullptr;
	}
	else {
		return nullptr;
	}
}

template <class T>
void List <T> ::Insert_first(const T& value) {//노드가 아무것도 없을 때 first insertion
	Listnode<T>* current = first;
	if (current == nullptr) {
		Listnode<T>* node = new Listnode<T>(value);
		first = node;
	}
}

template <class T>
void List <T> ::Insert(const T& location, const T& value) {//노드가 존재할 때 특정 위치에서의 삽입을 지원하는 insertion
	Listnode<T>* current = first;
	while (current != nullptr) {
		if (current->data == location) {
			break;
		}
		current = current->next;
	}
	if (current == nullptr) {
		throw "노드가 비어있습니다! 처음 노드를 생성하십시오";
	}
	Listnode<T>* node = new Listnode<T>(value);

	node->before = current;
	node->next = current->next;
	//current의 다음에 추가하는 것이기에 before는 current가 되고, current가 지칭하던 next가 추가할 노드의 next가 된다.
	//이때 current의 next는 이 노드로 해야하고, next의 before도 이 노드로 해야함.
	if (node->next != nullptr) {
		node->next->before = node;
	}
	current->next = node;
	//return TRUE;
}

template <class T>
void List <T> :: Delete(const T& value) {
	Listnode<T>* current = first;
	while (current != nullptr) {
		if (current->data == value) {
			break;
		}
		current = current->next;
	}
	if (current == nullptr) {
		return;
	}
	if (current->before != nullptr) {
		current->before->next = current->next;
	}
	else {
		first = current->next; //고른 노드의 이전이 없다면 노드 다음번이 first node가 되므로.
	}
	if (current->next != nullptr) {
		current->next->before = current->before;
	}
	delete current;
	//return TRUE;
}