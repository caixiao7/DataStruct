#pragma once
#include <iostream>
#define MAX_SIZE 10

class MyQueue
{
public:
	MyQueue();
	~MyQueue();

	bool isFull()
	{
		if (rear + 1 == front)
			return true;
		if (rear + 1 % MAX_SIZE == 0)
			return true;
		return false;
	}

	bool isEmpty()
	{
		return front == rear;
	}

	void push_front(int val)
	{
		if (front == -1)
		{
			front = rear = 0;
			data[front] = val;
			return;
		}

		if (isFull())
			return;
		if (front == 0)
			front = MAX_SIZE - 1;
		else
			front--;
		data[front] = val;
	}

	void push_back(int val)
	{
		if (front == -1)
		{
			front = rear = 0;
			data[front] = val;
			return;
		}
		if (isFull())
			return;
		if (rear == MAX_SIZE - 1)
			rear = 0;
		else
			rear++;
		data[rear] = val;
	}

	void Print()
	{
		int begin = front;
		int end = rear;
		while (1)
		{
			if (begin == -1)
			{
				return;
			}
			if (begin == end)
			{
				std::cout << data[begin];
				return;
			}
			std::cout << data[begin];
			if (begin == MAX_SIZE - 1)
				begin = 0;
			else
				begin++;
		}
	}

	int front;
	int rear;

	int data[MAX_SIZE];
};

