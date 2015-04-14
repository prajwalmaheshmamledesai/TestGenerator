#pragma once
/*
///////////////////////////////////////////////////////////////////////
// XmlReader.cpp - Parse XML strings                                 //
// ver 2.1                                                           //
// Language:    Visual C++, Visual Studio 2010, SP1                  //
// platform:    Windows 7, Home Premium, SP1   //
// Application: OOD Projects1                                 //
//Author:       Prajwal Mahesh Mamledesai
//				MS Computer Engineering, pmamlede@syr.edu, (315)-600-6662
/////////////////////////////////////////////////////////////////////////

  Module Operations: 
  ==================
  This is a base class , here it has a 
  constructor, a destructor and class function which is
  used to generate thr data

   Public Interface:
  =================
 virtual ~IGenerator(void) {}
  static IGenerator* createTest();
  virtual std::string test(std::string& a)=0;
  
  Build Process:
  ==============
  Required files
    - Generator.h, IGenerate.h

  Maintenance History:
  ====================
  Ver 1.0 - 10th Feb 2012

*/
#ifdef TEST_EXPORTS
#define TEST_API __declspec(dllexport)
#else
#define TEST_API __declspec(dllimport)
#endif
#include <string>
class TEST_API IGenerator
{
public:
  virtual ~IGenerator(void) {}
  static IGenerator* createTest();
  virtual std::string test(std::string& a)=0;
};

