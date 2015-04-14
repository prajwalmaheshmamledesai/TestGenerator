#ifndef CodeGen_H
#define CodeGen_H

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
  This module defines the class CodeGenerator classs and it 
  generates the generic templates
  It takes inputs of two files 
  one file will be the .txt file and other will be the file 
  to which it will write
  it useses file stream functions

   Public Interface:
  =================
  void codeGenerator(std::string head, std::string body);

  Build Process:
  ==============
  Required files
    - CodeGenerator.h, CodeGenerator.cpp

  Maintenance History:
  ====================
  Ver 1.0 - 5th Feb 2012

  */

#include <iostream>
#include <string>

class CodeGenerator{
	std::string value;
public:
      void codeGenerator(std::string head, std::string body);
};

#endif