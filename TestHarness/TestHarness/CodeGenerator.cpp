///////////////////////////////////////////////////////////////////////
// XmlReader.cpp - Parse XML strings                                 //
// ver 2.1                                                           //
// Language:    Visual C++, Visual Studio 2010, SP1                  //
// platform:    Toshiba Portege R705, Windows 7, Home Premium, SP1   //
// Application: OOD 2012 Project1                                 //
//Author:       Prajwal Mahesh Mamledesai
//				MS Computer Engineering, pmamlede@syr.edu, (315)-600-6662
///////////////////////////////////////////////////////////////////////
#include "CodeGenerator.h"
#include<iostream>
#include<fstream>
#include<string>
#include "WinTools.h"
#include "FileSystem.h"

using namespace WinTools_Extracts;


//<------Class function which will generate the templates----------->

void CodeGenerator::codeGenerator(std::string head, std::string body)
{
File in(head,File::in);
 std:: ofstream out( body,std::ios::app);

while(in.isGood())
	{
		std:: ofstream out(body,std::ios::app);
		std::string temp = in.getLine();
		out << temp << "\n";
		
	}

}

//<-----------test stub--------->
#ifdef  TestCodeGenerator
void main(){

	CodeGenerator coGen;
std::string gen1 ="../Files/Testdrivergen.txt";
std::string gen2 ="../Files/Loggergen.txt";
std::string gen3 ="../Files/Generatorgen.txt";
std::string str1 ="../Files/Header.txt";
std::string str2 ="../Files/TestDriver.cpp";
std::string str6 ="../Files/Logger.cpp";
std::string str7 ="../Files/Generator.cpp";
coGen.codeGenerator(gen1,str2);
coGen.codeGenerator(gen2,str6);
coGen.codeGenerator(gen3,str7);


}
#endif
