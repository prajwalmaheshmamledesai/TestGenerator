#ifndef Insert_H
#define Insert_H

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
  This module defines the class Insert class and it 
  insert the input data from text inside the templates whereever it finds 
   /*MyCode*/ //and /*MyHeader*/.
  /*It takes inputs of of the file and reads it and converts it into string 
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
#include <string>

class Insert{
	std::string value;
public:
    void InsertItems(std::string head, std::string body);
	 void InsertHeader(std::string head, std::string body);
	  
};
#endif