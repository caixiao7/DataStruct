#include "MyRBTree.h"
#include <iostream>



MyRBTree::MyRBTree()
{
	root_ = nullptr;
}


MyRBTree::~MyRBTree()
{
}


void MyRBTree::PrePrint(RBTNode *tree)
{
	if (tree == nullptr)
		return;
	PrePrint(tree->lchild);

	auto tmp = (tree->color == COLOR::RED) ? "RED" : "BLACK         ";
	std::cout << tmp << "___:" << tree->value << "\n";

	PrePrint(tree->rchild);
}

void MyRBTree::MidPrint(RBTNode *tree)
{
	if (tree == nullptr)
		return;
	auto tmp = (tree->color == COLOR::RED) ? "RED" : "BLACK         ";
	std::cout << tmp << "___:" << tree->value<<"\n";
	MidPrint(tree->lchild);
	MidPrint(tree->rchild);
}


RBTNode* MyRBTree::Search(RBTNode *tree, int value)
{
	//û���ҵ�
	if (tree == nullptr)
		return nullptr;

	if (tree->value == value)
		return tree;

	if (tree->value >= value)
		return Search(tree->lchild, value);
	else
		return Search(tree->rchild, value);
}


RBTNode *MyRBTree::FindForwardNode(RBTNode *tree)
{
	while (tree->rchild != nullptr)
	{
		tree = tree->rchild;
	}
	return tree;
}

void MyRBTree::Del(int value)
{
	//��һ�����ҵ��ڵ�
	auto node = Search(root_,value);
	if (node == nullptr)
		return;
	
	bool left_side_unbalance = true;//��ǰ�ڵ����(��)����ȱ��һ���ڽڵ�
	RBTNode *backward_node = nullptr;
	if (node->lchild != nullptr) {
		backward_node = FindForwardNode(node->lchild);//������ǰ��,û���ҽڵ�
	}
	else
		backward_node = node;//ԭʼnode��û����ڵ�

	//������̽ڵ�͵�ǰ�ڵ�value
	{
		auto tmp = node->value;
		node->value = backward_node->value;
		backward_node->value = tmp;
	}

	//��ʼɾ����̽ڵ㣬ֻ��һ������û������
	{
		bool is_red = backward_node->color == COLOR::RED;
		auto parent = backward_node->parent;

		//���backward_nodeΪ���ڵ�
		if (parent == nullptr)
		{
			if (backward_node->lchild != nullptr)
			{
				backward_node->lchild->parent = nullptr;
				backward_node->lchild->color = COLOR::BLACK;
				delete backward_node;
				root_ = backward_node->lchild;
			}
			else if (backward_node->rchild != nullptr)
			{
				backward_node->rchild->parent = nullptr;
				backward_node->rchild->color = COLOR::BLACK;
				delete backward_node;
				root_ = backward_node->rchild;
			}

			return;
		}

		if (parent->lchild == backward_node)		{
			left_side_unbalance = true;
		}
		else {
			left_side_unbalance = false;
		}

		//�����������ӽڵ㶼���ڵ����
		if (backward_node->lchild != nullptr)
		{
			if (left_side_unbalance) {
				parent->lchild = backward_node->lchild;
			}
			else {
				parent->rchild = backward_node->lchild;
			}
			backward_node->lchild->parent = parent;
		}
		else if (backward_node->rchild != nullptr)
		{
			if (left_side_unbalance) {
				parent->lchild = backward_node->rchild;
			}
			else {
				parent->rchild = backward_node->rchild;
			}
			backward_node->rchild->parent = parent;
		}
		else if (backward_node->lchild == nullptr
			&&
			backward_node->rchild == nullptr
			)
		{
			if (left_side_unbalance)
				parent->lchild = nullptr;
			else
				parent->rchild = nullptr;
		}

		delete backward_node;
		//��̽ڵ�����ָ��
		backward_node = parent;

		//���ɾ���Ľڵ�Ϊ��ɫ�ģ���Ӱ���������ԣ����ɾ��
		if (is_red)
			return;
	}
	
	
	DelBalance(backward_node, left_side_unbalance);

}


