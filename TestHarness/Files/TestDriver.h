#pragma once
// TestDriver.h
//
// Jim Fawcett

#define TEST_EXPORTS
#include "itest.h"

class TestDriver : public ITest
{
public:
  TestDriver(void);
  virtual ~TestDriver(void);
  virtual bool test();
  // inherits static creational function
};

