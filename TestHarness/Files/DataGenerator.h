#ifndef DaGen_H
#define DaGen_H

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
  This module defines the class Generators class and it 
  generates the input data.
  It takes inputs of of the file and reads it and converts it into string 
  it useses file stream functions

   Public Interface:
  =================
  std::string StringGenerator(std :: string& a);
  void Datalogger(std :: string& a);

  Build Process:
  ==============
  Required files
    - DataGenerator.h, DataGenerator.cpp

  Maintenance History:
  ====================
  Ver 1.0 - 5th Feb 2012

  */
#include <iostream>
#include <fstream>

class Generators
{
	std::string value;
public:
    std::string StringGenerator(std :: string& a);
	void Datalogger(std :: string& a);
};
#endif