void MyRBTree::DelBalance(RBTNode *forward_parent, bool left_side_unbalance)
{
	auto parent = forward_parent;

	//���backward_nodeΪ���ڵ�
	if (parent == nullptr)
	{
		root_ = forward_parent;
		return;
	}

	//����backward_node������������һ���ڽڵ�����
	if (left_side_unbalance)
	{
		//node �ڵ㵽�����Ѿ������Ǻ�ɫ��
		auto uncle = parent->rchild;
		auto u_l = uncle->lchild;
		auto u_r = uncle->rchild;

		auto color_parent = parent->color;
		auto color_uncle_parent = uncle==nullptr? COLOR::BLACK:uncle->color;
		auto color_u_l_parent = u_l == nullptr ? COLOR::BLACK : u_l->color;
		auto color_u_r_parent = u_r == nullptr ? COLOR::BLACK : u_r->color;

		//���1��ȫ��
		if (color_parent == COLOR::BLACK
			&&color_uncle_parent == COLOR::BLACK
			&&color_u_l_parent == COLOR::BLACK
			&&color_u_r_parent == COLOR::BLACK
			)
		{
			uncle->color = COLOR::BLACK;
			return DelBalance(parent);//�ݹ�parent������parent������һ���ڽڵ�
		}

		//���2��P�� ������
		if (color_parent == COLOR::RED
			&&color_uncle_parent == COLOR::BLACK
			&&color_u_l_parent == COLOR::BLACK
			&&color_u_r_parent == COLOR::BLACK
			)
		{
			uncle->color = COLOR::RED;
			parent->color = COLOR::BLACK;
			return;//�㶨
		}

		//���3��U�� ������
		if (color_parent == COLOR::BLACK
			&&color_uncle_parent == COLOR::RED
			&&color_u_l_parent == COLOR::BLACK
			&&color_u_r_parent == COLOR::BLACK
			)
		{
			uncle->color = COLOR::BLACK;
			parent->color = COLOR::RED;
			L_Rotate(parent);
			return DelBalance(parent,true);//�ݹ鵱ǰ�ڵ�
		}

		//���4��UL�� P����ν ������
		if (color_uncle_parent == COLOR::BLACK
			&&color_u_l_parent == COLOR::RED
			&&color_u_r_parent == COLOR::BLACK
			) {
			uncle->color = COLOR::RED;
			u_l->color = COLOR::BLACK;
			R_Rotate(uncle);
			return DelBalance(parent);//�ݹ鵱ǰ�ڵ�
		}

		//���5��UR�� P\UL����ν U��
		if (color_uncle_parent == COLOR::BLACK
			&&color_u_r_parent == COLOR::RED
			)
		{
			uncle->color = parent->color;
			parent->color = COLOR::BLACK;
			u_r->color = COLOR::BLACK;
			L_Rotate(parent);
			return;
		}
	}
}

void MyRBTree::Insert(int value)
{
	RBTNode *node = new RBTNode();
	node->value = value;
	//���ڵ� ֱ�Ӳ���
	if (root_ == nullptr)	{
		root_ = node;
		root_->color = COLOR::BLACK;//���ڵ���ɫĬ��Ϊ��
		return;
	}
	else {
		InsertNode(root_, root_,node);

		Balance(node);
	}


}



void MyRBTree::InsertNode(RBTNode*&tree, RBTNode*parent, RBTNode*node)
{
	do {
		if (tree == nullptr)
		{
			tree = node;
			node->parent = parent;
			break;
		}
		//���뵽������
		if (tree->value >= node->value) {
			InsertNode(tree->lchild, tree, node);
			break;
		}
		//���뵽������
		if (tree->value < node->value) {
			InsertNode(tree->rchild, tree,node);
			break;
		}
	} while (0);
	
}


