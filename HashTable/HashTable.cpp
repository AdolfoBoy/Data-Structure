#include <iostream>
#include <vector>
#include <string>
using namespace std;

enum Status  
{
	EMPTY,  
    EXIST,  
    DELETE,
};  
  
template<class K,class V>
struct KVNode  
{
	K _key;  
    V _value;  
    Status _status;  
    KVNode(const K& key=K(),const V& value=V()) 
		:_key(key)  
        ,_value(value)  
        ,_status(EMPTY)  
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

template<class K,class V,class HashFunc=__HashFunc<K>>  
class HashTable 
{  
	typedef KVNode<K,V> Node;  
public:  
	HashTable()  
		:_size(0)  
	{  
		_tables.resize(_GetPrime(0));  
	}  
	
	HashTable(const HashTable<K,V,HashFunc>& hash)  
	{  
		size_t size=hash._tables.size();  
        _tables.resize(size);  
        for (size_t i=0;i<_tables.size();++i)  
        {  
			if (hash._tables[i]._status == EXIST)  
			{  
				_tables[i]._key=hash._tables[i]._key;  
                _tables[i]._value=hash._tables[i]._value;  
                _tables[i]._status=hash._tables[i]._status;  
			}  
		}  
		_size=hash._size;
	}  
	
	HashTable<K,V,HashFunc>& operator=(const HashTable<K,V,HashFunc>& hash)  
	{  
		if (this != &hash)  
		{  
			HashTable<K,V,HashFunc> tmp(hash);  
			_Swap(tmp);  
		}  
		return *this;  
	}  
public:  
	bool Insert(const K& key,const V& value)  
	{  
		_CheckSize();  
        size_t index=_GetPosition(key);  
        while (_tables[index]._status == EXIST)  
		{  
			if (_tables[index]._key == key)  
			{  
				return false;
			}  
			++index;  
            if(index == _tables.size())  
				index=0;  
		}  
		_tables[index]._key=key;
		_tables[index]._value=value;  
		_tables[index]._status=EXIST;  
        _size++;  
        return true;  
	}  
    
	void Remove(const K& key)       //伪删除法  
	{  
		Node *ret=Find(key);  
		if (ret != NULL)  
		{  
			ret->_status=DELETE;  
			_size--;  
		}  
	}  
	
	Node *Find(const K& key)  
	{  
		size_t index=_GetPosition(key);  
        size_t src=index;  
        while (_tables[index]._status != EMPTY)  
        {  
			if (_tables[index]._key == key)  
            {  
				if (_tables[index]._status == DELETE) 
					return NULL;  
                else  
					return &_tables[index];  
			}  
            ++index;  
            if(index == src)  
				break;  
		}  
		return NULL;
	}  
	
	void Print()  
	{  
		for (size_t i=0;i<_tables.size();++i)  
		{  
			if (_tables[i]._status == EXIST)  
			{  
				cout<<_tables[i]._key<<"  "<<_tables[i]._status<<endl;  
			}
		}  
		cout<<endl; 
	}  
protected:  
	void _CheckSize()  
    {  
		if (_tables.size() == 0 || _size*10/_tables.size() >= 8)  
        {  
			size_t NewSize=_GetPrime(_tables.size());  
            HashTable<K,V,HashFunc> hashTable;         //现代的写法  
            hashTable._tables.resize(NewSize);  
            for (size_t i=0;i<_tables.size();++i)  
			{  
				if(_tables[i]._status == EXIST)  
				{  
					hashTable.Insert(_tables[i]._key,_tables[i]._value);  
				}  
			}   
			_Swap(hashTable);  
		}  
	}  
	
	void _Swap(HashTable<K,V,HashFunc> hash)
	{  
		_tables.swap(hash._tables);  
		swap(_size,hash._size);  
	}  
	
	size_t _GetPrime(size_t num)  
	{  
		const int _PrimeSize= 28;  
       //素数表  
       static const unsigned long _PrimeList[_PrimeSize] =  
	   {  
		   53ul, 97ul, 193ul, 389ul, 769ul,  
           1543ul, 3079ul, 6151ul, 12289ul, 24593ul,  
           49157ul, 98317ul, 196613ul, 393241ul,  
           786433ul,  
           1572869ul, 3145739ul, 6291469ul, 12582917ul,  
           25165843ul,  
           50331653ul, 100663319ul, 201326611ul, 402653189ul,  
           805306457ul,  
          1610612741ul, 3221225473ul, 4294967291ul  
	   };  
       for (size_t i=0;i<_PrimeSize;++i)  
	   {  
		   if(_PrimeList[i] > num)  
			   return _PrimeList[i];  
		   else
			   continue;  
	   }  
	   return _PrimeList[_PrimeSize-1];  
	}  
	
	size_t _GetPosition(const K& key)        //除留余数法  
	{  
		HashFunc hf;  
		return hf(key)%_tables.size();  
	}  
protected:  
	vector<Node> _tables;  
    size_t _size;  
};  

void test()
{
	HashTable<string,string> ht;
	ht.Insert("dictionary","字典");
	ht.Insert("person","人");
	ht.Insert("fruit","水果");
	ht.Insert("vegetable","蔬菜");
	ht.Print();

	KVNode<string,string> * ret=ht.Find("fruit");
	cout<<ret->_status<<" "<<ret->_value<<endl;

	ht.Remove("fruit");
	ret=ht.Find("fruit");
	cout<<ret;
}

int main()
{
	test();
	getchar();
	return 0;
}
