#pragma once
#include <math.h> 
//m阶B树定义
/*
1.每个节点最多有m-1个关键字，m个子节点
2.根节点最少可以有1个关键字
3.非根节点最少有ceil（m/2）-1个关键字。例如5阶B树有3-1=2个关键字
4.所有的叶子节点都位于同一层
*/

typedef struct _B_KEY {
	int data;
	int key;
}B_KEY;

typedef struct _B_Node{
	int key_num;
	int child_num;
	int hierarchy;//阶数
	_B_Node *parent;
	_B_Node**child_list;//子节点
	B_KEY**key_list;//关键字

	
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
		key_list = new B_KEY*[hierarchy];//之所以用hierarchy，而不是hierarchy-1，是为了满员时，临时插入后，选择中间的节点,防止溢出
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

	//在树中插入节点
	void InsertNode(B_Node*tree, int val);
	//直接插到当前节点的key队列中
	void InsertValToKeyList(B_Node *node,int val);

	int hierarchy_;//阶数
};

