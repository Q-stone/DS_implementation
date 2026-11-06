#pragma once
#include "heap.h"

template <class T>
max_heap<T>::max_heap(int capacity) {
	if (capacity < 1) {
		throw "Capacity must be greater or equal to 1";
	}
	this->capacity = capacity;
	heapsize = 0;
	heap = new T[capacity + 1];
}

template <class T>
void max_heap<T>::push(const T& element) {
	T value = element;
	if (heapsize == capacity) {//doubler
		T* new_heap = new T[2 * capacity + 1];
		try {
			for (int i = 1; i <= heapsize; i++) {
				new_heap[i] = heap[i];
			}
		} catch (...) {//혹여 heap 생성 중 튕기면 delete.
			delete[] new_heap;
			throw;
		}
		delete[] heap;
		heap = new_heap;
		capacity *= 2;
	}
	int currentNode = ++heapsize;//노드는 하위 level(수적으로는 increment)의 좌->우 방면으로 추가된다.
	//단 array에서는 이를 고려하지 않고 heapsize가 가장 하위 node를 가리키고, bst의 array에서 표현과 같이 /2가 parent node가 된다.
	while (currentNode != 1 && heap[currentNode / 2] < value) {
		//max_heap이므로 부모노드는 항상 자식노드보다 커야한다. 만약 자식노드가 더 크다면, 가장 하위의 자식 노드가 부모노드가 되고.. 계속 하여 이를 반복한다.
		heap[currentNode] = heap[currentNode / 2];
		currentNode /= 2;
		//즉 34 -> 17 -> 8 -> 4 -> 2 -> 1 꼴로 insertion은 O(logN)에 수렴한다.
	}
	heap[currentNode] = value;
	//자식노드가 부모노드보다 더 작은 상태라면 해당 위치에 놓이면 그만이다.
}

template <class T>
bool max_heap<T>::isEmpty() const {
	return heapsize == 0 ? true : false;
}

template <class T>
void max_heap<T>::pop() {
	if (isEmpty()) {
		throw "The heap is empty. deletion is not allowed at this time.";
	}
	//heap[1].~T(); 필요치 않다. 어차피 currentNode는 변경되므로  더더욱 그러하다.
	T last_element = heap[heapsize--];//last element of max heap. last element와 동일한 레벨의 노드들은 서로 비교관계에 있어 뚜렷한 우위 관계가 없다.
	int currentNode = 1; //root
	int child = 2; // root의 자식 중에서 존재할 수 있는 가장 우선의 경우.
	while (child <= heapsize) {
		if (child < heapsize && heap[child] < heap[child + 1]) {
			//비교할 때 child와 child + 1 노드는 무조건 경쟁 상대이다.
			child++;//만약 child + 1 노드가 더 크다면, child++로 우선 선택을 갱신한다.
		}
		if (last_element >= heap[child]) {
			break;
		}
		heap[currentNode] = heap[child]; //부모 노드의 자식 노드간 비교에서 큰 노드는 부모노드가 된다. 처음에 루트가 deletion 되었을 때 자식 노드는 root로.
		currentNode = child;//이후 current node를 child 노드로 갱신해서 점점 depth를 깊이 두며 탐색을 진행한다.
		child *= 2; //부모 노드 기준 왼쪽 자식 노드는 * 2에 해당하기에.
	}
	heap[currentNode] = last_element;
	//최종적으로 last_element는 빈 자리에 메꿔지게 된다. 그 이유는 complete binary tree 구조를 띄어야 하기 때문이다.
}

template <class T>
const T& max_heap<T>::top() const {
	if (isEmpty()) {
		throw "The heap is empty. top is not available at this time.";
	}
	return heap[1];
}