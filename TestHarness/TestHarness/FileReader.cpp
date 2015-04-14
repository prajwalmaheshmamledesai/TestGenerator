///////////////////////////////////////////////////////////////////////
// FileReader.cpp - Parse XML strings                                 //
// ver 2.1                                                           //
// Language:    Visual C++, Visual Studio 2010, SP1                  //
// platform:    Dell Alienware, Windows 7, Professional   //
// Application: Project 1 OOD                                  //
// Author:      Prajwal Mahesh Mamledesai, Syracuse University,           //
//              Student MS- Computer Engineering , (315) 600-6662      //
///////////////////////////////////////////////////////////////////////
#include <iostream>
#include <fstream>
#include "FileReader.h"
#include "FileMgr.h"


//<------------------gets the file and generates the data and stores into string and returns that string--------> 
std::string FileGenerator :: getFile(std :: string& input)
{	value.clear();
	int position=0;
	std::fstream var;
	var.open(input);
    std::ifstream s(input);
	while(!var.eof()){
		value.push_back((char) var.get());
		var.seekg(++position);
	}
		std :: cout << value;
	
	return value;
}

//<--------------Test Stub-------------------------->

#ifdef  Test FileReader

int main()
{

	std::string input;
	std::cin>>input;
	std::string input1;
    std::cin>>input1;
	FileGenerator files;
	std::string str = files.getFile(input);
	std::string str1 = files.getFile(input1);
	std::cout<<str;
	std::cout<<str1;
	if(str1.compare(str)==0)
	   std::cout<<"true";
	else
		std::cout<<"false";
}

#endif



