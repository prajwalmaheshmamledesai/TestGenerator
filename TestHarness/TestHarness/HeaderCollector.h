#ifndef HeaderCollector_H
#define HeaderCollector_H

///////////////////////////////////////////////////////////////////////
// XmlReader.cpp - Parse XML strings                                 //
// ver 2.1                                                           //
// Language:    Visual C++, Visual Studio 2010, SP1                  //
// platform:    Toshiba Portege R705, Windows 7, Home Premium, SP1   //
// Application: Summer 2011 Projects                                 //
//Author:       Prajwal Mahesh Mamledesai
//				MS Computer Engineering, pmamlede@syr.edu, (315)-600-6662
/////////////////////////////////////////////////////////////////////////

/*
  Module Operations: 
  ==================
  This is a defines the class getHeader, here it accepts 
  the filenames and stores it in 1D vector and then it reads
  each files and stores the headers in 2D vectors and the it 
  reads the 2D vectors and finds the root

   Public Interface:
  =================
void Header(std::vector<std::string> fileNames);
	std::vector<std::string> returnTemp();
	std::vector<std::string> returnHeaders();
	int RootFinding();
  
  Build Process:
  ==============
  Required files
    - HeaderCollector.h, HeaderCollector.cpp

  Maintenance History:
  ====================
  Ver 1.0 - 10th Feb 2012

  */


#include "XmlReader.h"
#include <ctype.h>
#include <stack>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>


class  getHeader
{std::vector<std::string> temp;
std::vector<std::string> temp1;
	std::vector<std::vector<std::string >> Headers;
public:
	
	void Header(std::vector<std::string> fileNames);
	std::vector<std::string> returnTemp();
	std::vector<std::string> returnHeaders();
	int RootFinding();
};
#endif