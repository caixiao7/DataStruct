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
	//没有找到
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
	//第一步，找到节点
	auto node = Search(root_,value);
	if (node == nullptr)
		return;
	
	bool left_side_unbalance = true;//当前节点的左(右)子树缺少一个黑节点
	RBTNode *backward_node = nullptr;
	if (node->lchild != nullptr) {
		backward_node = FindForwardNode(node->lchild);//真正的前驱,没有右节点
	}
	else
		backward_node = node;//原始node，没有左节点

	//交换后继节点和当前节点value
	{
		auto tmp = node->value;
		node->value = backward_node->value;
		backward_node->value = tmp;
	}

	//开始删除后继节点，只有一个或者没有子树
	{
		bool is_red = backward_node->color == COLOR::RED;
		auto parent = backward_node->parent;

		//如果backward_node为根节点
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

		//不存在两个子节点都存在的情况
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
		//后继节点重新指向
		backward_node = parent;

		//如果删除的节点为红色的，不影响红黑树属性，完成删除
		if (is_red)
			return;
	}
	
	
	DelBalance(backward_node, left_side_unbalance);

}


void MyRBTree::DelBalance(RBTNode *forward_parent, bool left_side_unbalance)
{
	auto parent = forward_parent;

	//如果backward_node为根节点
	if (parent == nullptr)
	{
		root_ = forward_parent;
		return;
	}

	//讨论backward_node的左子树少了一个黑节点的情况
	if (left_side_unbalance)
	{
		//node 节点到这里已经必须是黑色的
		auto uncle = parent->rchild;
		auto u_l = uncle->lchild;
		auto u_r = uncle->rchild;

		auto color_parent = parent->color;
		auto color_uncle_parent = uncle==nullptr? COLOR::BLACK:uncle->color;
		auto color_u_l_parent = u_l == nullptr ? COLOR::BLACK : u_l->color;
		auto color_u_r_parent = u_r == nullptr ? COLOR::BLACK : u_r->color;

		//情况1：全黑
		if (color_parent == COLOR::BLACK
			&&color_uncle_parent == COLOR::BLACK
			&&color_u_l_parent == COLOR::BLACK
			&&color_u_r_parent == COLOR::BLACK
			)
		{
			uncle->color = COLOR::BLACK;
			return DelBalance(parent);//递归parent，整颗parent树少了一个黑节点
		}

		//情况2：P红 其他黑
		if (color_parent == COLOR::RED
			&&color_uncle_parent == COLOR::BLACK
			&&color_u_l_parent == COLOR::BLACK
			&&color_u_r_parent == COLOR::BLACK
			)
		{
			uncle->color = COLOR::RED;
			parent->color = COLOR::BLACK;
			return;//搞定
		}

		//情况3：U红 其他黑
		if (color_parent == COLOR::BLACK
			&&color_uncle_parent == COLOR::RED
			&&color_u_l_parent == COLOR::BLACK
			&&color_u_r_parent == COLOR::BLACK
			)
		{
			uncle->color = COLOR::BLACK;
			parent->color = COLOR::RED;
			L_Rotate(parent);
			return DelBalance(parent,true);//递归当前节点
		}

		//情况4：UL红 P无所谓 其他黑
		if (color_uncle_parent == COLOR::BLACK
			&&color_u_l_parent == COLOR::RED
			&&color_u_r_parent == COLOR::BLACK
			) {
			uncle->color = COLOR::RED;
			u_l->color = COLOR::BLACK;
			R_Rotate(uncle);
			return DelBalance(parent);//递归当前节点
		}

		//情况5：UR红 P\UL无所谓 U黑
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
	//根节点 直接插入
	if (root_ == nullptr)	{
		root_ = node;
		root_->color = COLOR::BLACK;//跟节点颜色默认为黑
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
		//插入到左子树
		if (tree->value >= node->value) {
			InsertNode(tree->lchild, tree, node);
			break;
		}
		//插入到右子树
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
		//说明node已经是根节点,直接黑化
		node->color = COLOR::BLACK;
		root_ = node;
		return;
	}
	if (parent->parent == nullptr)
	{
		//说明node->parent已经是根节点,直接黑化
		node->parent->color = COLOR::BLACK;
		root_ = node->parent;
		return;
	}

	//前面两步确保node含有父节点、祖父节点


	if (node->parent->color == COLOR::BLACK)
	{
		//父节点颜色为黑 无需调整
		return;
	}
	else if (node->parent->color == COLOR::RED)
	{
		//父节点为红，继续讨论
		bool node_is_L_Child = false; //当前节点是父节点的左节点
		bool parent_L_Child = false; //父节点为祖父节点的左孩子
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

		//最简单的情况
		//第0种情况:叔叔为红，那祖父肯定是黑
		if (uncle != nullptr && uncle->color == COLOR::RED)
		{
			//策略：父节点和叔节点变黑，祖父变红，祖父作为当前节点，递归此函数
			parent->color = COLOR::BLACK;
			uncle->color = COLOR::BLACK;
			grand_pa->color = COLOR::RED;
			Balance(grand_pa);
			return;
		}

		//以下情况，叔叔节点必须为黑，G点为黑，P点为红，N点也为红.

		//<型：P点作为G点的左孩子，N点作为右孩子
		//策略：以P为当前节点，左旋
		if (!node_is_L_Child && parent_L_Child)
		{
			L_Rotate(parent);
			return Balance(parent);
		}

		//>型：P点作为G点的右孩子，N点作为左孩子
		//策略：以P为当前节点，右旋
		if (node_is_L_Child && !parent_L_Child)
		{
			R_Rotate(parent);
			return Balance(parent);
		}

		//   /型：P点作为G点的左孩子，N点作为左孩子
		//策略：祖父节点为当前节点，祖父节点变红，父节点变黑. 祖父节点右旋
		if (node_is_L_Child && parent_L_Child)
		{
			parent->color = COLOR::BLACK;
			grand_pa->color = COLOR::RED;
			R_Rotate(grand_pa);
			return Balance(grand_pa);
		}

		//  \型：P点作为G点的右孩子，N点作为右孩子
		//策略：祖父节点为当前节点，祖父节点变红，父节点变黑. 祖父节点左旋
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

//对node作右旋
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

