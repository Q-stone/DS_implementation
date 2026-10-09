#pragma once
#include "queue.h"

template <class T>
Queue<T>::Queue(int Capacity) : capacity(Capacity)//capacity = Capacity로 해서 capacity 멤버를 init.
{
	if (capacity < 1) {
		throw "Queue의 용량은 0보다 커야합니다!";
	}
	queue = new T[capacity];
	front = 0;
	rear = 0;
	//front, rear 모두 0인 이유는 아무 데이터가 없기 때문.
	//queue에서 데이터가 없음을 증명할 때에는 front == rear만 확인하면 된다.
}
template <class T>
void Queue<T>::Push(const T& item) {
	if ((rear + 1) % capacity == front) {
		T* newqueue = new T[2 * capacity];
		
		int start_point = front % capacity;
		//push할 때 어떤 값을 사용했는지 생각해보자. rear를 사용했다.
		//pop할 때는 front를 사용했다.
		//가장 뒤에 값을 넣는다고 생각하고, 가장 앞에 있는 값을 지운다고 생각하면 편하다.
		//이때 가장 앞에 있는 값은 즉, deque를 해야하는 부분이므로 start _point가 되겠다.
		// [end point] empty region [start point] -> ex) 6 7 8 X X 2 3 4 5 -> 즉, 여기서 push를 할 때 location of 8에 +1을 한 위치에 해야하는데 그걸 지칭하는게 rear 값이니깐.
		// front가 가장 앞에 있는 값인데, 2를 지칭하니 뭐. 그냥 front로.
		// size가 1인 경우에 단방향 증가된 꼴이니 rear > front인 경우와 같다. 그리고 rear == front인 경우가 size = 1인 경우 밖에 없으니 아래와 같이 구성 가능하겠다.
		int index = 0;
		if (rear >= front) {
			for (int i = start_point; i < rear; i++) {
				newqueue[index++] = queue[i];
			}
		}
		else {
			//PDF 자료보다 훨씬 쉬운 auto-resizing circular queue. 저 자료구조 책은 도대체 누굴 위한 편의인가?
			for (int i = start_point; i < capacity; i++) {
				newqueue[index++] = queue[i];
			}
			for (int i = 0; i < rear; i++) {//값을 추가할 때 rear(pre increment)에다가 하니깐 end point region이 된다.
				newqueue[index++] = queue[i];
			}
		}
		delete[] queue;
		queue = newqueue;
		capacity *= 2;
		front = 0;
		rear = index; //index++로 인덱싱하여서 다음을 위한 preincrement가 구현됨.
	}
	queue[rear] = item;
	rear = (rear + 1) % capacity;
	//다음을 위한 pre increment. 즉, rear로 조회시 -1해야함!
}

template <class T>
void Queue<T>::Pop() {
	if (isEmpty()) {
		throw "queue가 비어있습니다!";
	}
	front = (front + 1) % capacity;
	//queue[front].~T(); //사실 이 부분은 없어도 된다. 굳이 destructor를 소환하지 않아도 pop과 push, front, rear만 사용할거라면 문제 없다.
}
