#include <iostream>
#include <vector>
using namespace std;

template <class T>
struct Less
{
	bool operator()(const T& l,const T& r)
	{
		return l < r;
	}
};

template <class T>
struct Greater
{
	bool operator()(const T& l,const T& r)
	{
		return l > r;
	}
};

template <class T,class Container=Greater<T>>
class _heap
{
	Container _con;
public:
	_heap()
	{

	}

	_heap(T* a,size_t sz)
	{
		for(int i=0;i<sz;i++)
		{
			_a.push_back(a[i]);
		}
	}

	~_heap()
	{

	}
	void _Max_Heap()
	{
		for(int i=(_a.size()-2)/2;i>=0;i--)
		{
			_AdjustDown(i);
		}
	}

	void _Min_Heap()
	{
		for(int i=1;i<_a.size();i++)
		{
			_AdjustUp(i);
		}
	}

	//堆排序（升序排序） 
	void _Heap_Sort()
	{
		//建大堆（已经建好），堆顶元素和堆底元素交换，重新调整再次建堆
		for(int i=(_a.size()-2)/2;i>=0;i--)
		{
			_AdjustDown(i);
		}
		for(int i=0;i<_a.size();i++)
		{
			swap(_a[0],_a[_a.size()-1-i]);
			_AdjustDown_Sort(_a.size()-1-i,0);
		}
	}

	void Push(const T& x)
	{
		_a.push_back(x);
		_AdjustUp(_a.size()-1);
	}

	void Pop()
	{
		if(!_a.empty())
		{
			swap(_a[0],_a[_a.size()-1]);
			_a.pop_back();
			_AdjustDown(0);
		}
	}

	bool Empty()
	{
		return _a.empty();
	}

	size_t size()
	{
		return _a.size();
	}
protected:
	void _AdjustDown(size_t parent)
	{
		size_t child=parent*2+1;
		while(child <_a.size())
		{
			if(child+1 < _a.size() && _con(_a[child+1],_a[child]))
			{
				child++;
			}
			else if(_con(_a[child],_a[parent]))
			{
				swap(_a[parent],_a[child]);
				parent=child;
				child=parent*2+1;
			}
			else
			{
				break;
			}
		}
	}

	void _AdjustUp(size_t child)
	{
		size_t parent=(child-1)/2;
		while(child > 0)
		{
			if(_con(_a[parent],_a[child]))
			{
				swap(_a[child],_a[parent]);
				child=parent;
				parent=(child-1)/2;
			}
			else
			{
				break;
			}
		}
	}

	void _AdjustDown_Sort(size_t sz,size_t parent)
	{
		size_t child=parent*2+1;
		while(child < sz)
		{
			if(child+1 < sz && _a[child+1] > _a[child])
			{
				child++;
			}
			else if(_a[parent] < _a[child])
			{
				swap(_a[child],_a[parent]);
				parent=child;
				child=parent*2+1;
			}
			else
			{
				break;
			}
		}
	}
private:
	vector<T> _a;  
};
