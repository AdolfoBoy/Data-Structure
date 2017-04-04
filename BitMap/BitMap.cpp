#include <iostream>
#include<vector>
using namespace std; 

class Bitmap
{
public:
	Bitmap()
		: _size(0)
	{}

	Bitmap(size_t size)
		:_size(0)
	{
		_array.resize((size>>5)+1);
	}

	void Set(size_t num)
	{
		size_t index = num >> 5;
		size_t i = num % 32;
		if (_array[index] & (1 << i))
		{
			return;
		}
		else
		{
			_array[index] |= (1 << i);
			_size++;
		}
	}

	void Reset(size_t num)
	{
		size_t index = num >> 5;
		size_t i = num % 32;
		if (_array[index] & (1 << i))
		{
			_array[index] &= ~(1 << i);
			_size--;
		}
		else
		{
			return;
		}
	}

	bool Test(size_t num)
	{
		size_t index = num >> 5;
		size_t i = num % 32;

		return _array[index] & (1 << i);
	}

private:
	vector<size_t> _array;
	size_t _size;
};

int main()
{
	Bitmap bt(62);
	bt.Set(1);
	bt.Set(5);

	bt.Reset(1);

	cout << bt.Test(5) << endl;
	cout << bt.Test(23) << endl;
	getchar();
	return 0;
}
