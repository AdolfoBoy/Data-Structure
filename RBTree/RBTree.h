#include<iostream>
using namespace std;

enum Colour
{
	RED,
	BLACK,
};

template<class K, class V>
struct RBTreeNode
{
	K _key;
	V _value;
	RBTreeNode<K, V>* _left;
	RBTreeNode<K, V>* _right;
	RBTreeNode<K, V>* _parent;
	Colour _col;

	RBTreeNode(const K& key, const V& value, Colour col = RED)
		:_key(key)
		, _value(value)
		, _left(NULL)
		, _right(NULL)
		, _parent(NULL)
		, _col(col)
	{}
};

template<class K, class V>
class RBTree
{
	typedef RBTreeNode<K, V> Node;
public:
	RBTree()
		:_root(NULL)
	{}

	bool Insert(const K& key, const V& value)
	{
		if (_root == NULL)
		{
			_root = new Node(key, value, BLACK);
			return true;
		}
		Node* parent = NULL;
		Node* cur = _root;
		while (cur)
		{
			if (key < cur->_key)
			{
				parent = cur;
				cur = cur->_left;
			}
			else if (key > cur->_key)
			{
				parent = cur;
				cur = cur->_right;
			}
			else
			{
				return false;
			}
		}

		cur = new Node(key, value, RED);
		if (parent->_key > key)
		{
			parent->_left = cur;
			cur->_parent = parent;
		}
		else
		{
			parent->_right = cur;
			cur->_parent = parent;
		}


		while (cur != _root && parent->_col == RED)
		{
			Node *grandfather = parent->_parent;
			if (parent == grandfather->_left)
			{
				Node *uncle = grandfather->_right;

				if (uncle && uncle->_col == RED)
				{
					parent->_col = uncle->_col = BLACK;
					grandfather->_col = RED;

					cur = grandfather;
					parent = cur->_parent;
				}
				else 
				{
					if (cur == parent->_right)
					{
						_RotateL(parent);
						swap(cur, parent);
					}

					_RotateR(grandfather);
					parent->_col = BLACK;
					grandfather->_col = RED;
				}
			}	
			else //parent == grandfather->right
			{
				Node *uncle = grandfather->_left;
				if (uncle && uncle->_col == RED)
				{
					parent->_col = uncle->_col = BLACK;
					grandfather->_col = RED;

					cur = grandfather;
					parent = cur->_parent;
				}
				else 
				{
					if (cur == parent->_left)
					{
						_RotateR(parent);
						swap(cur, parent);
					}

					_RotateL(grandfather);
					parent->_col = BLACK;
					grandfather->_col = RED;
				}
			}
		}
		_root->_col = BLACK;
		return true;
	}

	void InOrder()
	{
		_InOrder(_root);
		cout << endl;
	}

	bool IsBalanceTree()
	{
		return _IsBalance(_root);
	}

protected:
	bool _IsBalance(Node* root)
	{
		if (root == NULL)
		{
			return true;
		}

		int lheight = _Height(root->_left);
		int rheight = _Height(root->_right);

		int bf = abs(rheight - lheight);
		if (bf > 1)
		{
			return false;
		}

		return _IsBalance(root->_left) && _IsBalance(root->_right);
	}

	int _Height(Node *root)
	{
		if (root == NULL)
		{
			return 0;
		}

		int lheight = _Height(root->_left) + 1;
		int rheight= _Height(root->_right) + 1;

		return lheight > rheight ? lheight : rheight;
	}

	void _RotateL(Node* parent)
	{
		Node *subR = parent->_right;
		Node *subRL = subR->_left;
		parent->_right = subRL;
		if (subRL != NULL)
		{
			subRL->_parent = parent;

		}
		subR->_left = parent;
		subR->_parent = parent->_parent;
		parent->_parent = subR;
		parent = subR;
 		if (parent->_parent == NULL)
		{
			_root = parent;
		}
		else
		{
			if (parent->_parent->_key < parent->_key)
			{
				parent->_parent->_right = parent;
			}
			else
			{
				parent->_parent->_left = parent;
			}
		}
	}

	void _RotateR(Node* parent)
	{
		Node *subL = parent->_left;
		Node *subLR = subL->_right;
		parent->_left = subLR;
		if (subLR != NULL)
		{
			subLR->_parent = parent;
		}
		subL->_right = parent;
		subL->_parent = parent->_parent;
		parent->_parent = subL;
		parent = subL;
		if (parent->_parent == NULL)
		{
			_root = parent;
		}
		else
		{
			if (parent->_parent->_key < parent->_key)
			{
				parent->_parent->_right = parent;
			}
			else
			{
				parent->_parent->_left = parent;
			}
		}
	}

	void _InOrder(Node *root)
	{
		if (root == NULL)
		{
			return;
		}
		_InOrder(root->_left);
		cout << root->_key << " ";
		_InOrder(root->_right);
	}
private:
	Node* _root;
};

