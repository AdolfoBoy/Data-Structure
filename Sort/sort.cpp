#include <iostream>
#include <assert.h>
#include <time.h>
#include <Windows.h>
using namespace std;

//插入排序类

//直接插入排序
void InsertSort(int *a,int size)
{
	assert(a);
	for(int index=1;index<size;index++)
	{
		int tmp=a[index];
		int pos=index-1;
		while(pos >= 0 && a[pos] > tmp)
		{
			a[pos+1]=a[pos];
			pos--;
		}
		a[pos+1]=tmp;  //注意是pos+1，不是pos
	}
}

//希尔排序
void ShellSort(int *a,int size)
{
	assert(a);
	int gap=size;
	//int gap=size/3;
	while(gap > 1)
	{
		gap=gap/3+1;
		for(int index=gap;index<size;index+=gap)  //index++ 和 index+=gap
		{
			int tmp=a[index];
			int pos=index-gap;
			while(pos>=0 && a[pos] > tmp)
			{
				a[pos+gap]=a[pos];
				pos-=gap;
			}
			a[pos+gap]=tmp;
		}
		//gap--;
	}
}

//选择排序类

//选择排序

void SelectSort(int* a,int size)
{
	assert(a);
	for(int i=0;i<size;i++)
	{
		int minIndex=i;
		for(int j=i+1;j<size;j++)  //j=0 error
		{
			if(a[minIndex] > a[j])
				minIndex=j;
		}
		swap(a[minIndex],a[i]);
	}
}

void SelectSort_OP(int* a,int size)
{
	assert(a);
	int left=0,right=size-1;
	while(left < right)
	{
		for(int i=left;i<=right;i++)
		{
			if(a[i] < a[left])
				swap(a[i],a[left]);
			if(a[i] > a[right])
				swap(a[i],a[right]);
		}
		left++;
		right--;
	}
}

//堆排序
void AdjustDown(int *a,int size,int parent)
{
	size_t child=parent*2+1;
	while(child < size)
	{
		if(child +1 < size && a[child+1] > a[child])
			child++;
		else if(a[child] > a[parent])
		{
			swap(a[child],a[parent]);
			parent=child;
			child=parent*2+1;
		}
		else
		{
			break;
		} 
	}
}

void HeapSort(int* a,int size)
{
	assert(a);
	int n=0;
	//建堆
	for(int i=(size-1)/2;i>=0;i--)
	{
		AdjustDown(a,size,i);
	}
	//排序
	while(n<size)
	{
		swap(a[0],a[size-n-1]);
		n++;
		AdjustDown(a,size-n,0);
	}
}

//快速排序
//前后指针法
int GetMid(int *a,int begin,int end)
{
	int left=begin;
	int right=end-1;
	int key=a[end];
	while(left < right)
	{
		while(left <right && a[left] < key)
			left++;
		while(left < right && a[right] > key)
			right--;
		if(left < right)
			swap(a[left],a[right]);
	}
	if(a[left] > a[end])
	{
		swap(a[left],a[end]);  //a[left]和key不能交换
	    return left;
	}
	else
		return end;
}

void QuickSort1(int *a,int begin,int end)
{
	
	assert(a);
	//int div=GetMid(a,begin,end); bug
	if(begin < end)  //left == right 时，数组只剩一个元素，递归结束
	{
		int div=GetMid(a,begin,end);
		QuickSort1(a,begin,div-1);
		QuickSort1(a,div+1,end);
	}
}

//前后指针法-挖坑法
int GetPit(int *a,int begin,int end)
{
	int left=begin;
	int right=end;
	int key=a[end];
	while(left < right)
	{
		while(left < right && a[left] <= key)
			left++;
		a[right]=a[left];
		while(left <right && a[right] >= key)
			right--;
		a[left]=a[right];
	}
	a[left]=key;
	return left;
}

void  QuickSort2(int *a,int begin,int end)
{
	assert(a);
	/*int pos=GetPit(a,begin,end);*/
	
	if(begin < end)
	{
		int pos=GetPit(a,begin,end);
		QuickSort2(a,begin,pos-1);
		QuickSort2(a,pos+1,end);
	}
}

