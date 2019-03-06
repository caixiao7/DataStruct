#include "MyAVLTree.h"
#include <iostream>


MyAVLTree::MyAVLTree()
{
	root_ = nullptr;
}


MyAVLTree::~MyAVLTree()
{
}


void MyAVLTree::UpdateDepth(MyTNode *node)
{
	if (node == nullptr)
		return;
	int l_depth = node->lchild == nullptr ? 0 : node->lchild->depth;
	int r_depth = node->rchild == nullptr ? 0 : node->rchild->depth;
	node->depth = (l_depth > r_depth ? l_depth : r_depth) + 1;
}


int MyAVLTree::GetBF(MyTNode *node)
{
	int l_depth = node->lchild==nullptr?0: node->lchild->depth;
	int r_depth = node->rchild == nullptr ? 0 : node->rchild->depth;
	return l_depth - r_depth;
}

//左旋转本质：
//node变成其右子节点的左子节点
//node的右子节点的左子节点变成node的右子节点
MyTNode* MyAVLTree::L_Rotate(MyTNode*node)
{
	//第一步 交换N和R的值
	auto r_child = node->rchild;
	{
		auto tmp = node->value;
		node->value = r_child->value;
		r_child->value = tmp;
	}

	//第二步 N和R指针互换
	{
		auto tmp = r_child;
		r_child = node;
		node = tmp;
	}

	//第三步  做空N节点  r_child与r_child->r_child->r_child直连 跳过node
	r_child->rchild = node->rchild;
	if(node->rchild!=nullptr)
		node->rchild->parent = r_child;

	//第四步  node的左节点变成node的右节点
	node->rchild = node->lchild;

	//第五步 如果r_child拥有左节点X,node的左节点赋值为X
	node->lchild = r_child->lchild;
	if (r_child->lchild != nullptr)
	{		
		r_child->lchild->parent = node;
	}

	//第六步 r_child左节点赋值为node
	r_child->lchild = node;
	node->parent = r_child;
	return node;
}


MyTNode* MyAVLTree::R_Rotate(MyTNode*node)
{
	//auto parent = node->parent;
	auto l_child = node->lchild;

	//第一步交换 node 和其左孩子的值
	//这个时候node变成了原来的左孩子   左孩子l_child变成了node
	//下面操作node,其实已经是新的node了，即为l_child
	auto tmp = node->value;
	node->value = l_child->value;
	l_child->value = tmp;
	{
		auto tmp = node;
		node = l_child;
		l_child = tmp;
	}
	//
	if (l_child->lchild != nullptr)
	{
		l_child->lchild = node->lchild;
		if(node->lchild!=nullptr)
			node->lchild->parent = l_child;
	}

	//
	
	node->lchild = node->rchild;
	node->rchild = l_child->rchild;
	if (l_child->rchild != nullptr)
	{
		l_child->rchild->parent = node;
		l_child->rchild = node;
	}
	node->parent = l_child;
	l_child->rchild = node;
	return node;
}

void MyAVLTree::AVLTree(MyTNode *node)
{
	int bf = 0;//平衡因子
	while (node != nullptr)
	{
		UpdateDepth(node);
		bf = GetBF(node);
		//说明已经不平衡了！！！！
		if (bf > 1 || bf < -1)
		{
			//左子树高
			if (bf > 1)
			{
				int l_bf = GetBF(node->lchild);
				if (l_bf >= 0)//LL型
				{
					//需要重新计算深度
					node = R_Rotate(node);
					continue;
				}
				else//LR型
				{
					node = L_Rotate(node->lchild);
					//R_Rotate(node);
					//node = tmp;
					continue;
				}
			}
			if (bf < -1)
			{
				int l_bf = GetBF(node->rchild);
				if (l_bf <=0)//RR型
				{
					//需要重新计算深度
					node = L_Rotate(node);
					continue;
				}
				else//RL型
				{
					//需要重新计算深度
					node = R_Rotate(node->rchild);
					continue;
				}
			}
			//需要调整
		}
		node = node->parent;
	}
}


void MyAVLTree::Del(int val, MyTNode*node)
{
	if(node == nullptr)
		node = Search( val, root_);

	if (node == nullptr)
		return;

	//没有子树 直接删除
	if (node->lchild == nullptr && node->rchild == nullptr)
	{
		//调整node parent
		if (node->parent->lchild == node)
		{
			node->parent->lchild = nullptr;
		}
		else
			node->parent->rchild = nullptr;

		AVLTree(node->parent);

		delete node;
		node = nullptr;


		return;
	}
	else if (node->rchild == nullptr)
	{
		//需要从node_parent开始调整
		auto node_parent = node->parent;
		if (node->parent->lchild == node)
		{
			node->parent->lchild = node->lchild;
			node->lchild->parent = node->parent;
		}
		else
		{
			node->parent->rchild = node->lchild;
			node->lchild->parent = node->parent;
		}

		AVLTree(node->parent);

		delete node;
		node = nullptr;

		return;
	}
	else if (node->lchild == nullptr)
	{
		//需要从node_parent开始调整
		auto node_parent = node->parent;
		if (node->parent->lchild == node)
		{
			node->parent->lchild = node->rchild;
			node->rchild->parent = node->parent;
		}
		else
		{
			node->parent->rchild = node->rchild;
			node->rchild->parent = node->parent;
		}

		AVLTree(node->parent);
		delete node;
		node = nullptr;

		return;
	}
	else
	{
		//删除的节点node存在左右两颗子树
		auto forward_node = node->lchild;
		//使用复制删除  找到前驱（左子树中，一直往右下方寻找，找到没有右节点的节点，替代node）
		while (forward_node->rchild != nullptr)
		{
			forward_node = forward_node->rchild;
		}
		node->value= forward_node->value;
		return Del(-1, forward_node);
	}
}

MyTNode *MyAVLTree::Search(int val, MyTNode *node)
{
	if (node == nullptr)
		return nullptr;
	if (node->value == val)
		return node;
	if (node->value > val)
	{
		return Search( val, node->lchild);
	}
	else
		return Search( val, node->rchild);
}

void MyAVLTree::Insert(int val)
{
	//返回插入节点所在位置,按二叉排序树的简单插入
	MyTNode* node = InsertVal(root_, root_, val);
	//更新深度
	UpdateDepth(node);
	//二叉树的平衡调整,里面会递归更新每个节点的深度
	AVLTree(node);
}

void MyAVLTree::PrintLastOrder(MyTNode *node)
{
	if (node == nullptr)
		return;
	
	PrintLastOrder(node->lchild);
	PrintLastOrder(node->rchild);
	std::cout << node->value << std::endl;
}
void MyAVLTree::PrintMidOrder(MyTNode *node)
{
	if (node == nullptr)
		return;

	std::cout << node->value << std::endl;
	PrintMidOrder(node->lchild);
	PrintMidOrder(node->rchild);
}

void MyAVLTree::PrintPreOrder(MyTNode *node)
{
	if (node == nullptr)
		return;
	PrintPreOrder(node->lchild);
	std::cout << node->value << std::endl;
	PrintPreOrder(node->rchild);
}

MyTNode* MyAVLTree::InsertVal(MyTNode* &node, MyTNode* &parent, int val)
{
	if(node == nullptr)
	{
		if (parent == nullptr)
		{
			node = new MyTNode();
			node->value = val;
			node->parent = nullptr;
			return node;
		}
		node = new MyTNode();
		node->value = val;		
		node->parent = parent;
		return node;
	}
	if (node->value > val)
	{
		return InsertVal(node->lchild, node, val);
	}
	else
	{
		return InsertVal(node->rchild, node, val);
	}
}
