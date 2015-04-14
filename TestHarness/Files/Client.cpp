#include "IGenerator.h"
#include "ITest.h"
#include "ILogger.h"
#include <iostream>
#include <string>

int main()
{
  ITest* pTest = ITest::createTest();
   if(pTest->test())
   	   std::cout<<"Test Passed";
   else
	   std::cout<<"Test Failed";
}


