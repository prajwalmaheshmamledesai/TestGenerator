#ifndef FileReader_H
#define FileReader_H

///////////////////////////////////////////////////////////////////////
// XmlReader.cpp - Parse XML strings                                 //
// ver 2.1                                                           //
// Language:    Visual C++, Visual Studio 2010, SP1                  //
// platform:    Windows 7, Home Premium, SP1   //
// Application: OOD Projects1                                 //
//Author:       Prajwal Mahesh Mamledesai
//				MS Computer Engineering, pmamlede@syr.edu, (315)-600-6662
/////////////////////////////////////////////////////////////////////////


/*
  Module Operations: 
  ==================
  This module defines the class FileGenerator class and it 
  takes the input of a filename as a string and it opens it and 
  reads it and stores it in a string and returns that string
  
   Public Interface:
  =================
    std::string getFile(std :: string& a);
	std::string ReturnFile();

  Build Process:
  ==============
  Required files
    - FileReader.h, FileReader.cpp

  Maintenance History:
  ====================
  Ver 1.0 - 8th Feb 2012

  */
#include <iostream>
#include <fstream>

class FileGenerator
{
	std::string value;
public:
    std::string getFile(std :: string& a);
	std::string ReturnFile();

};
#endif