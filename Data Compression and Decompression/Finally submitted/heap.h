#include<iostream>
#include<fstream>
#include<string>
#include<sstream>
#include<cstdlib>
using namespace std;

class HeapElem;	
void swap2(HeapElem*,HeapElem*);		//swap for HeapElem

class HeapElem			//class HeapElem
{
friend class BHeap;		//friend class declarations
public:
	HeapElem(int a,int f)		//constructor
		:asc(a),freq(f),hcode(""),blPtr(0),brPtr(0)
	{
	}
	HeapElem()		//default constructor
	{;
	}
	
	bool operator<=(const HeapElem& right)		//comparison operator overloading
	{
		return ( freq <= right.freq );
	} 
	bool operator>(const HeapElem& right)
	{
		return ( freq > right.freq );
	}
	void setfreq(int f)
	{
		freq=f;
	}
	int getfreq()
	{
		return freq;
	}
	HeapElem* getblPtr()
	{
		return blPtr;
	}
	HeapElem* getbrPtr()
	{
		return brPtr;
	}
	void setblPtr(HeapElem* ptr)
	{
		blPtr=ptr;
	}
	void setbrPtr(HeapElem* ptr)
	{
		brPtr=ptr;
	}
	void sethcode(string h)
	{
		hcode=h;
	}
	string gethcode()
	{
		return hcode;
	}
	
//private:
	int asc;
	int freq;
	string hcode;
	HeapElem* blPtr;
	HeapElem* brPtr;
};

class BHeap			//class BHeap
{
public:
	BHeap(int hsize)		//constructor
		:size(hsize)
	{
		ptr=new HeapElem[hsize];		//create array for heap
		last=-1;				//set last to -1 initially
	}
	~BHeap()
	{
		delete[] ptr;
	}
	void MinHeapify(int k)
	{
		int v=0;
		if(last <= 0)return;		//if there are zero or only one element in heap, furthur hepaification is not required,return
		if(last == 1)v=1;		//if only two elements,consider the child 
		else
		{
			if( ((2*k) + 1)<=last)		//check for existence of left child
			{
				if( ((2*k) + 2)<=last)		//check for existence of right child
				{
					v=min( ((2*k) + 1),((2*k) + 2) );		//if both children are present,consider the minium for comparison
				}							//with parent
				else
				{
					v= ( (2*k) + 1 );	//if only left child exists, consider it foe comparison
				}
			}
			else			//if no children,return
			{	
				return;
			}
		}
		if(ptr[k] <= ptr[v])	
		return;
		else
		{					//if parent > child,swap them
			swap2(&ptr[k],&ptr[v]);
			MinHeapify(v);		//call minheapify on present child
		}
	}
	void Insert(HeapElem* e)			//Insert function
	{
		if(last<size)				//check if heap is alredy full
		{
			ptr[last+1]=(*e);		//first,insert at the end of heap
			last++;
			int temp=last;
			while( ptr[temp/2] > ptr[temp] )		//percolate up if parent > child
			{
				swap2(&ptr[temp/2],&ptr[temp]);
				temp=(temp/2);
			}
		}

	}
	
	HeapElem* ExtractMin()		//ExtractMin Function
	{	
		HeapElem* temp=new HeapElem(ptr[0]);
		//HeapElem temp=ptr[0];		//take the first element from the heap
		ptr[0]=ptr[last];			//take the last element and put it at root of the heap
		last--;				//heap size is reduced by one
		MinHeapify(0);			//call MinHeapify on root
		return temp;			
	}
	
	int min(int x,int y)
	{
		if(ptr[x].freq <= ptr[y].freq) return (x);
		else return (y);
	}
	HeapElem* getptr()
	{
		return ptr;
	}
	int getlast()
	{
		return last;
	}
private:
	HeapElem* ptr;		//array pointer for heap
	int last;
	int size;
};

void swap2(HeapElem *x,HeapElem *y)			//swap for HeapElem
{
	HeapElem temp=*x;
	*x=*y;
	*y=temp;
}
