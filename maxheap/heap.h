#pragma once
template <class T>
class max_heap
{
private:
	T* heap;
	int heapsize;
	int capacity;
public:
	virtual ~max_heap() {
		delete[] heap;//[]가 적용되는 상황인가?
	}
	max_heap(int capacity = 10);
	max_heap(const max_heap&) = delete;
	max_heap& operator=(const max_heap&) = delete;
	virtual bool isEmpty() const = 0;
	virtual const T& top() const = 0;
	virtual void push(const T&) = 0;
	virtual void pop() = 0;
};

#include "heap.cpp"
