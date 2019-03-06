#pragma once
typedef enum
{
	RED,
	BLACK
}COLOR;

typedef struct _RBTNode
{
	COLOR color;
	int value;
	_RBTNode *parent;
	_RBTNode *lchild;
	_RBTNode *rchild;
	_RBTNode()
	{
		parent = nullptr;
		lchild = nullptr;
		rchild = nullptr;
		int value = -1;
		color = COLOR::RED;//Ä¬ÈÏÑÕÉ«Îªºì

	}

}RBTNode;

class MyRBTree
{
public:
	MyRBTree();
	~MyRBTree();

	RBTNode*root_;

	void Insert(int value);

	void Del(int value);

	void MidPrint(RBTNode *tree);

	RBTNode* Search(RBTNode *tree,int value);

	void PrePrint(RBTNode *tree);

private:
	void DelBalance(RBTNode *forward_parent,bool left = true);

	RBTNode *FindForwardNode(RBTNode *tree);

	void InsertNode(RBTNode*&tree, RBTNode*parent, RBTNode*node);
	
	RBTNode* L_Rotate(RBTNode*node);

	RBTNode* R_Rotate(RBTNode*node);

	void Balance(RBTNode *&node);

};

