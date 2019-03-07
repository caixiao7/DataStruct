#include "B_Tree.h"
#include <iostream>


B_Tree::B_Tree(int hie)
{
	root_ = new B_Node(hie);
	hierarchy_ = hie;
}


B_Tree::~B_Tree()
{
}


void B_Tree::Insert(int val)
{
	//找到叶子节点
	auto leaf = SearchLeaf(root_, val);
	//插入到叶子节点
	InsertNode(leaf, val);
}


void B_Tree::Print(B_Node*tree)
{
	for (int i = 0; i < tree->key_num; i++)
	{
		std::cout << tree->key_list[i]->key << " \n";
	}
	std::cout << "---\n";

	for (int i = 0; i < tree->child_num; i++)
	{
		Print(tree->child_list[i]);
	}

}

B_Node* B_Tree::SearchLeaf(B_Node*tree,int val)
{
	//叶子节点
	if (tree->child_num == 0)
	{
		return tree;
	}
	else
	{
		for (int i = 0; i <= tree->key_num - 1; i++)
		{
			if (val < tree->key_list[i]->key)
			{
				return SearchLeaf((tree->child_list[i]),val);
			}
		}

		//最后一个child指针继续寻找
		return  SearchLeaf((tree->child_list[tree->child_num-1]), val);
	}
}


/*
1.取当前节点key队列的中间节点mid
2.分裂成左右两边
3.mid插入起父节点
4.返回父节点
*/
B_Node * B_Tree::SplitNode(B_Node*tree)
{
	B_Node*parent = tree->parent;
	//如果当前节点为根节点，新建一个新的根节点，当前节点分裂成两个节点
	if (parent == nullptr)
	{
		//创建新的根节点
		parent = new B_Node(hierarchy_);
		root_ = parent;
		parent->child_list[0] = tree;
		tree->parent = parent;
		parent->child_num++;
	}

	auto mid_key = tree->key_list[tree->key_num / 2];

	//插入mid key到父节点
	InsertValToKeyList(parent, mid_key->key);


	//创建tree的兄弟节点bro
	//tree分裂出一半给bro
	B_Node *bro = new B_Node(hierarchy_);
	//mid kid移到父节点
	tree->key_list[tree->key_num / 2] = nullptr;
	tree->key_num--;
	int ptr =1+ tree->key_num / 2;
	int child_num = tree->child_num;
	for (int i = ptr; i < child_num; i++)
	{
		auto adsf = tree->child_list[i];
		bro->child_list[i - ptr] = tree->child_list[i];
		bro->child_list[i - ptr]->parent = parent;
		tree->child_list[i] = nullptr;
		tree->child_num--;
		bro->child_num++;
	}
	int key_num = tree->key_num;
	for (int i = ptr; i <= key_num; i++)
	{
		bro->key_list[i - ptr] = tree->key_list[i];
		tree->key_list[i] = nullptr;
		bro->key_num++;
		tree->key_num--;
	}
	bro->parent = parent;

	for (int i = 0; i < parent->child_num; i++)
	{
		if (tree == parent->child_list[i])
		{
			B_Node *pre_node = parent->child_list[i + 1];
			parent->child_list[i + 1] = bro;
			int child_num = parent->child_num;
			parent->child_num++;
			for (int j = i+2; j <= child_num; j++)
			{
				parent->child_list[j] = pre_node;
				pre_node = parent->child_list[j+1];
			}
			break;
		}
	}	

	return parent;
}

void B_Tree::InsertNode(B_Node*tree, int val)
{
	//叶子结点未满 直接插入
	if (tree->key_num < hierarchy_ - 1)
	{
		return InsertValToKeyList(tree, val);
	}
	else{
		
		//先将叶子节点插满
		InsertValToKeyList(tree, val);

		//分裂叶子节点,并且把mid node上移到父节点
		auto parent = SplitNode(tree);
		//Print(root_);
		while (parent->child_num > hierarchy_)
		{
			parent = SplitNode(parent);
		}

	}

}


void B_Tree::InsertValToKeyList(B_Node *node, int val)
{
	B_KEY *tmp_node = new B_KEY();
	if (node->key_num == 0)
	{
		tmp_node->key = val;
		node->key_list[0] = tmp_node;
		node->key_num++;
		return;
	}

	for (int i = 0; i < node->key_num; i++)
	{
		if (val > node->key_list[i]->key)
		{
			continue;
		}
		else
		{
			int pre_key = node->key_list[i]->key;
			node->key_list[i]->key = val;
			for (int j = i+1; j < node->key_num + 1; j++)
			{
				int tmp = pre_key;
				if (j != node->key_num)
					pre_key = node->key_list[j]->key;
				else
					node->key_list[j] = tmp_node;
				node->key_list[j]->key = tmp;
			}
			node->key_num++;
			return;
		}
	}
	node->key_list[node->key_num] = tmp_node;
	node->key_list[node->key_num]->key = val;
	node->key_num++;
	return;
}