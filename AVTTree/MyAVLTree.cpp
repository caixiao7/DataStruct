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

//����ת���ʣ�
//node��������ӽڵ�����ӽڵ�
//node�����ӽڵ�����ӽڵ���node�����ӽڵ�
MyTNode* MyAVLTree::L_Rotate(MyTNode*node)
{
	//��һ�� ����N��R��ֵ
	auto r_child = node->rchild;
	{
		auto tmp = node->value;
		node->value = r_child->value;
		r_child->value = tmp;
	}

	//�ڶ��� N��Rָ�뻥��
	{
		auto tmp = r_child;
		r_child = node;
		node = tmp;
	}

	//������  ����N�ڵ�  r_child��r_child->r_child->r_childֱ�� ����node
	r_child->rchild = node->rchild;
	if(node->rchild!=nullptr)
		node->rchild->parent = r_child;

	//���Ĳ�  node����ڵ���node���ҽڵ�
	node->rchild = node->lchild;

	//���岽 ���r_childӵ����ڵ�X,node����ڵ㸳ֵΪX
	node->lchild = r_child->lchild;
	if (r_child->lchild != nullptr)
	{		
		r_child->lchild->parent = node;
	}

	//������ r_child��ڵ㸳ֵΪnode
	r_child->lchild = node;
	node->parent = r_child;
	return node;
}


MyTNode* MyAVLTree::R_Rotate(MyTNode*node)
{
	//auto parent = node->parent;
	auto l_child = node->lchild;

	//��һ������ node �������ӵ�ֵ
	//���ʱ��node�����ԭ��������   ����l_child�����node
	//�������node,��ʵ�Ѿ����µ�node�ˣ���Ϊl_child
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
	int bf = 0;//ƽ������
	while (node != nullptr)
	{
		UpdateDepth(node);
		bf = GetBF(node);
		//˵���Ѿ���ƽ���ˣ�������
		if (bf > 1 || bf < -1)
		{
			//��������
			if (bf > 1)
			{
				int l_bf = GetBF(node->lchild);
				if (l_bf >= 0)//LL��
				{
					//��Ҫ���¼������
					node = R_Rotate(node);
					continue;
				}
				else//LR��
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
				if (l_bf <=0)//RR��
				{
					//��Ҫ���¼������
					node = L_Rotate(node);
					continue;
				}
				else//RL��
				{
					//��Ҫ���¼������
					node = R_Rotate(node->rchild);
					continue;
				}
			}
			//��Ҫ����
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

	//û������ ֱ��ɾ��
	if (node->lchild == nullptr && node->rchild == nullptr)
	{
		//����node parent
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
		//��Ҫ��node_parent��ʼ����
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
		//��Ҫ��node_parent��ʼ����
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
		//ɾ���Ľڵ�node����������������
		auto forward_node = node->lchild;
		//ʹ�ø���ɾ��  �ҵ�ǰ�����������У�һֱ�����·�Ѱ�ң��ҵ�û���ҽڵ�Ľڵ㣬���node��
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
	//���ز���ڵ�����λ��,�������������ļ򵥲���
	MyTNode* node = InsertVal(root_, root_, val);
	//�������
	UpdateDepth(node);
	//��������ƽ�����,�����ݹ����ÿ���ڵ�����
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
