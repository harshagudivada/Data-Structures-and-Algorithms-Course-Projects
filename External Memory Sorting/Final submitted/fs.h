#include<iostream>
#include<cstdlib>
#include<fstream>
#include<string>
#include<ctime>
#include<sstream>
#include"rec.h"

using namespace std;

template<typename T>		//prototype declarations for classes which are friend functions of earlier classes
class HeapElem;

template<typename T>
class KMerge;

template<typename T>
class BHeap;

template<typename T>
class FileSort;

template<typename T>		//prototypes for swap functions
void swap(T* ,T* );				//swap function for template
template<typename T>
void swap2(HeapElem<T> *,HeapElem<T> *);		//swap for HeapElem


template<typename T>
class HeapElem			//class HeapElem
{
friend class BHeap<T>;		//friend class declarations
friend class KMerge<T>;
friend class FileSort<T>;
public:
	HeapElem(T r,int index)		//constructor
		:rec(r),findex(index)
	{
	}
	HeapElem()		//default constructor
	{;
	}
	
	bool operator<=(const HeapElem& right)		//comparison operator overloading
	{
		return ( rec <= right.rec );
	} 
	bool operator>(const HeapElem& right)
	{
		return ( rec > right.rec );
	}
private:
	T rec;
	int findex;
};
template<typename T>
class BHeap			//class BHeap
{
friend class KMerge<T>;			//friend class declarations
friend class FileSort<T>;
public:
	BHeap(int hsize)		//constructor
		:size(hsize)
	{
		ptr=new HeapElem<T>[hsize];		//create array for heap
		last=-1;				//set last to -1 initially
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
	void Insert(HeapElem<T>* e)			//Insert function
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
	
	HeapElem<T> ExtractMin()		//ExtractMin Function
	{	
		HeapElem<T> temp=ptr[0];		//take the first element from the heap
		ptr[0]=ptr[last];			//take the last element and put it at root of the heap
		last--;				//heap size is reduced by one
		MinHeapify(0);			//call MinHeapify on root
		return temp;			
	}
	
	int min(int x,int y)
	{
		if(ptr[x].rec <= ptr[y].rec) return (x);
		else return (y);
	}
	
private:
	HeapElem<T>* ptr;		//array pointer for heap
	int last;
	int size;
};


template<typename T>
void swap2(HeapElem<T> *x,HeapElem<T> *y)			//swap for HeapElem
{
	HeapElem<T> temp=*x;
	*x=*y;
	*y=temp;
}

template<typename T>
class KMerge				//class Merge
{
friend class FileSort<T>;		//friend class declaration
public:
	KMerge(int k)			//constructor
		:b1(k)			//initialise BHeap data member with size=k
	{
	}
	void AddToMerge(T* recp,int file_index)			//Add to merge function
	{
		HeapElem<T> h1( (*recp),(file_index) );		//Create a heap element and Insert it into the heap
		b1.Insert(&h1);
	}
	int getNextMin(T** recpp)		//getNextMin function
	{
		static HeapElem<T> temp;		//It is declared static because it should not go out of scope after the function
		temp=b1.ExtractMin();			//It is not initialised at the declaration because,in such a way it can't be re-initialised
		(*recpp)=(&(temp.rec));			//change the next min value by pass by reference method
		return (temp.findex);			//return file index of the last extracted record from heap
	}
	
	
private:
	BHeap<T> b1;
};

template<typename T>
class FileSort			//Class FileSort 
{
public:
	FileSort(string IfName,string OfName,int m1,int k1)		//FileSort function
			:ipname(IfName),opname(OfName),m(m1),k(k1)	
	{
	}
	
