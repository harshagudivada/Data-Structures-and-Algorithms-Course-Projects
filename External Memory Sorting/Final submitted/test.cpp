#include<iostream>
#include<cstdlib>
#include<sstream>
#include<cstdlib>
#include"fs.h"
using namespace std;
int main(int argc,char* argv[])			//main function  to take command line arguments
{
	if(argc != 5)			//cheack no.of arguments
	{
		cout<<"Invalid no.of arguments\n";exit(1);
	}
	string s1,s2;
	int m=0,k=0;
	stringstream ss;
	ss<<argv[1];
	ss>>s1;
	ss.clear();
	ss<<argv[2];
	ss>>s2;
	ss.clear();
	ss<<argv[3];
	ss>>m;
	ss.clear();
	ss<<argv[4];
	ss>>k;
	ss.clear();
	if( (s1.length()<1) || (s2.length() <1) )		//check validity of arguments
	{
		cout<<"Invalid file names\n";exit(1);
	}
	if( m<1 || k<=1)
	{
		cout<<"Invalid arguments\n";exit(1);
	}
	FileSort<StudentRec> f1(s1,s2,m,k);		//Create a FileSort object
	if(f1.Sort() == 0)			//call sort function and check its return value
	{
		cout<<"Error in Sorting\n";
		exit(1);
	}
	
}
