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
  This is a derieved class of ILogger, here it has a 
  constructor, a destructor and class function which is
  used to Log the data

   Public Interface:
  =================
 Logger(void);
  virtual ~Logger(void);
  virtual bool test(std::string& a); 
  Build Process:
  ==============
  Required files
    - Logger.h, ILogger.h

  Maintenance History:
  ====================
  Ver 1.0 - 10th Feb 2012

  */


#define TEST_EXPORTS
#include "ILogger.h"

class Logger : public ILogger
{
public:
  Logger(void);
  virtual ~Logger(void);
  virtual bool test(std::string& a);
  // inherits static creational function
};

