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
	//�ҵ�Ҷ�ӽڵ�
	auto leaf = SearchLeaf(root_, val);
	//���뵽Ҷ�ӽڵ�
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
	//Ҷ�ӽڵ�
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

		//���һ��childָ�����Ѱ��
		return  SearchLeaf((tree->child_list[tree->child_num-1]), val);
	}
}


/*
1.ȡ��ǰ�ڵ�key���е��м�ڵ�mid
2.���ѳ���������
3.mid�����𸸽ڵ�
4.���ظ��ڵ�
*/
B_Node * B_Tree::SplitNode(B_Node*tree)
{
	B_Node*parent = tree->parent;
	//�����ǰ�ڵ�Ϊ���ڵ㣬�½�һ���µĸ��ڵ㣬��ǰ�ڵ���ѳ������ڵ�
	if (parent == nullptr)
	{
		//�����µĸ��ڵ�
		parent = new B_Node(hierarchy_);
		root_ = parent;
		parent->child_list[0] = tree;
		tree->parent = parent;
		parent->child_num++;
	}

	auto mid_key = tree->key_list[tree->key_num / 2];

	//����mid key�����ڵ�
	InsertValToKeyList(parent, mid_key->key);


	//����tree���ֵܽڵ�bro
	//tree���ѳ�һ���bro
	B_Node *bro = new B_Node(hierarchy_);
	//mid kid�Ƶ����ڵ�
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
	//Ҷ�ӽ��δ�� ֱ�Ӳ���
	if (tree->key_num < hierarchy_ - 1)
	{
		return InsertValToKeyList(tree, val);
	}
	else{
		
		//�Ƚ�Ҷ�ӽڵ����
		InsertValToKeyList(tree, val);

		//����Ҷ�ӽڵ�,���Ұ�mid node���Ƶ����ڵ�
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