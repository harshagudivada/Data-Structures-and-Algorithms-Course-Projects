#include<iostream>
#include<cstdlib>
#include<ctime>
#include<fstream>
#define N 10000000
using namespace std;
int main()		//program to create inpur files
{
	srand(time(NULL));
	char buffer;
	int asc=0;
	ofstream ofs;
	ofs.open("File1",ios::out | ios::binary);	//File1 uniform code,so not much compression,furthur -ve compression may happen
	if(!ofs)
	{
		cout<<"File1 couldn't be opened\n";
	}
	else
	{
		for(int i=0;i< N ;i++)
		{
			asc=(rand()%256);
			buffer=(char)asc;
			ofs.write(reinterpret_cast<const char*>(&buffer),sizeof(char));
		}
	}
	ofs.close();
	int j=0;
	unsigned m=0;
	int b=7;
	unsigned mask=1<<7;
	int c=0;
	ofs.open("File2",ios::out | ios::binary);			//File2 skewed data input with varying frequencies
	if(!ofs)
	{
		cout<<"File1 couldn't be opened\n";
	}
	else
	{
		for(int i=0;i<N;i++)
		{
			j=rand()%32;
			m=rand()%256;
			while((!(m & mask)) && (m!=0)  )
			{
				m<<=1;
				b--;
			}
			if(m==0) b=0;
			 c=(32*b)+j;
			 buffer=(char)c;
			 ofs.write(reinterpret_cast<const char*>(&buffer),sizeof(char));
			 b=7;
		}
	}
	ofs.close();
}
