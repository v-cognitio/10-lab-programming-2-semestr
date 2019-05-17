#include "pch.h"
#include <iostream>
#include <vector>
#include <algorithm>

template<class T>
struct AVL
{
	T Value;
	AVL *LeftChild;
	AVL *RightChild;
	AVL *Parent;
	int Height;

	AVL(AVL<T> *LChild, AVL<T> *RChild, AVL<T> *Par, T val)
	{
		LeftChild = LChild;
		RightChild = RChild;
		Parent = Par;
		Value = val;
		Height = 1;
	}

	AVL(const AVL &avl)
	{
		LeftChild = avl.LeftChild;
		RightChild = avl.RightChild;
		Value = avl.Value;
		Parent = avl.Parent;
		Height = avl.Height;
	}

	AVL& operator=(const AVL &avl)
	{
		Parent = avl.Parent;
		Value = avl.Value;
		if (Parent->LeftChild == &avl)
		{
			Parent->LeftChild = this;
		}
		else
		{
			Parent->RightChild = this;
		}
		LeftChild = avl.LeftChild;
		RightChild = avl.RightChild;
		Height = avl.Height;
		delete avl;
		return *this;
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

	BST(const BST &otherBst) : BST()
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
		RecountHeights(newNode->Parent);
		CheckTreeCorrectness(newNode->Parent);
	}

	void RecountHeights(AVL<T> *node)
	{
		if (!node)
			return;
		if (!node->LeftChild && !node->RightChild)
			node->Height = 1;
		else
			node->Height = std::max(node->LeftChild ? node->LeftChild->Height : 0, node->RightChild ? node->RightChild->Height : 0) + 1;
		RecountHeights(node->Parent);
	}

	void Remove(const T &val)
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
		{
			root->RightChild->Parent = nullptr;
			root = root->RightChild;
			RecountHeights(root);
		}
		else if(!root->RightChild)
		{
			root->LeftChild->Parent = nullptr;
			root = root->LeftChild;
			RecountHeights(root);
		}
		else
		{
			AVL<T> *newRoot = FindMaximum(root);
			Swap(root, newRoot);
			RemovingNode(newRoot);
		}
		
	}

	void CheckTreeCorrectness(AVL<T> *node)
	{
		if (!node)
			return;
		RecountHeights(node);
		int heightDiff = GetHeightDifference(node);

		if (std::abs(heightDiff) > 1)
		{
			if (heightDiff > 0)
			{
				if (GetHeightDifference(root->RightChild) == -1)
				{
					BigLeftRotate(root);
				}
				else
				{
					SmallLeftRotate(root);
				}
			}
			else
			{
				if (GetHeightDifference(root->LeftChild) == 1)
				{
					BigRightRotate(root);
				}
				else
				{
					SmallRightRotate(root);
				}
			}
			return;
		}
		CheckTreeCorrectness(node->Parent);
	}

	int GetHeightDifference(AVL<T> *node)
	{
		if (!node)
			return 0;
		return (node->RightChild ? node->RightChild->Height : 0) - (node->LeftChild ? node->LeftChild->Height : 0);
	}

	void SmallLeftRotate(AVL<T> *&node)
	{
		AVL<T> *help = node->RightChild;
		node->RightChild = help->LeftChild;
		help->Parent = node->RightChild;
		node->Parent = node->Parent->Parent;
		help->LeftChild = node;
		RecountHeights(node);
		RecountHeights(help);
	}
	void SmallRightRotate(AVL<T> *node)
	{
		AVL<T> *help = node->LeftChild;
		/*node->LeftChild = help->RightChild;
		help->Parent = node;
		help->RightChild = node;*/
		node->LeftChild = help->RightChild;
		help->Parent = node->Parent;
		help->RightChild = node;
		node->Parent = help;
		help->Parent->RightChild = help;
		RecountHeights(node);
		RecountHeights(help);
	}

	void BigRightRotate(AVL<T> *node)
	{
		SmallLeftRotate(node->LeftChild);
		SmallRightRotate(node);
	}

	void BigLeftRotate(AVL<T> *node)
	{
		SmallRightRotate(node->RightChild);
		SmallLeftRotate(node);
	}

	void RemovingNode(AVL<T> *&current)
	{
		if (!current->LeftChild && !current->RightChild)
		{
			if (IsItLeftChild(current, current->Parent))
				current->Parent->LeftChild = nullptr;
			else
				current->Parent->RightChild = nullptr;

			CheckTreeCorrectness(current->Parent);
			delete current;
		}
		else if (current->LeftChild && current->RightChild)
		{
			AVL<T> *swapNode = FindMaximum(current);
			Swap(current, swapNode);
			RemovingNode(swapNode);
		}
		else if (current->RightChild)
		{
			if (IsItLeftChild(current, current->Parent))
			{
				current->Parent->LeftChild = current->RightChild;
			}
			else
			{
				current->Parent->RightChild = current->RightChild;
			}
			current->RightChild->Parent = current->Parent;
			CheckTreeCorrectness(current->Parent);
		}
		else
		{
			if (IsItLeftChild(current, current->Parent))
			{
				current->Parent->LeftChild = current->LeftChild;
			}
			else
			{
				current->Parent->RightChild = current->LeftChild;
			}
			CheckTreeCorrectness(current->Parent);
			current->LeftChild->Parent = current->Parent;
		}
		
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
		if (node->LeftChild)
			node = node->LeftChild;
		else
			return node = node->RightChild;
		while (node->RightChild != nullptr)
		{
			node = node->RightChild;
		}
		return node;
	}

	void Swap(AVL<T> *first, AVL<T> *second)
	{
		T temp = first->Value;
		first->Value = second->Value;
		second->Value = temp;
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
			std::cout << node->Value << " Height : " << node->Height << std::endl;
			ShowTree(node->LeftChild);
			ShowTree(node->RightChild);
		}
	}

	AVL<T>* GetRoot()
	{
		return root;
	}
};

template<class T>
class iterator
{
	friend class BST<T>;
public:
	BST<T> *bst;
	iterator(BST<T> *bstTree)
	{
		bst = bstTree;
	}
	iterator begin()
	{

	}
	iterator end()
	{

	}
	iterator operator++()
	{

	}
	iterator operator++(T)
	{

	}
	iterator operator--()
	{

	}
	iterator operator--(T)
	{

	}
	T operator*()
	{

	}
	bool operator==(const iterator &iter2)
	{

	}
	bool operator!=(const iterator &iter2)
	{

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
	bst.ShowTree(bst.GetRoot());
	bst.Remove(7);
	std::cout << "!!!!\r\n";
	bst.ShowTree(bst.GetRoot());
	bst.Remove(10);
	std::cout << "!!!!\r\n";
	bst.ShowTree(bst.GetRoot());
	bst.Remove(0);
	std::cout << "!!!!\r\n";
	bst.ShowTree(bst.GetRoot());
	bst.Remove(bst.GetRoot()->Value);
	std::cout << "!!!!\r\n";
	bst.ShowTree(bst.GetRoot());*/
}