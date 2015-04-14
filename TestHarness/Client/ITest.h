#pragma once
// ITest.h
//
// Jim Fawcett

#ifdef TEST_EXPORTS
#define TEST_API __declspec(dllexport)
#else
#define TEST_API __declspec(dllimport)
#endif

class TEST_API ITest
{
public:
  virtual ~ITest(void) {}
  static ITest* createTest();
  virtual bool test()=0;
};

