///////////////////////////////////////////////////////////////////////
// XmlReader.cpp - Parse XML strings                                 //
// ver 2.1                                                           //
// Language:    Visual C++, Visual Studio 2010, SP1                  //
// platform:    Windows 7, Home Premium, SP1   //
// Application: OOD Projects1                                 //
//Author:       Prajwal Mahesh Mamledesai
//				MS Computer Engineering, pmamlede@syr.edu, (315)-600-6662
/////////////////////////////////////////////////////////////////////////



#include "Insert.h"
#include <iostream>
#include <string>


//<-------------it inserts the tokanizer functions inside the generic template--------->
void main()
{
	Insert insert;

	std::string str1 ="../Files/Header.txt";
	std::string str2 ="../Files/TestDriver.cpp";
	std::string str3 ="../Files/Driver.txt";
	std::string str4 ="../Files/Logger.txt";
	std::string str5 ="../Files/Generator.txt";
	std::string str6 ="../Files/Logger.cpp";
	std::string str7 ="../Files/Generator.cpp";

	insert.InsertHeader(str1,str2);
	insert.InsertItems(str3,str2);
	insert.InsertItems(str4,str6);
	insert.InsertItems(str5,str7);

    }
