#include <iostream>
#include <vector>
#include <string>
using namespace std;

template<class K, class V>
struct HashBucketNode
{
	K _key;
	V _value;
	HashBucketNode<K, V>* _next;

	HashBucketNode(const K& key, const V& value)
		:_key(key)
		,_value(value)
		,_next(NULL)
	{}
};

template<class K>  
struct __HashFunc  
{  
	 size_t operator()(const K& key)  
     {  
		 return key;  
     }  
};

//特化  
template<>  
struct __HashFunc<string>  
{  
	static size_t BKDRHash(const char*str)  
    {  
		unsigned int seed= 131;// 31 131 1313 13131 131313  
        unsigned int hash= 0;  
        while(*str)  
        {  
			hash = hash * seed + (*str++);  
		}
		return(hash& 0x7FFFFFFF);  
	}  
	size_t operator()(const string& str)  
	{
		return BKDRHash(str.c_str());  
	}
};

template<class K, class V, class HashFunc = __HashFunc<K> >
class HashBucket
{
	typedef HashBucketNode<K, V> Node;
	typedef HashBucket<K, V> Table;
public:
	HashBucket()
		:_size(0)
	{}

	HashBucket(size_t capacity)
		:_size(0)
	{
		_tables.resize(_GetNextPrime(capacity));
	}

	HashBucket(const Table& t)
		:_size(0)
	{
		_tables.resize(t._tables.size());
		for (size_t i = 0; i < t._tables.size(); ++i)
		{
			Node* cur = t._tables[i];
			while(cur)
			{
				this->Insert(cur->_key, cur->_value);
				cur = cur->_next;
			}
		}
	}

	// ht = ht;
	/*Table& operator=(Table t)
	{
		_tables.swap(t);
		swap(_size, t._size);

		return *this;
	}*/

	Table& operator=(const Table& t)
	{
		if (this != &t)
		{
			Table tmp(t);
			_tables.swap(tmp._tables);
			swap(_size, tmp->_size);
		}

		return *this;
	}

	~HashBucket()
	{
		for (size_t i = 0; i < _tables.size(); ++i)
		{
			Node* cur = _tables[i];
			while (cur)
			{
				Node* del = cur;
				cur = cur->_next;
				delete del;
			}

			_tables[i] = NULL;
		}
	}

	bool Insert(const K& key, const V& value)
	{
		_CheckCapacity();

		size_t index = _GetPosition(key, _tables.size());
		// 检查是否存在
		Node* cur = _tables[index];
		while (cur)
		{
			if (cur->_key == key)
			{
				return false;
			}
			cur = cur->_next;
		}

		// 头插
		Node* tmp = new Node(key, value);
		tmp->_next = _tables[index];
		_tables[index] = tmp;
		++_size;

		return true;
	}

	Node* Find(const K& key)
	{
		size_t index = _GetPosition(key, _tables.size());
		Node* cur = _tables[index];
		while (cur)
		{
			if (cur->_key == key)
				return cur;

			cur = cur->_next;
		}

		return NULL;
	}

	bool Remove(const K& key)
	{
		size_t index = _GetPosition(key, _tables.size());
		Node* cur = _tables[index];
		if (cur == NULL)
			return false;

		// 处理头
		if (cur->_key == key)
		{
			_tables[index] = cur->_next;
			delete cur;
			return true;
		}

		Node* prev = cur;
		cur = cur->_next;
		while (cur)
		{
			if (cur->_key = key)
			{
				prev->_next = cur->_next;
				delete cur;
				return true;
			}
			
			prev = cur;
			cur = cur->_next;
		}
		
		return false;
	}

	void Print()
	{
		for (size_t i = 0; i < _tables.size(); ++i)
		{
			printf("tables[%d]->",i);
			Node* cur = _tables[i];
			while(cur)
			{
				cout<<"["<<cur->_key<<":"<<cur->_value<<"]"<<"->";
				cur = cur->_next;
			}

			cout<<"NULL"<<endl;
		}

		cout<<endl;
	}

protected:
	size_t _GetPosition(const K& key, size_t capacity)
	{
		HashFunc hf;  
        return hf(key)%_tables.size();
	}

	size_t _GetNextPrime(size_t size)
	{
		const int _PrimeSize = 28;
		static const unsigned long _PrimeList [_PrimeSize] =
		{
			53ul,         97ul,         193ul,       389ul,       769ul,
			1543ul,       3079ul,       6151ul,      12289ul,     24593ul,
			49157ul,      98317ul,      196613ul,    393241ul,    786433ul,
			1572869ul,    3145739ul,    6291469ul,   12582917ul,  25165843ul,
			50331653ul,   100663319ul,  201326611ul, 402653189ul, 805306457ul,
			1610612741ul, 3221225473ul, 4294967291ul
		};

		for (size_t i = 0 ; i < _PrimeSize; ++i)
		{
			if (_PrimeList[i] > size)
			{
				return _PrimeList[i];
			}
		}

		return _PrimeList[_PrimeSize-1];
	}

	void _CheckCapacity()
	{
		if (_size == _tables.size())
		{
			size_t nextPrime = _GetNextPrime(_size);
			//HashTable<K, V> tmp(nextPrime);
		
			vector<Node*> newTables;
			newTables.resize(nextPrime);
			for (size_t i = 0; i < _tables.size(); ++i)
			{
				Node* cur = _tables[i];
				while (cur)
				{
					// 摘节点 
					Node* tmp = cur;
					cur = cur->_next;

					// 计算在新表里面的位置，头插
					size_t index = _GetPosition(tmp->_key,
						newTables.size());
					tmp->_next = newTables[index];
					newTables[index] = tmp; 
				}

				_tables[i] = NULL;
			}

			_tables.swap(newTables);
		}
	}

protected:
	vector<Node*> _tables;	// 哈希表
	size_t _size;			// 数据个数
};

void test1()
{
	HashBucket<int, int> ht;
	for (size_t i = 0; i < 53; ++i)
	{
		ht.Insert(i, i);
	}

	ht.Insert(54, 54);
}

void test2()
{
	HashBucket<string, string> ht;
	ht.Insert("dictionary","字典");
	ht.Insert("person","人类");
	ht.Insert("peace","和平");
	ht.Print();
}
int main()
{
	//test1();
	test2();
	getchar();
}
