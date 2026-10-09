#pragma once

template <class T>
class Queue
{
private:
	int capacity;
	int front;
	int rear;
	T* queue;

public:
	Queue(int queueCapacity = 10); //initailzer인데, 값이 아무것도 없는 상태로 들어오면 기본적으로 queueCapacity = 10으로 둠
	//Definition파트에서 private member capacity에 queueCapacity를 넣음으로써 기본 사이즈 정의 완료.
	//여기서는 크기가 자유롭게 변경되는 queue에 대해서 다루고 있다.

	bool isEmpty() const { return front == rear; };
	//비어있는가?
	//inline으로 해도 될 정도로 코드가 bulky하지 않다
	T& Front() const {
		if (isEmpty()) {
			throw "Queue가 비어있습니다!";
		}
		return queue[front];
	};
	//가장 앞에 있는 요소
	T& Rear() const {
		if (isEmpty()) {
			throw "Queue가 비어있습니다!";
		}
		return queue[(rear + capacity - 1) % capacity];
	};
	//가장 뒤에 있는 요소
	void Push(const T& item);
	//Front에 추가
	void Pop();
	//FIFO에 의해 가장 앞에 있는 값이 삭제
	~Queue() {
		delete[] queue;
	}
};

#include "queue.cpp"