///////////////////////////////////////////////////////////////////////
// XmlReader.cpp - Parse XML strings                                 //
// ver 2.1                                                           //
// Language:    Visual C++, Visual Studio 2010, SP1                  //
// platform:    Windows 7, Home Premium, SP1   //
// Application: OOD Projects1                                 //
//Author:       Prajwal Mahesh Mamledesai
//				MS Computer Engineering, pmamlede@syr.edu, (315)-600-6662
/////////////////////////////////////////////////////////////////////////

#include "IGenerator.h"
#include "ITest.h"
#include "ILogger.h"
#include <iostream>
#include <string>

//<----------runs the create test function of Itest----------->
int main()
{
  ITest* pTest = ITest::createTest();
   if(pTest->test())
   	   std::cout<<"Test Passed";
   else
	   std::cout<<"Test Failed";
}


