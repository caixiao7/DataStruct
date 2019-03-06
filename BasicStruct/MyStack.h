#pragma once

class StackNote
{
public:
	StackNote() {};
	~StackNote() {};
};

class MyStack
{
public:
	MyStack();
	~MyStack();

	void push(int val)
	{
		front++;
		a[front] = val;
	}

	bool isEmpty()
	{
		return front == -1;
	}

	int pop()
	{
		if (isEmpty())
			return -999;
		auto tmp = a[front];
		front--;
		return tmp;
	}

	int front;
	int a[1024];
};

