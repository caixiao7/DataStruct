#pragma once
#include <iostream>
class MyOneSideNote
{
public:
	MyOneSideNote(int val, MyOneSideNote*n=nullptr) 
	{
		value = val;
		next = n;
	};
	~MyOneSideNote() {};

	MyOneSideNote *next;
	int value;
};

class MyList
{
public:
	MyList();
	~MyList();

	void AddToTail(MyOneSideNote *note)
	{
		note->next = nullptr;

		if (head == nullptr)
		{
			//队首队尾都是它
			head = note;
			tail = note;			
			return;
		}

		tail->next = note;
		tail = note;
	}

	void AddToHead(MyOneSideNote *note)
	{
		note->next = nullptr;

		if (head == nullptr)
		{
			//队首队尾都是它
			tail = note;
		}
		note->next = head;
		head = note;
	}

	void RemoveHead()
	{
		auto ptr = head;
		head = head->next;
		delete ptr;
		ptr = nullptr;
	}

	void RemoveTail()
	{
		if (head == nullptr)
			return;
		if (head == tail)
		{
			auto ptr = head;
			head = nullptr;
			tail = nullptr; delete ptr;
			ptr = nullptr;
			return;
		}

		auto ptr = head;
		while (ptr->next != tail) {
			ptr = ptr->next;
		}
		delete tail;
		tail = ptr;
		tail->next = nullptr;//队尾指针指向null
	}

	void Print()
	{
		auto ptr = head;
		while (ptr != nullptr)
		{
			std::cout << ptr->value;
			ptr = ptr->next;
		}
	}

	MyOneSideNote *head;
	MyOneSideNote *tail;
};

