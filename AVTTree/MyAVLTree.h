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

	//������ò��뺯��,�������Զ�����
	void Insert(int val);

	MyTNode *Search(int val, MyTNode *node);

	void Del(int val, MyTNode*node =nullptr);

	void AVLTree(MyTNode *node);

	MyTNode* L_Rotate(MyTNode*node);
	
	MyTNode* R_Rotate(MyTNode*node);

	int GetBF(MyTNode *node);

	void UpdateDepth(MyTNode *node);

	//ǰ�����
	void PrintPreOrder(MyTNode *node);

	//�������
	void PrintMidOrder(MyTNode *node);

	//�������
	void PrintLastOrder(MyTNode *node);
private:
	//�ڲ����뺯�����������Ĳ���,���ز���ڵ�
	MyTNode* InsertVal(MyTNode* &node, MyTNode* &parent,int val);


};

