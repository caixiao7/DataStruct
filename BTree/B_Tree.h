#pragma once
#include <math.h> 
//m��B������
/*
1.ÿ���ڵ������m-1���ؼ��֣�m���ӽڵ�
2.���ڵ����ٿ�����1���ؼ���
3.�Ǹ��ڵ�������ceil��m/2��-1���ؼ��֡�����5��B����3-1=2���ؼ���
4.���е�Ҷ�ӽڵ㶼λ��ͬһ��
*/

typedef struct _B_KEY {
	int data;
	int key;
}B_KEY;

typedef struct _B_Node{
	int key_num;
	int child_num;
	int hierarchy;//����
	_B_Node *parent;
	_B_Node**child_list;//�ӽڵ�
	B_KEY**key_list;//�ؼ���

	
	_B_Node()
	{
		child_num = 0;
		key_num = 0;
		hierarchy = -1;
		parent = nullptr;
		child_list = nullptr;
		key_list = nullptr;
	}

	
	_B_Node(int hie) {
		child_num = 0;
		key_num = 0;
		hierarchy = hie;
		parent = nullptr;
		child_list = new _B_Node*[hierarchy+1];
		key_list = new B_KEY*[hierarchy];//֮������hierarchy��������hierarchy-1����Ϊ����Աʱ����ʱ�����ѡ���м�Ľڵ�,��ֹ���
	}
}B_Node;

class B_Tree
{
public:
	B_Tree(int hie);
	~B_Tree();

	void Insert(int val);

	void Print(B_Node*tree);

	B_Node *root_;
private:
	B_Node* SearchLeaf(B_Node*tree,int val);

	B_Node * SplitNode(B_Node*tree);

	//�����в���ڵ�
	void InsertNode(B_Node*tree, int val);
	//ֱ�Ӳ嵽��ǰ�ڵ��key������
	void InsertValToKeyList(B_Node *node,int val);

	int hierarchy_;//����
};

