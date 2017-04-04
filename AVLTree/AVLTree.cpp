#include<iostream>
using namespace std;

template<class K, class V>
struct AVLTreeNode
{
	K _key;
	V _value;
	AVLTreeNode<K, V>* _left;
	AVLTreeNode<K, V>* _right;
	AVLTreeNode<K, V>* _parent;
	int _bf;

	AVLTreeNode(const K& key, const V& value)
		:_key(key)
		, _value(value)
		, _left(NULL)
		, _right(NULL)
		, _parent(NULL)
		, _bf(0)
	{}
};

template < class K, class V>
class AVLTree
{
	typedef AVLTreeNode<K, V> Node;
public:
	AVLTree()
		:_root(NULL)
	{}

	~AVLTree()
	{
		_destroy(_root);
	}
public:
	bool Insert(const K& key, const V& value)
	{
		if (_root == NULL)
		{
			_root = new Node(key, value);
		}
		Node *parent = NULL;
		Node *cur = _root;
		while (cur)
		{
			if (cur->_key > key)
			{
				parent = cur;
				cur = cur->_left;
			}
			else if (cur->_key < key)
			{
				parent = cur;
				cur = cur->_right;
			}
			else
			{
				return false;
			}
		}
		cur =new Node(key, value);
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

		bool IsRotate = false;

		while (parent)
		{
			if (parent->_left == cur)
			{
				parent->_bf--;
			}
			else
			{
				parent->_bf++;
			}

			if (parent->_bf == 0)
			{
				break;
			}
			else if (parent->_bf == 1 || parent->_bf == -1)
			{
				cur = parent;
				parent = cur->_parent;
			}
			else
			{
				IsRotate = true;
				if (parent->_bf == 2)
				{
					if (cur->_bf == 1)
					{
						//left
						_RotateL(parent);
					}
					else
					{
						//right left
						_RotateRL(parent);
					}
				}
				else if (parent->_bf == -2)
				{
					if (cur->_bf == -1)
					{
						//right
						_RotateR(parent);
					}
					else
					{
						//left right
						_RotateLR(parent);
					}
				}
				break;
			}
		}
		if (IsRotate)
		{
			Node *ppNode = parent->_parent;
			if (ppNode == NULL)
			{
				_root = parent;
			}
			else
			{
				if (ppNode->_key < parent->_key)
				{
					ppNode->_right = parent;
				}
				else
				{
					ppNode->_left = parent;
				}
			}
		}
		return true;
	}

	bool IsBalanceTree()
	{
		return _IsBalance(_root);
	}

	void InOrder()
	{
		_InOrder(_root);
		cout << endl;
	}

protected:

	bool _IsBalance(Node* root)
	{
		if (root == NULL)
		{
			return true;
		}
		int lefth = _Height(root->_left);
		int righth = _Height(root->_right);

		int bf = abs(righth - lefth);
		if (bf > 1)
		{
			return false;
		}
		if (bf != abs(root->_bf))
		{
			cout << root->_key << " ";
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
		int lefth = _Height(root->_left);
		int righth = _Height(root->_right);

		return lefth > righth ? lefth + 1 : righth + 1;
	}

	void _RotateL(Node*& parent)
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

		parent->_bf = subR->_bf = 0;
		parent = subR;
	}

	void _RotateR(Node*& parent)
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

		parent->_bf = subL->_bf = 0;
		parent = subL;
	}

	void _RotateRL(Node*& parent)
	{
		_RotateR(parent->_right);
		_RotateL(parent);
	}

	void _RotateLR(Node*& parent)
	{
		_RotateL(parent->_left);
		_RotateR(parent);
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

	void _destroy(Node* root)
	{
		if (root)
		{
			_destroy(root->_left);
			_destroy(root->_right);
			delete root;
			root = NULL;
		}
	}

private:
	Node* _root;
};

void test1()
{
	AVLTree<int, int> at;
	int a[] = { 16, 3, 7, 11, 9, 26, 18, 14, 15};
	for (int i = 0; i < sizeof(a)/sizeof(a[0]); i++)
	{
		at.Insert(a[i], a[i]);
	}
	at.InOrder();
	int ret=at.IsBalanceTree();
	cout<<"IsBalanceTree:?"<<ret<<endl;
}

int main()
{
	test1();
	getchar();
	return  0;
}