	bool Sort()			//Sort function
	{
//---------------------------Splitting phase-----------------------------------------------------------------------//
		ifstream ifs(&ipname[0],ios::in);		//open the input file
		if(!ifs)
		{
			cout<<"File couldn't be opened\n";		//return false if input file couldn't be opened
			return false;
		}
		stringstream ss;
		int mTemp=0,filename=1;
		string fname;
		T* A=new T[m];				//Array for Quick sort
		while( !(ifs.eof()) )			//loop till end of the input file
		{
			mTemp=0;		//mTemp for extracting m records from input file
			ss<<filename;
			ss>>fname;
			ss.clear();
			while(mTemp < m)		//extracting m records from input file
			{
				ifs >> A[mTemp];
				if (ifs.eof()) break;		//if end of file is encountered, break the loop of extraction
				mTemp++;
			}
			
			if (ifs.eof()) 				//check if end of file is set
			{
				if(mTemp==0){break;}		//if no records are pending for sorting(mTemp=0),break the loop for Quick sort
				else
				{
					T* Atemp=A;			//if records are pending for sorting
					A=new T[mTemp];
					for(int c=0;c<mTemp;c++)
					{
						A[c]=Atemp[c];		//create a smaller array of records, and copy records from older array
					}
				}
			}
			ofstream ofs(&fname[0],ios::out);		//create a file for inserting the sorted <=m records
			RQSORT(A,0,(mTemp-1));		//call recursive quick sort on the array of <=m records
			int c=0;
			while(c<mTemp)
			{
				ofs<<A[c]<<endl;		//insert sorted array elements into file
				c++;
			}
			filename++;		//increment no.of files
		}
//---------------------------Merging phase-----------------------------------------------------------------------//
	
		filename--;			
		int files=filename;		//variable for no.of files remaining
		k=min(k,filename);		//variable for no.of files to be merged at once
		int krem=k;			
		T temp;				//reccord for  extraction operatoions
		int index=0;			//index of file
		int mergec=0;			//counter for no.of merges
		int kTemp=k;			//counter for merging k files
		int kmin;			//variable for creating required o.of streams
		int nrec=1;			
		bool read=0;			//variable to indicate a file to be merged is read completely
		KMerge<T> k1(k);		//create a KMerge object with size k
		T* rp;				//pointer to the next minimum
		T** rpp=(&rp);			//pointer to rp
		T temp2;			//reccord for  insertion operatoions
		string cmd,fname2;		//for system commands,
		fstream fs;			//stream for new file creation
		while(files>1)
		{
			nrec=1;
			kmin=min(k,files);	//kmin=minimum of existing files and user given k(alredy minimum of user given k and total no.of files
			kTemp=kmin;		
			ifstream* Inst=new ifstream[kmin];		//create required no.of streams for extracting from files
			for(int c=0;c<kmin;c++)		//loop through no.of files to be merged at once
			{	
				ss<<( ((mergec*k)+c+1) );
				ss>>fname;
				ss.clear();
				Inst[c].open(&fname[0]);		//link each stream in the array to a file
				if(!(Inst[c]))
				{
					cout<<"File couldn't be opened\n";return false;	
				}
				Inst[c]>>temp;		//extract from file 
				k1.AddToMerge( (&(temp)), ((mergec*k)+c+1) );		//Add to merge 
			}

			while(1)
			{
				index=k1.getNextMin(rpp);		//modify yhe index and record of latest minimum
				temp2=(*(rp));				
				Inst[(index-(mergec*k)-1)]>>temp;	//extract from file with latest minimum
				if(fs.is_open())
				{
					fs<<temp2<<endl;		//insert the latest minimum into the new merge output file
				}
				
				if( !(Inst[(index-(mergec*k)-1)].eof()) )
				{
					k1.AddToMerge( (&(temp)),(index) );   nrec++;          //Add to merge
				}
				else
				{
					kTemp--;read=0;			//if end of file is reached
					if(kTemp == 0)			//if all files to be merged at once reached their end of file
					{
						mergec++;		//increment no.of merges
						
						for(int c=((mergec*krem)-krem+1);c<=((mergec*krem)-krem+kmin);c++)	//loop through to delete
						{							//latest merged files
							cmd="rm ";
							ss<<c;
							ss>>fname2;
							if(fname2 == fname) break;	//don't delete the merge output file
							cmd+=fname2;
							system(&cmd[0]);	//system command to remove files
							cmd.clear();
							ss.clear();
							fname2.clear();
						}
						files-=k;			//decrement no.of tottal files
						fs.close();			//close the stream so that it can be linked to new file created
						break;
					}
				}
	
				if(  ( (kTemp == kmin) && (read == 0) ) || ( (kTemp == (kmin-1)) && (nrec == 1) ) )    
				{	
					ss<<(filename+mergec+1);		//Here, files will be created if a new merge starts 
					ss>>fname;
					ss.clear();
					fs.open(&fname[0],fstream::out | fstream::in | fstream::app);		//open a merge output file
					if(!fs)
					{		
						cout<<"File couldn't be opened\n";return false;			
					}
					files++;			//increase no.of files
					read=1;				
					fs<<temp2<<endl;		//write latest minimum to the merge output file
				}
			}	
		}
		int result=rename(&fname[0],&opname[0]);		//rename the final merge opuput file
		if(result != 0)
		{
			cout<<"File couldn't be renamed\n";return false;
		}
		
		return true;
	}

	
private:
	string ipname;
	string opname;
	int m;
	int k;
};


template<typename T>
void RQSORT(T A[],int start,int end)		//RQSORT function
{	
	if(start<end)				
	{
		T temp;
		srand(time(0));					//seed random number generator using system time settings
		int i=( (rand()%(end-start+1)) + start );			//randomise the pivot
		swap(&A[i],&A[end]);				//swap the pivot and last element in the sub array
		int lstart=(start-1),lend=(start-1);		//boundaries of smaller array
		for(int c=start;c<=end;c++)			//loop through array
		{
			if(A[c]<=A[end])
			{
				swap(&A[lend+1],&A[c]);		//if a element is smaller than pivot,swap it with head of bigger array
				lend++;
			}
		}
		RQSORT(A,start,lend-1);			//recursively call RQSORT on smaller and larger sub arrays
		RQSORT(A,lend+1,end);
	}
	
} 

template<typename T>
void swap(T* x,T* y)			//swap function for template
{
	T temp=*x;
	*x=*y;
	*y=temp;
}

int max(int x,int y)		//max function
{
	if( x >= y) return x;
	else return y;
}
int min(int x,int y)		//min function
{
	if( x <= y) return x;
	else return y;
}
/*int main()
{
	FileSort<StudentRec> f1("inp2.txt","oup.txt",4,3);	//removing the files may be required (remove function)
}*/

