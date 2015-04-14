#pragma once
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
  This is a derieved class of IGenerator, here it has a 
  constructor, a destructor and class function which is
  used to generate thr data

   Public Interface:
  =================
    Generator(void);
  virtual ~Generator(void);
  virtual std::string test(std :: string& a);
  
  Build Process:
  ==============
  Required files
    - Generator.h, IGenerate.h

  Maintenance History:
  ====================
  Ver 1.0 - 10th Feb 2012

  */


#define TEST_EXPORTS
#include "IGenerator.h"

class Generator : public IGenerator
{
public:
  Generator(void);
  virtual ~Generator(void);
  virtual std::string test(std :: string& a);
  // inherits static creational function
};

