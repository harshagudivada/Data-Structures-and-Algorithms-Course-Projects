#include<iostream>
#include<fstream>
#include<string>
#include<sstream>
#include<cstdlib>
#include"heap.h"
using namespace std;
class Node;				//Prototype decalarations
class DecodeRec;
void Scan(fstream&,Node[]);
void Huff(HeapElem**,Node* );
void InorderIndex(HeapElem* ,string ,Node[] );
void InorderPrint(HeapElem* );
void InorderPrint2(DecodeRec* );
void Encode(string ,string,Node[]);
void Decode(string,string);
void Stat(string,string);
class Node			//class node
{
friend void Encode(string,string,Node[]);
public:
	Node()
	  :freq(0),hcode("")
	{
	}
	~Node()
	{
	}
	int getfreq()
	{
		return freq;
	}
	void setfreq(int f)
	{
		freq=f;
	}
	string gethcode()
	{
		return hcode;
	}
	void sethcode(string h)
	{
		hcode=h;
	}
private:
	int freq;
	string hcode;
};

class HuffCodeRec			//class Huffcoderec
{
friend void Decode(string,string);
public:
	HuffCodeRec()
		:asc(0),hcode("")
	{
	}
	~HuffCodeRec()
	{
	}
	void setasc(int a)			//set and get member functions
	{
		asc=a;
	}
	void sethcode(string h)
	{	
		hcode=h;
	}
	
private:
	int asc;
	string hcode;
};

class DecodeRec			//class DecodeRec
{
friend void Decode(string,string);
public:
	DecodeRec(int a,DecodeRec* lp,DecodeRec* rp)
		:asc(a),lPtr(lp),rPtr(rp)
	{
	}
//private:
	int asc;
	DecodeRec* lPtr;
	DecodeRec* rPtr;
};




int main(int argc,char* argv[])		//main function taking command line arguments
{
	Node* ptr= new Node[256];
	stringstream ss;
	string ipst,opst,command;
	ss<<argv[1];
	ss>>command;
	ss.clear();
	ss<<argv[2];
	ss>>ipst;
	ss.clear();
	ss<<argv[3];
	ss>>opst;
	ss.clear();
	fstream f1(&ipst[0],ios::in | ios::out | ios::binary);			//opens input file for scanning
	if(!f1)
	{
		cout<<"File"<<ipst<<" could not be opened\n";exit(1);
	}
	Scan(f1,ptr);		//call scan function
	f1.clear();
	f1.seekg (0, ios::end);
	long length = f1.tellg();
	HeapElem* root=NULL;
	Huff(&root,ptr);		//call Huff function
	f1.close();
	if(command == "-e")			//validating command line arguments
	{
		Encode(ipst,opst,ptr);		//e means call encode function
	}
	else
	{
		if(command == "-d")
		{
			Decode(ipst,opst);		//d means call decode function
		}
		else
		{
			cout<<"Invalid command\n";exit(1);
		}
	}
	delete[] ptr;
	
}
void Stat(string ist,string ost)			//stat function to print statistics of input,compressed files
{
	fstream f1(&ist[0],ios::in | ios::out | ios::binary);
	if(!f1)
	{
		cout<<"File"<<ist<<" not opened\n";exit(1);
	}
	fstream f2(&ost[0],ios::in | ios::out | ios::binary);
	if(!f2)
	{
		cout<<"File"<<ist<<" not opened\n";exit(1);
	}
	f1.seekg(0);
	f1.seekg(0,ios::end);
	int isize=f1.tellg();
	cout<<"No.of bytes in "<<ist<<" ="<<isize<<endl;
	f2.seekg(0);
	f2.seekg(0,ios::end);
	int osize=f2.tellg();
	cout<<"No.of bytes in "<<ost<<" ="<<osize<<endl;
	cout<<"Percentage of Compression= "<<(1-((double)osize)/isize)*100<<"%"<<endl;		//convert to double
	cout<<"Average no.of bits per symbol="<<((double)(8*osize)/isize)<<endl;
	
}
void Scan(fstream& f1,Node ptr[])		//scan function to get frequency information of input file
{
	f1.seekg(0,ios::end);
	if(f1.tellg() == 0)
	{
		cout<<"Input file is empty\n";exit(1);
	}
	char buffer;
	int asc=0;
	f1.seekg(0);
	f1.read((&buffer),sizeof(buffer));
	while(f1 && ( !(f1.eof()) ) )		//scan all the bytes of input file 
	{
		asc=(int)buffer;
		if(asc<0)
		{
			asc=asc+256;
		}
		ptr[asc].setfreq( (ptr[asc].getfreq())+1 );
		f1.read((&buffer),sizeof(buffer));
	}
}