void MyRBTree::Balance(RBTNode *&node)
{	
	auto parent = node->parent;

	if (parent == nullptr)
	{
		//˵��node�Ѿ��Ǹ��ڵ�,ֱ�Ӻڻ�
		node->color = COLOR::BLACK;
		root_ = node;
		return;
	}
	if (parent->parent == nullptr)
	{
		//˵��node->parent�Ѿ��Ǹ��ڵ�,ֱ�Ӻڻ�
		node->parent->color = COLOR::BLACK;
		root_ = node->parent;
		return;
	}

	//ǰ������ȷ��node���и��ڵ㡢�游�ڵ�


	if (node->parent->color == COLOR::BLACK)
	{
		//���ڵ���ɫΪ�� �������
		return;
	}
	else if (node->parent->color == COLOR::RED)
	{
		//���ڵ�Ϊ�죬��������
		bool node_is_L_Child = false; //��ǰ�ڵ��Ǹ��ڵ����ڵ�
		bool parent_L_Child = false; //���ڵ�Ϊ�游�ڵ������
		auto parent = node->parent;
		auto grand_pa = node->parent->parent;
		RBTNode*uncle = nullptr;
		if (parent == grand_pa->lchild) {
			uncle = grand_pa->rchild;
			parent_L_Child = true;
		}
		else {
			uncle = grand_pa->lchild;
			parent_L_Child = false;
		}			 

		if (node == parent->lchild)
			node_is_L_Child = true;
		else
			node_is_L_Child = false;

		//��򵥵����
		//��0�����:����Ϊ�죬���游�϶��Ǻ�
		if (uncle != nullptr && uncle->color == COLOR::RED)
		{
			//���ԣ����ڵ����ڵ��ڣ��游��죬�游��Ϊ��ǰ�ڵ㣬�ݹ�˺���
			parent->color = COLOR::BLACK;
			uncle->color = COLOR::BLACK;
			grand_pa->color = COLOR::RED;
			Balance(grand_pa);
			return;
		}

		//�������������ڵ����Ϊ�ڣ�G��Ϊ�ڣ�P��Ϊ�죬N��ҲΪ��.

		//<�ͣ�P����ΪG������ӣ�N����Ϊ�Һ���
		//���ԣ���PΪ��ǰ�ڵ㣬����
		if (!node_is_L_Child && parent_L_Child)
		{
			L_Rotate(parent);
			return Balance(parent);
		}

		//>�ͣ�P����ΪG����Һ��ӣ�N����Ϊ����
		//���ԣ���PΪ��ǰ�ڵ㣬����
		if (node_is_L_Child && !parent_L_Child)
		{
			R_Rotate(parent);
			return Balance(parent);
		}

		//   /�ͣ�P����ΪG������ӣ�N����Ϊ����
		//���ԣ��游�ڵ�Ϊ��ǰ�ڵ㣬�游�ڵ��죬���ڵ���. �游�ڵ�����
		if (node_is_L_Child && parent_L_Child)
		{
			parent->color = COLOR::BLACK;
			grand_pa->color = COLOR::RED;
			R_Rotate(grand_pa);
			return Balance(grand_pa);
		}

		//  \�ͣ�P����ΪG����Һ��ӣ�N����Ϊ�Һ���
		//���ԣ��游�ڵ�Ϊ��ǰ�ڵ㣬�游�ڵ��죬���ڵ���. �游�ڵ�����
		if (!node_is_L_Child && !parent_L_Child)
		{
			parent->color = COLOR::BLACK;
			grand_pa->color = COLOR::RED;
			L_Rotate(grand_pa);

			return Balance(grand_pa);
		}
	}
	else
	{
		std::cout << "\nerror";
	}
}


RBTNode* MyRBTree::L_Rotate(RBTNode*node) {

	RBTNode* parent = node->parent;
	RBTNode * r_child = node->rchild;

	if (parent == nullptr)
	{
	}
	else if (parent->lchild == node)
	{
		parent->lchild = r_child;
	}
	else if (parent->rchild == node)
	{
		parent->rchild = r_child;
	}

	r_child->parent = parent;

	if (r_child->lchild != nullptr)
	{
		r_child->lchild->parent = node;
	}
	node->rchild = r_child->lchild;

	r_child->lchild = node;
	node->parent = r_child;

	return node;
}

//��node������
RBTNode* MyRBTree::R_Rotate(RBTNode*node) {

	RBTNode* parent = node->parent;
	RBTNode * l_child = node->lchild;

	if (parent == nullptr)
	{
	}
	else if (parent->lchild == node)
	{
		parent->lchild = l_child;
	}
	else if (parent->rchild == node)
	{
		parent->rchild = l_child;
	}

	l_child->parent = parent;

	if (l_child->rchild != nullptr)
	{
		l_child->rchild->parent = node;
	}
	node->lchild = l_child->rchild;

	l_child->rchild = node;
	node->parent = l_child;

	return node;
}

