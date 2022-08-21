#include<iostream>
#include<fstream>
#include<string>
#include<cstdlib>
#include<cctype>

using namespace std;

class StudentRec		//StudentRec class
{
friend ifstream &operator>>(ifstream &, StudentRec & );			//friend class declarations 
friend ostream &operator<<(ostream &, StudentRec & );			
public:
	StudentRec(string x,string y)		//constructor
		:name(x),no(y)			 
	{
	}
	StudentRec()			//default constructor
	{
	}
	
	bool operator==(const StudentRec &right)		//comparison operartor overloading
	{
		return (no == right.no);
	}

	bool operator<(const StudentRec &right)
	{
		return (no < right.no);
	}

	bool operator>(const StudentRec &right)
	{
		return (no > right.no);
	}
	
	bool operator<=(const StudentRec &right)
	{
		return (no <= right.no);
	}
	
	
	
private:		//data members
	string name;
	string no;
};

ifstream &operator>>(ifstream &ifs, StudentRec &rec )		//opeartor >> overloading as global function
{
	string s1,s2,s3;
	if( !(ifs.is_open()) )				//check if the file stream is not opened for data extraction
	{
		cout<<"File stream is not opened\n";exit(1);
	}
	getline(ifs,s1);			//getline from the input file
	if(ifs.eof()){ return ifs;}		//eof is set if the above operation fails due to end of file,then return to the calling function
	while(s1.find_first_not_of("\t\f\v\r\n ") == string::npos)		//check for any non blank characterrs,if none is  
	{								//present,it means it is a blnak/empty line(space is important)
		s1.clear();			
		getline(ifs,s1);				//skip the lank line and get another line
		if(ifs.eof()){ return ifs;}
	}
	
	for(int c=0;s1[c]!='\0';)					//loop through the entire line
	{
		if( (isalpha(s1[c])) || (s1[c] == ' ') || (s1[c] == '\t') )		//initial characters should be either" " or tab or alphabet
		{
			while( (s1[c] == ' ') || (s1[c] == '\t') ) c++;		//skip initial spaces and tabs (if exist)
			if(isalpha(s1[c]))			
			{
				s2=s1[c];
				c++;
				while( (isalpha(s1[c])) || (s1[c] == ' ') || (s1[c] == '.') )	//loop through entire student name and make a string
				{
					s2+=s1[c];
					c++;
				}
				if(s1[c] != '\t')		//after name next character chould be tab,else invalid student name
				{
				cout<<"Student name is Invalid1\n";
				exit(1);
				}
			}
			else					//if student name begins with other than alphabets,invalid name 
			{
				cout<<"Student name is Invalid2\n";
				exit(1);
			}
		}
				
		else						//if student name begins with other than alphabets,invalid name 
		{
			cout<<"Student name is Invalid3\n";
			exit(1);
		}
			
		if(s1[c] == '\t')		
		{	
			c++;
			while(s1[c] == '\t') c++;		//skip all tabs before number
			if(isalnum(s1[c]))			//number should contain only alphanumeric
			{
				s1[c]=tolower(s1[c]);		//change to lowercase for uniformity
				s3=s1[c];c++;
				while(isalnum(s1[c]))		//loop through number
				{
					s1[c]=tolower(s1[c]);
					s3+=s1[c];
					c++;
				}
				while( (s1[c] == '\t') || (s1[c] == ' ') ) c++;		//skip spaces at the end
				if(s1[c] != '\0')				//null character shoulld be there after number
				{
					cout<<"Roll No is Invalid1\n";		
					exit(1);
				}
				
						
			}
			else
			{
				cout<<"Roll no invalid2\n";
				exit(1);
			}
					
		}
			
	}	
	StudentRec obj(s2,s3);			//create StudentRec object with s2,s3 as members
	rec=obj;				//assign above object to right operand of  >> operator
	s2.clear();
	s3.clear();				//clear all string and stream objects
	s1.clear();
	return ifs;				//required yto allow concantination
}

ostream &operator<<(ostream &out, StudentRec &rec )			//operator << overloading
{
	if(!out)
	{
		cout<<"Error in the output stream\n";exit(1);		//check if the file stream is not opened for data insertion
	}
	out<<rec.name<<'\t'<<rec.no;
}



/*int main()
{
	int m=38;
	int mtemp=0;
	StudentRec* ptr=new StudentRec[m];
	string s1,s2,s3;
	ifstream ifs("test.txt",ios::in);
	if(!ifs)
	{
		cout<<"File couldn't be not opened\n";exit(1);
	}

	for(int c=0;c<m;c++)
	{
		ifs>>ptr[c];
		cout<<ptr[c]<<endl;
	}
	if(ptr[4] < ptr[5])
	{
		cout<<"True\n";
	}
	else cout<<"False\n";
}*/