//三数取中法
int GetMidNum(int *a,int begin,int end)
{
	int mid=begin+(end-begin)/2;
	if(a[begin] < a[end])
	{
		if(a[begin] < a[mid])
			return mid;
		else if(a[end] < a[mid])
			return end;
		else
			return begin;
	}
	else  //a[begin] > a[end]
	{
		if(a[begin] < a[mid])
			return begin;
		else if(a[end] < a[mid])
			return mid;
		else
			return end;
	}
}

int PartSort(int *a,int begin,int end)
{
	int midIndex=GetMidNum(a,begin,end);

	swap(a[midIndex],a[end]);
	int key=a[end];

	int cur=begin;
	int prev=begin-1;
	while(cur < end)
	{
		if(a[cur] < key && ++prev != cur)
			swap(a[cur],a[prev]);
		cur++;
	}
	swap(a[++prev],a[end]);
	return prev;
}

void QuickSort_OP(int *a,int begin,int end)
{
	assert(a);
	int div=PartSort(a,begin,end);
	if(end-begin < 13)
	{
		InsertSort(a+begin,end-begin+1);
	}
	else
	{
		int div=PartSort(a,begin,end);
		QuickSort_OP(a,begin,div-1);
		QuickSort_OP(a,div+1,end);
	}
	
}

//冒泡排序
void BubbleSort(int* a,int size)
{
	assert(a);
	for(int i=0;i<size;i++)
	{
		bool exchange=false;
		for(int j=1;j<size-i;j++)
		{
			if(a[j-1] > a[j])
			{
				swap(a[j-1],a[j]);
				exchange=true;
			}
		} 

		if(exchange == false)
			break;
	}
}

//归并排序
void SectionSort(int *a, int *tmp, int begin1, int end1, int begin2, int end2)
{
	int index = begin1;
	while (begin1 <= end1 && begin2 <= end2)
	{
		if (a[begin1] < a[begin2])
		{
			tmp[index++] = a[begin1++];
		}
		else
		{
			tmp[index++] = a[begin2++];
		}
	}
	if (begin1 <= end1)
	{
		for (int i = begin1; i <= end1; i++)
		{
			tmp[index++] = a[begin1++];
		}
	}
	if (begin2 <= end2)
	{
		for (int i = begin2; i <= end2; i++)
		{
			tmp[index++] = a[begin2++];
		}
	}
}

void _MergeSort(int *a, int *tmp, int left, int right)
{
	int mid = left + (right - left) / 2;
	if (left < right)
	{
		_MergeSort(a, tmp, left, mid);
		_MergeSort(a, tmp, mid+1, right);
		SectionSort(a, tmp, left, mid, mid+1, right);
		memcpy(a + left, tmp + left, (right - left + 1)*sizeof(int));
	}	
}

void MergeSort(int *a,int size, int left, int right)
{
	assert(a);
	int *tmp = new int[size];
	_MergeSort(a, tmp,left, right);
	delete []tmp;
}

void Print(int *a,int size)
{
	for(int i=0;i<size;i++)
	{
		cout<<a[i]<<" ";
	}
	cout<<endl;
}

void test()
{
	int a[]={2,0,4,9,3,6,8,7,1,5};
    int size=sizeof(a)/sizeof(a[0]);
	/*InsertSort(a,size);
	cout<<"直接插入排序:";
	Print(a,size);*/
	/*ShellSort(a,size);
	cout<<"希尔排序:";
	Print(a,size);*/
	/*QuickSort1(a,0,size-1);
	cout<<"快速排序1(前后指针一般思路):";
	Print(a,size);*/
	/*QuickSort2(a,0,size-1);
	cout<<"快速排序2(前后指针挖坑法):";
	Print(a,size);*/
	/*QuickSort_OP(a,0,size-1);
	cout<<"快速排序3(三数取中法):";
	Print(a,size);*/
	/*SelectSort(a,size);
	cout<<"选择排序:";
	Print(a,size);*/
	/*SelectSort_OP(a,size);
	cout<<"选择排序(优化):";
	Print(a,size);*/
	/*HeapSort(a,size);
	cout<<"堆排序:";
	Print(a,size);*/
	/*BubbleSort(a,size);
	cout<<"冒泡排序:";
	Print(a,size);*/
	MergeSort(a,size,0,size-1);
	cout<<"归并排序:";
	Print(a,size);
}

int main()
{
	test();
	//TimeTest();
	getchar();
	return 0;
}
