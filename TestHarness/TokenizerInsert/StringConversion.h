#ifndef STRINGCONVERSION_H
#define STRINGCONVERSION_H
/////////////////////////////////////////////////////////////////////
//  StringConversion.h  -  Win32 API-based helper functions        //
//  ver 2.1                                                        //
//  Language:      Visual C++, Visual Studio 2010, SP1             //
//  Platform:      Toshiba Portege R705, Win 7 Home Premium, SP1   //
//  Application:   Summer Projects, 2011                           //
//  Author:        Jim Fawcett, Syracuse University CST 4-187      //
//                 (315) 443-3948, jfawcett@twcny.rr.com           //
/////////////////////////////////////////////////////////////////////
/*
    Module StringConversion Operations:
    -----------------------------------
    This module defines:
      typedefs: stdStr, stdChar, stdOstream, stdOstringstream
      macros:   stdOut, compare, AM, PM, and a few message strings

    This module also defines the class Convert with static members:
      std::wstring ToWstring(const std::string& src);
      std::wstring ToWstring(const std::wstring& src);
      std::string ToString(const std::string& src);
      std::string ToString(const std::wstring& src);

    The idea is to accept either ascii or unicode strings and convert
    them to one of three specific types: std::string, std::wstring, or
    stdStr.
*/
///////////////////////////////////////////////////////////////
//                      maintenance page                     //
///////////////////////////////////////////////////////////////
//  Build Process                                            //
//                                                           //
//  Files Required:                                          //
//    StringConversion.h, StringConversion.cpp               //
//                                                           //
//  Building with Visual C++ , 8.0, from command line:       //
//    cl -EHsc -DTEST_STRINGCONVERSION StringConversion.cpp  //
//                                                           //
///////////////////////////////////////////////////////////////
/*
    Maintenance History
    ===================
    ver 2.1 : 26 Jan 12
    - fixed bugs relating to string sizes
    ver 2.0 : 02 Jul 11
    - removed UNICODE/ASCII definitions
    - removed ToStdStr()
    - modified test stub
    ver 1.0 : 13 Jan 09
    - first release
*/

#include <string>
#include <iostream>
#include <sstream>
#include <Windows.h>

///////////////////////////////////////////////////////////////
// class Convert - convert between basic types

namespace Win32Tools
{
  class Convert
  {
  public:
    static std::wstring ToWstring(const std::string& src);
    static std::wstring ToWstring(const std::wstring& src);
    static std::string ToString(const std::string& src);
    static std::string ToString(const std::wstring& src);
    template <typename stdStr>
    static stdStr ToStdStr(const std::string& src);
    template <typename stdStr>
    static stdStr ToStdStr(const std::wstring& src);
  };
}
#endif
