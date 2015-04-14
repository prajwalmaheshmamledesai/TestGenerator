///////////////////////////////////////////////////////////////////////
// XmlReader.cpp - Parse XML strings                                 //
// ver 2.1                                                           //
// Language:    Visual C++, Visual Studio 2010, SP1                  //
// platform:    Windows 7, Home Premium, SP1   //
// Application: OOD Projects1                                 //
//Author:       Prajwal Mahesh Mamledesai
//				MS Computer Engineering, pmamlede@syr.edu, (315)-600-6662
/////////////////////////////////////////////////////////////////////////
#include<iostream>
#include<fstream>
#include<string>
#include "Insert.h"
#include "WinTools.h"
#include "FileSystem.h"

using namespace WinTools_Extracts;

//<---------------insert the headers in the templates generated-------------->
void Insert::InsertHeader(std::string head, std::string body)
{	File in(head,File::in);
	std::string fileline;
	std::string fileContent;
	std::ifstream headFile(head);
	std::ifstream cppfile(body);
	if(cppfile.is_open())
	{
		while(cppfile.good())
		{
			getline(cppfile,fileline);
			fileContent.append(fileline);
			fileContent.append("\n");
			if(fileline.compare("/*MyHeader*/")==0)
			{	while(in.isGood()){
				std::string line1 = in.getLine();
				fileContent.append(line1);
				fileContent.append("\n");
			}
			}
		}
		cppfile.close();
	}
	std::ofstream writecpp (body, std::ios::out);
	if(writecpp.is_open())
	{
	  writecpp << fileContent.c_str();
	  writecpp.close();
	  std::cout << "File created" << std::endl;
	}
  }

//<--------------insert the body in functions of templates where ever there is /*MyCode*/------->
void Insert::InsertItems(std::string head, std::string body)
{
	std::string fileline;
	std::string fileContent;
	std::ifstream headFile(head);
	headFile.is_open();		
	std::ifstream cppfile(body);
	if(cppfile.is_open())
	{
		while(cppfile.good())
		{
			getline(cppfile,fileline);
			fileContent.append(fileline);
			fileContent.append("\n");
			if(fileline.compare("/*MyCode*/")==0)
			{					
				while(headFile.good()){
				getline(headFile,fileline);
				fileContent.append(fileline);
				fileContent.append("\n");
			}
			}
		}
		cppfile.close();
	}
	std::ofstream writecpp (body, std::ios::out);
	if(writecpp.is_open())
	{
	  writecpp << fileContent.c_str();
	  writecpp.close();
	  std::cout << "File created" << std::endl;
	}
}

//<-------test stub------------->

#ifdef TesttheInsert

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
#endif