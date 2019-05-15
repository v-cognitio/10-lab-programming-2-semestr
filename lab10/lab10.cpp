#include "pch.h"
#include <iostream>
#include <vector>


	template<class T>
struct AVL
{
	T Value;
	AVL *LeftChild;
	AVL *RightChild;
	AVL *Parent;
	AVL(AVL<T> *LChild, AVL<T> *RChild, AVL<T> *Par, T val)
	{
		LeftChild = LChild;
		RightChild = RChild;
		Parent = Par;
		Value = val;
	}
	AVL(const AVL &avl)
	{
		LeftChild = avl.LeftChild;
		RightChild = avl.RightChild;
		Value = avl.Value;
		Parent = avl.Parent;
	}
};
template<class T, class Compare = std::less<T>>
class BST
{
private:
	size_t nodeCount, dataSize;
	AVL<T> *root;
	AVL<T> **treeData;
	
public:
	BST()
	{
		nodeCount = 0;
		dataSize = 2;
		root = nullptr;
		treeData = new AVL<T>*[2];
		treeData[0] = root;
		treeData[1] = nullptr;
	}

	BST(const BST &) : BST()
	{

	}

	BST& operator=(const BST &)
	{

	}

	bool empty() const
	{
		return !root;
	}

	size_t size() const
	{
		return nodeCount;
	}

	template<class InputIterator>
	void Assign(InputIterator first, InputIterator last)
	{
		nodeCount = 0;
		dataSize = 2;
		root = nullptr;
		treeData = new AVL<T>*[2];
		while (first != last)
		{
			Insert(*first);
			first++;
		}
	}

	void Insert(const T &val)
	{
		if (empty())
		{
			InsertionIntoEmptyTree(val);
		}
		else
		{
			AVL<T> *parentPosition = FindParentPosition(root, nullptr, val);
			if (parentPosition->Value == val)
				throw new std::exception;

			Insertion(parentPosition, val);
		}
		nodeCount++;
	}

	void InsertionIntoEmptyTree(const T &val)
	{
		root = new AVL<T>(nullptr, nullptr, nullptr, val);
		nodeCount++;
		dataSize++;
		return;
	}

	void Insertion(AVL<T> *parent, const T &val)
	{
		AVL<T> *newNode = new AVL<T>(nullptr, nullptr, parent, val);

		if (Compare()(parent->Value, val))
			parent->RightChild = newNode;
		else
			parent->LeftChild = newNode;

		AddNode(newNode);
	}

	void Remove(const T &val)// �� ��������
	{
		AVL<T> *position = FindParentPosition(root, root, val);
		if (position == root && position->Value == val)
		{
			RemovingRoot();
		}
		else
		{
			AVL<T> *current;
			if (position->LeftChild->Value == val)
				current = position->LeftChild;
			else
				current = position->RightChild;

			RemovingNode(current);
		}
	}

	void RemovingRoot()
	{
		if (!root->LeftChild && !root->RightChild)
			throw new std::exception;
		if (!root->LeftChild)
			root = root->RightChild;
		AVL<T> *newRoot = FindMaximum(root);
		Swap(root, newRoot);
		root = newRoot;
	}

	void RemovingNode(AVL<T> *current)
	{
		if (!current->LeftChild && !current->RightChild)
		{
			if (IsItLeftChild(current, current->Parent))
				current->Parent->LeftChild = nullptr;
			else
				current->Parent->RightChild = nullptr;
			delete current;
		}
		else
			Swap(current, FindMaximum(current));
		Remove(current->Value);
	}

	AVL<T>* FindParentPosition(AVL<T> *node , AVL<T> *prevNode, const T &val)
	{
		if (!node || node->Value == val)
			return prevNode;
		else if (Compare()(node->Value, val))
			return FindParentPosition(node->RightChild, node, val);
		else
			return FindParentPosition(node->LeftChild, node, val);
	}

	AVL<T>* FindMaximum(AVL<T> *node)
	{
		if(node->LeftChild)
			node = node->LeftChild;
		while (node->RightChild != nullptr)
		{
			node = node->RightChild;
		}
		return node;
	}

	void Swap(AVL<T> *first, AVL<T> *second)
	{
		//...
	}

	bool FindNode(const T &val)
	{
		AVL<T> *parentPosition = FindParentPosition(root, root, val);
		if (parentPosition->LeftChild->Value == val || parentPosition->RightChild->Value == val)
			return true;
		else
			return false;
	}

	void AddNode(AVL<T> *node)
	{
		AVL<T> **helpData = new AVL<T>*[++dataSize];
		int i = 0;
		for (i ; i < dataSize - 2; i++)
		{
			helpData[i] = treeData[i];
		}
		helpData[i] = node;
		helpData[++i] = nullptr;
		delete[] treeData;
		treeData = helpData;
	}

	bool IsItLeftChild(AVL<T> *child, AVL<T> *parent)
	{
		if (child->Parent->LeftChild == child)
			return true;
		else if (child->Parent->RightChild == child)
			return false;
		else
			throw new std::exception;
	}

	void ShowTree(AVL<T> *node)
	{
		if (node != nullptr)
		{
			std::cout << node->Value << std::endl;
			ShowTree(node->LeftChild);
			ShowTree(node->RightChild);
		}
	}

	AVL<T>* GetRoot()
	{
		return root;
	}
};

int main()
{
	BST<int> bst;
	std::vector<int> vect = { 1, 7, 6, 0, 10, 3 };
	bst.Assign(vect.begin(), vect.end());
	bst.ShowTree(bst.GetRoot());
	/*bst.Remove(6);
	std::cout << "!!!!\r\n";
	bst.ShowTree(bst.GetRoot());
	bst.Remove(3);
	std::cout << "!!!!\r\n";
	bst.ShowTree(bst.GetRoot());*/
	bst.Remove(7);
	std::cout << "!!!!\r\n";
	bst.ShowTree(bst.GetRoot());
}