void Huff(HeapElem** rootp,Node nodePtr[])		//huff function to generate huffman codes
{
	BHeap* heapPtr=new BHeap(256);
	HeapElem* troot=NULL;
	HeapElem* temp=NULL;
	int x=0,y=0;
	for(int i=0;i<256;i++)			//make a binary heap with frequency information
	{
		if(nodePtr[i].getfreq() == 0)
		{
			continue;
		}
		temp=new HeapElem(i,nodePtr[i].getfreq());
		heapPtr->Insert(temp);
		delete temp;
	}
	for(int i=0;i<=((heapPtr->getlast())-1);)			//make a binary tree
	{
		temp=new HeapElem(-1,0);
		temp->setblPtr((heapPtr->ExtractMin()));
		temp->setbrPtr((heapPtr->ExtractMin()));
		temp->setfreq((temp->getblPtr()->getfreq())+(temp->getbrPtr()->getfreq()));
		heapPtr->Insert(temp);
		delete temp;
	}
	troot=&((heapPtr->getptr())[0]);
	InorderIndex(troot,"",nodePtr);			//index binary tree to get huff codes
	HeapElem* currentPtr=troot;
	*rootp=troot;
	
}

void InorderIndex(HeapElem* rootPtr,string s1,Node nodePtr[] )		//Inorder index binary tree
{
	string s2="";
	string s3="";
	if(rootPtr != NULL)
	{
		rootPtr->sethcode( (rootPtr->gethcode())+s1);
		if(rootPtr->getblPtr() == NULL && rootPtr->getbrPtr()==NULL )
		{
			nodePtr[(rootPtr->asc)].sethcode(rootPtr->gethcode());
			return;
		}
		InorderIndex(rootPtr->getblPtr(),((rootPtr->gethcode()).append("0")),nodePtr);		//index left child with 0,
		InorderIndex(rootPtr->getbrPtr(),((rootPtr->gethcode().append("1"))),nodePtr);		//right child with 1
	}
	s2.clear();
	s3.clear();
}

