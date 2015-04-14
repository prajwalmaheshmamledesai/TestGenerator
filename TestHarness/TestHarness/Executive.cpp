///////////////////////////////////////////////////////////////////////
// Executive.cpp - Parse XML strings                                 //
// ver 2.1                                                           //
// Language:    Visual C++, Visual Studio 2010, SP1                  //
// platform:    Toshiba Portege R705, Windows 7, Home Premium, SP1   //
// Application: OOD 2012 Project1                                 //
//Author:       Prajwal Mahesh Mamledesai
//				MS Computer Engineering, pmamlede@syr.edu, (315)-600-6662
//Reference:      Jim Fawcett, Syracuse University, CST 4-187          //
//              www.lcs.syr.edu/faculty/fawcett,             (315) 443-3948            //
///////////////////////////////////////////////////////////////////////
#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include "FileCollectors.h"
#include "FileMgr.h"
#include "WinTools.h"
#include <vector>
#include "HeaderCollector.h"
#include "DataGenerator.h"
#include "FileSystem.h"
#include "GenerateProject.h"
#include "CodeGenerator.h"
#include "XmlReader.h"
#include "XmlWriter.h"
using namespace WinTools_Extracts;


int main(int argc,char *argv[]){
getClassPath classpath; getHeader head; std::string ident;
std::string values; std::string path=argv[1];
FileMgr fm(argc, argv);
FileMgr::fileSet fs = fm.FindFiles(path, "*.xml");//<-----using file manager to find the XML----------->
int positions=0;
if(fs.size()!=0){
std::fstream files; std::string line;
files.open(path + "\\" +fs[0]); 
std::ifstream s(path + "\\" +fs[0]);
while(getline(files, line)){values+=line;}
std::string test1 = values;                         //<---------------reading XML---------------------->
XmlReader rdr(test1);
size_t position = 1;
do{
ident = rdr.extractIdentifier(position);
} while(ident.size() > 0);
rdr.reset();
while(rdr.next()){
std::string Body=rdr.body().c_str();
classpath.path(Body);                               //<---------------------reading xml and storing file in a vector-------->
XmlReader::attribElems attribs = rdr.attributes();
for(size_t i=0; i<attribs.size(); ++i)
std::cout << "\n  attributes: " << attribs[i].first.c_str() << ", " << attribs[i].second.c_str();
} }
else{
	std::string value;
	fs=fm.FindFiles(path,"*.h");
	for(unsigned int f=0;f<fs.size();f++) {
	value=fs[f];
	classpath.path(value);
	}	}
std::vector<std::string> fileNames;                 //declaration of vectors for storing filenames------------>
fileNames = classpath.returnFiles();                //retreving the filenames------------------>
head.Header(fileNames);
head.RootFinding();                                 //-----------Finding the root-------------->
CodeGenerator coGen; //<-----------Code Generation-Generation of templates---------->
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
GenerateProject Gen; //<----------Generate the VCX project---------------------->
int i=Gen.GenProj();
}

