#pragma once

#include <iostream>
class MyTwoSideNote
{
public:
	MyTwoSideNote(int val)
	{
		value = val;
		next = nullptr;
		pre = nullptr;
	};
	~MyTwoSideNote() {};

	MyTwoSideNote *next;
	MyTwoSideNote *pre;
	int value;
};

class MyTwoSideList
{
public:
	MyTwoSideList();
	~MyTwoSideList();

	void Addhead(MyTwoSideNote* note)
	{
		note->next = nullptr;
		note->pre = nullptr;

		if (head == nullptr)
		{
			head = note;
			tail = note;
			return;
		}
		note->next = head;
		head->pre = note;
		head = note;
	}

	void AddTail(MyTwoSideNote* note)
	{
		note->next = nullptr;
		note->pre = nullptr;

		if (head == nullptr)
		{
			head = note;
			tail = note;
			return;
		}

		note->pre = tail;
		tail->next = note;
		tail = note;
	}

	void RemoveHead()
	{
		if (head == nullptr)
		{
			return;
		}

		auto ptr = head;
		head = head->next;
		head->pre = nullptr;
		delete ptr;
		ptr = nullptr;
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

	MyTwoSideNote *head;
	MyTwoSideNote *tail;
};

