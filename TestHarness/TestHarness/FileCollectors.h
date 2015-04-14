#ifndef FileCollector_H
#define FileCollector_H

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
  This module defines the class getClassPath class and it 
  stores the list of files in 1D vector
  It takes inputs of of the string and reads it stores in a vector

   Public Interface:
  =================
  void path(std :: string& a);
  std::vector<std::string> returnFiles();
  void Headerprocessing(std :: string& a);

  Build Process:
  ==============
  Required files
    - FileCollector.h, FileCollector.cpp

  Maintenance History:
  ====================
  Ver 1.0 - 6th Feb 2012

  */

#include "XmlReader.h"
#include <ctype.h>
#include <stack>
#include <iostream>
#include <fstream>
#include <string>

class  getClassPath
{std::vector<std::string> Classpath  ;
  public:
	
	void path(std :: string& a);
	std::vector<std::string> returnFiles();
	void Headerprocessing(std :: string& a);
};
#endif