void Encode(string ist,string ost,Node nodePtr[])			//encode function
{
	fstream f1(&ist[0],ios::in | ios::out | ios::binary);
	if(!f1)
	{
		cout<<"File"<<ist<<" not opened\n";exit(1);
	}
	ofstream f2(&ost[0],ios::out | ios::binary);
	if(!f2)
	{
		cout<<"File"<<ost<<" not opened\n";exit(1);
	}
	char nrec=(char)0;
	f2.write(reinterpret_cast<const char*>(&nrec),sizeof(char));		//no.of non zero records at top
	int pad=0;
	char padcode;
	char lencode;
	int len=0;
	int ltemp=0;
	char nchar,dump;
	string s1="";
	string temp="";
	int q=0;
	int nzcount=0;
	char casc;
	for(int i=0;i<256;i++)
	{
		if(nodePtr[i].freq != 0)			//write huff codes for only non zero frequency characters
		{
			casc=char(i);
			f2.write(reinterpret_cast<const char*>(&casc),sizeof(char));		//write ascii char
			nzcount++;
			len=((s1=(nodePtr[i].gethcode())).length());
			if(len%8 != 0)
			{
				padcode=(char)(8-(len%8));
			}
			else
			{
				padcode=(char)0;
			}
			f2.write(reinterpret_cast<const char*>(&padcode),sizeof(char));		//write no.of padding bits
			if((int)padcode != 0)
			{
				nchar=(char)((len/8)+1);
			}
			else
			{
				nchar=(char)(len/8);
			}
			f2.write(reinterpret_cast<const char*>(&nchar),sizeof(char));		//write no.of characters used for each huff code
			int loop=(int)nchar;
			ltemp=0;
			while(ltemp<(loop))			//loop through code to convert to characters
			{
				if(s1.length() <8)
				{
					pad=(int)padcode;
					s1.append(pad,'0');
				}
				temp=s1.substr(0,8);
				s1=s1.substr(8);
				q=0;
				for(int j=0;j<8;j++)
				{
					q=q*2;
					if(temp[j] == '1')
					{
						q++;
					}
				}
				temp.clear();
				temp="";
				dump=char(q);
				f2.write((&dump),sizeof(dump));
				ltemp++;
				if( (ltemp == loop-1) && (int)padcode != 0  )
				{
					pad=(int)padcode;
					s1.append(pad,'0');
				}
			}	
		}
	}
	f2.seekp(0);
	dump=(char)(nzcount-1);
	f2.write(reinterpret_cast<const char*>(&dump),sizeof(char));		//modify no of non zero records
	f2.seekp(0,ios::end);
	pad=0;
	char buffer;
	buffer=(char)pad;
	int padbyte=f2.tellp();
	f2.write(reinterpret_cast<const char*>(&buffer),sizeof(char));		//no.of padding bits=0
	s1="";
	temp="";
	int asc=0;
	f1.read((&buffer),sizeof(buffer));
	while(f1 && ( !(f1.eof()) ) )		//scan through file to encode
	{
		asc=(int)buffer;
		if(asc<0)
		{
			asc=asc+256;
		}
		s1+=nodePtr[asc].gethcode();
		q=0;
		if(s1.length() > 8)
		{
			temp=s1;
			s1=s1.substr(0,8);			//convert hcode to char and write it to output file
			for(int j=0;j<8;j++)
			{
				q=q*2;
				if(s1[j] == '1')
				{
					q++;
				}
			}
			s1.clear();
			s1="";
			dump=char(q);
			f2.write((&dump),sizeof(dump));
			s1=temp.substr(8);
		}
		f1.read((&buffer),sizeof(buffer));
	}
	if(s1 != "")			//append padding bits at the end
	{
		pad=8-(s1.length());
		s1.append(pad,'0');
		q=0;
		for(int j=0;j<8;j++)
		{
			q=q*2;
			if(s1[j] == '1')
			{
				q++;
			}
		}
		s1.clear();
		s1="";
		dump=char(q);
		f2.write((&dump),sizeof(dump));
		f2.seekp(padbyte,ios::beg);
		dump=(char)pad;
		f2.write(reinterpret_cast<const char*>(&dump),sizeof(char));
	}
	f1.close();
	f2.close();
	Stat(ist,ost);
	
}

