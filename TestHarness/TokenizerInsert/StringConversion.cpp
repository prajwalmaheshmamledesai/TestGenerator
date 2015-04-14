/////////////////////////////////////////////////////////////////////
//  StringConversion.cpp  -  Win32 API-based helper functions      //
//  ver 2.1                                                        //
//  Language:      Visual C++, Visual Studio 2010, SP1             //
//  Platform:      Toshiba Portege R705, Win 7 Home Premium, SP1   //
//  Application:   Summer Projects, 2011                           //
//  Author:        Jim Fawcett, Syracuse University CST 4-187      //
//                 (315) 443-3948, jfawcett@twcny.rr.com           //
/////////////////////////////////////////////////////////////////////

#include "StringConversion.h"
using namespace Win32Tools;

//----< convert std::string to std::wstring >----------------------------

std::wstring Convert::ToWstring(const std::string& src)
{
  std::wstring dest;
  for(size_t i=0; i<src.length(); ++i)
    dest += static_cast<wchar_t>(src[i]);
  return dest;
}
//----< convert std::wstring to std::wstring >---------------------------

std::wstring Convert::ToWstring(const std::wstring& src)
{
  return src;
}
//----< convert std::wstring to std::string >----------------------------

std::string Convert::ToString(const std::wstring& src)
{
  std::string dest;
  for(size_t i=0; i<src.length(); ++i)
    dest += static_cast<char>(src[i]);
  return dest;
}
//----< convert std::string to std::string >-----------------------------

std::string Convert::ToString(const std::string& src)
{
  return src;
}
//----< convert std::string to stdStr >--------------------------------

template <typename stdStr>
stdStr Convert::ToStdStr(const std::string& src)
{
  if(typeid(stdStr).name() == typeid(std::string))
    return src;
  else
  {
    std::wstring dest;
    for(size_t i=0; i<src.length(); ++i)
      dest += static_cast<wchar_t>(src[i]);
    return dest;
  }
}
 //----< convert std::wstring to stdStr >--------------------------------

template <typename stdStr>
stdStr Convert::ToStdStr(const std::wstring& src)
{
  if(typeid(stdStr).name() == typeid(std::wstring))
    return src;
  else
  {
    std::string dest;
    for(size_t i=0; i<src.length(); ++i)
      dest += static_cast<char>(src[i]);
    return dest;
  }
}

//----< test stub >------------------------------------------------------

#ifdef TEST_STRINGCONVERSION

void main()
{
  std::cout << "\n  testing string conversions ";
  std::cout << "\n ============================\n";

  std::string asciiTestString = "an ascii test string";
  std::wstring unicodeTestString = L"a unicode test string";

  // test converting either ascii or unicode to std::wstring
  std::wstring wtest = Convert::ToWstring(asciiTestString);
  std::wcout << L"\n  wstring converted from string: " << wtest;
  wtest = Convert::ToWstring(unicodeTestString);
  std::wcout << L"\n  wstring converted from wstring: " << wtest << std::endl;

  // test converting either ascii or unicode to std::string
  std::string atest = Convert::ToString(asciiTestString);
  std::cout << "\n  string converted from string: " << atest;
  atest = Convert::ToString(unicodeTestString);
  std::cout << "\n  string converted from wstring: " << atest << std::endl;

  // test converting either ascii or unicode to stdStr
  std::wstring atwtest = Convert::ToWstring(asciiTestString);
  std::wcout << L"\n  string converted to wstring: " << atwtest;
  std::string wtatest = Convert::ToString(unicodeTestString);
  std::cout << "\n  wstring converted to string: " << wtatest << "\n";
  std::cout << "\n\n";

}
#endif
