#pragma once

typedef struct MyTNode_S_
{
	MyTNode_S_ *parent;
	MyTNode_S_ *lchild;
	MyTNode_S_ *rchild;
	int value;
	int depth;

	MyTNode_S_()
	{
		parent = nullptr;
		lchild = nullptr;
		rchild = nullptr;
		value = 0;
		depth = 0;
	}
}MyTNode;

class MyAVLTree
{
public:
	MyTNode *root_;
	MyAVLTree();
	~MyAVLTree();

	//外面调用插入函数,插入后会自动调整
	void Insert(int val);

	MyTNode *Search(int val, MyTNode *node);

	void Del(int val, MyTNode*node =nullptr);

	void AVLTree(MyTNode *node);

	MyTNode* L_Rotate(MyTNode*node);
	
	MyTNode* R_Rotate(MyTNode*node);

	int GetBF(MyTNode *node);

	void UpdateDepth(MyTNode *node);

	//前序输出
	void PrintPreOrder(MyTNode *node);

	//中序输出
	void PrintMidOrder(MyTNode *node);

	//后序输出
	void PrintLastOrder(MyTNode *node);
private:
	//内部插入函数，二叉树的插入,返回插入节点
	MyTNode* InsertVal(MyTNode* &node, MyTNode* &parent,int val);


};