void Decode(string ist,string ost)			//decode function
{
	ifstream ifs(&ist[0],ios::in | ios::binary);
	if(!ifs)
	{
		cout<<"File "<<ifs<<" couldn't be opened for reading\n";exit(1);
	}
	ofstream ofs(&ost[0],ios::out | ios::binary);
	if(!ofs)
	{
		cout<<"File "<<ofs<<" couldn't be opened for writing\n";exit(1);
	}
	char buffer;
	string s1="";
	int nzcount=0;
	ifs.read(reinterpret_cast<char*>(&buffer),sizeof(char));
	nzcount=(int)buffer;
	if(nzcount<0)
	{
		nzcount=nzcount+256;
	}
	nzcount++;
	int nztemp=0;
	int asc=0;
	int padrec=0;
	int block=0;
	int pad=0;
	int q=0;
	int tblock=0;
	stringstream ss;
	int r=0;
	string rst="";
	string temp="";
	string empty="";
	int len=0;
	int ltemp=0;
	DecodeRec* currentPtr=NULL;
	DecodeRec* prevPtr=NULL;
	DecodeRec* newPtr=NULL;
	DecodeRec* rootPtr=new DecodeRec(-1,0,0);
	while(nztemp<nzcount)			//scan huffman table to recreate a binary tree
	{
		ifs.read(reinterpret_cast<char*>(&buffer),sizeof(char));		//get ascii char
		asc=(int)buffer;
		if(asc<0)
		{
			asc=asc+256;
		}
		ifs.read(reinterpret_cast<char*>(&buffer),sizeof(char));		//get no of pad bits
		padrec=(int)buffer;
		ifs.read(reinterpret_cast<char*>(&buffer),sizeof(char));
		block=(int)buffer;
		tblock=0;
		s1="";
		while(tblock<block)						//loop through huff code to make binary tree
		{
			ifs.read(reinterpret_cast<char*>(&buffer),sizeof(char));
			q=(int)buffer;
			if(q<0)
			{
				q=q+256;
			}
			temp="";
			while(q!=0)
			{
				r=q%2;
				ss<<r;
				ss>>rst;
				ss.clear();
				temp=rst+temp;
				q=q/2;
			}
			if(temp.length() != 8)
			{
				empty.append(8-temp.length(),'0');
			}
			temp=empty+temp;
			empty="";
			s1=s1+temp;
			tblock++;
		}
		len=s1.length();
		s1.erase(len-padrec);
		len=len-padrec;
		prevPtr=currentPtr=rootPtr;
		ltemp=0;
		while(ltemp<len)				//loop through huff code to make binary tree
		{
			if(s1[ltemp]=='0')			//if 0 extend btree to left
			{
				if(currentPtr != NULL)
				{
					prevPtr=currentPtr;
					currentPtr=currentPtr->lPtr;
				}
				if(currentPtr == NULL)
				{
					if(ltemp==(len-1))
					{
						newPtr=new DecodeRec(asc,0,0);
					}
					else
					{
						newPtr=new DecodeRec(-1,0,0);
					}
					prevPtr->lPtr=newPtr;
					currentPtr=newPtr;
				}
			}
			if(s1[ltemp]=='1') 		//if 1 extend btree to left
			{
				if(currentPtr != NULL)
				{
					prevPtr=currentPtr;
					currentPtr=currentPtr->rPtr;
				}
				if(currentPtr == NULL)
				{
					if(ltemp==(len-1))
					{
						newPtr=new DecodeRec(asc,0,0);			//set ascii valuse at leaves
					}
					else
					{
						newPtr=new DecodeRec(-1,0,0);
					}
					prevPtr->rPtr=newPtr;
					currentPtr=newPtr;
				}
			}
			ltemp++;
		}
		nztemp++;
	}
	temp="";
	s1="";
	char main;
	char wbuffer;
	ifs.read(reinterpret_cast<char*>(&buffer),sizeof(char));
	pad=(int)buffer;
	ifs.read(reinterpret_cast<char*>(&buffer),sizeof(char));			//maintain 2 buffers to scan compressed files
	main=buffer;
	ifs.read(reinterpret_cast<char*>(&buffer),sizeof(char));
	currentPtr=prevPtr=rootPtr;
	empty="";
	while(ifs && ( !(ifs.eof()) ) )			//scan compressed file
	{
		s1="";
		asc=(int)main;
		if(asc<0)
		{
			asc=asc+256;
		}
		q=asc;		
		while(q!=0)
		{
			r=q%2;
			ss<<r;
			ss>>rst;
			ss.clear();
			s1=rst+s1;
			q=q/2;
		}
		if(s1.length() != 8)
		{
			empty.append(8-s1.length(),'0');
		}
		s1=empty+s1;			//2 statements 
		s1=temp+s1;
		empty="";
		len=s1.length();
		ltemp=0;
		while(ltemp<len)		//scan each byte and get its bits and parse the tree 
		{
			if(s1[ltemp] == '0')
			{
				if(currentPtr != NULL)
				{
					prevPtr=currentPtr;
					currentPtr=currentPtr->lPtr;
				}
				if(currentPtr->lPtr == NULL)			//when leaf is reached,code is completed
				{
					wbuffer=(char)(currentPtr->asc);
					ofs.write(reinterpret_cast<const char*>(&wbuffer),sizeof(char));
					currentPtr=prevPtr=rootPtr;
					//ltemp--;
					if(ltemp == (len-1))
					{	
						main=buffer;
						ifs.read(reinterpret_cast<char*>(&buffer),sizeof(char));
						temp="";
						break;
					}
					else
					{
						temp=s1.substr(ltemp+1);
					}
				}
			}
			if(s1[ltemp]=='1')
			{
				if(currentPtr != NULL)
				{
					prevPtr=currentPtr;
					currentPtr=currentPtr->rPtr;
				}
				if(currentPtr->rPtr == NULL)
				{
					wbuffer=(char)(currentPtr->asc);
					ofs.write(reinterpret_cast<const char*>(&wbuffer),sizeof(char));
					currentPtr=prevPtr=rootPtr;
					if(ltemp == (len-1))
					{	
						main=buffer;
						ifs.read(reinterpret_cast<char*>(&buffer),sizeof(char));
						temp="";
						break;
					}
					else
					{
						temp=s1.substr(ltemp+1);
					}
				}
			}
			ltemp++;
			if(ltemp == len)
			{
				main=buffer;
				ifs.read(reinterpret_cast<char*>(&buffer),sizeof(char));		//read next byte
				temp="";
				if( !ifs || (ifs.eof()) )
				{
					break;
				}
			}
		}
	}
	//len=main.length();
	//main.erase(len-pad+1);
	s1="";
	rst="";
	asc=(int)main;
	if(asc<0)
	{
		asc=asc+256;
	}
	q=asc;		
	while(q!=0)
	{
		r=q%2;
		ss<<r;
		ss>>rst;
		ss.clear();
		s1=rst+s1;
		q=q/2;
	}
	if(s1.length() != 8)
	{
		empty.append(8-s1.length(),'0');
	}
	s1=temp+empty+s1;
	s1.erase(len-pad);
	len=s1.length();
	ltemp=0;
	while(ltemp<len)			//taking care of last byte and its padding bits
	{
		if(s1[ltemp] == '0')
		{
			if(currentPtr != NULL)
			{
				prevPtr=currentPtr;
				currentPtr=currentPtr->lPtr;
			}
			if(currentPtr->lPtr == NULL)
			{
				wbuffer=(char)(currentPtr->asc);
				ofs.write(reinterpret_cast<const char*>(&wbuffer),sizeof(char));		
				currentPtr=prevPtr=rootPtr;
				if(ltemp == (len-1))
				{	
					break;
				}
				else
				{
					temp=s1.substr(ltemp+1);
				}
			}
		}
		if(s1[ltemp]=='1')
		{
			if(currentPtr != NULL)
			{
				prevPtr=currentPtr;
				currentPtr=currentPtr->rPtr;
			}
			if(currentPtr->rPtr == NULL)
			{
				wbuffer=(char)(currentPtr->asc);
				ofs.write(reinterpret_cast<const char*>(&wbuffer),sizeof(char));
				currentPtr=prevPtr=rootPtr;
				if(ltemp == (len-1))
				{	
					break;
				}
				else
				{
					temp=s1.substr(ltemp+1);
				}
			}
		}
		ltemp++;
		
	}
	ifs.close();
	ofs.close();
}



void InorderPrint(HeapElem* rootPtr)
{
	if(rootPtr != NULL)
	{
		InorderPrint(rootPtr->getblPtr());
		cout<<"asc="<<rootPtr->asc<<"hcode="<<rootPtr->gethcode()<<endl;cin.get();
		InorderPrint(rootPtr->getbrPtr());
	}
}

void InorderPrint2(DecodeRec* rootPtr)
{
	cout<<"InorderPrint2\n";cin.get();
	if(rootPtr != NULL)
	{
		InorderPrint2(rootPtr->lPtr);
		cout<<"asc="<<rootPtr->asc<<endl;cin.get();
		InorderPrint2(rootPtr->rPtr);
	}
}


