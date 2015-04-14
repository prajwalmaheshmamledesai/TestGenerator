/////////////////////////////////////////////////////////////////////////
//  fileInfo.cpp  -  manage file information extracted from Win32 API  //
//  ver 4.1                                                            //
//  Language:      Visual C++, Visual Studio 2010, SP1                 //
//  Platform:      Toshiba Portege R705, Win 7 Home Premium, SP1       //
//  Application:   Summer Projects, 2011                               //
//  Author:        Jim Fawcett, Syracuse University CST 4-187          //
//                 (315) 443-3948, jfawcett@twcny.rr.com               //
/////////////////////////////////////////////////////////////////////////

//----< test stub >--------------------------------------------------

#ifdef TEST_FILEINFO

#pragma warning(disable : 4786)
#include <iostream>
#include <iomanip>
#include <set>
#include "fileInfo.h"

using namespace Win32Tools;

///////////////////////////////////////////////////////////////
// The classes below are private - only used by this test
// stub, and so are placed here, and not in fileInfo.h
//
//----< function object which detects case insensitive order >-------

template <typename stdStr>
class lessNoCase
{
public:
  bool operator()(const FileInfo<stdStr> &fi1, const FileInfo<stdStr> &fi2) {
    stdStr name1 = fi1.name(), name2 = fi2.name();
    size_t leastSize = min(name1.size(), name2.size());
    for(size_t i=0; i<leastSize; ++i)
      if(tolower(name1[i]) > tolower(name2[i]))
        return false;
    return name1.size() < name2.size();
  }
};
//----< function object which detects date order >-------------

template <typename stdStr>
class earlier 
{
public:
  bool operator()(const FileInfo<stdStr> &fi1, const FileInfo<stdStr> &fi2) {
    return fi1.earlier(fi2);
  }
};
//----< function object which detects size order >-------------

template <typename stdStr>
class smaller 
{  
public:
  bool operator()(const FileInfo<stdStr> &fi1, const FileInfo<stdStr> &fi2) {
    return fi1.smaller(fi2);
  }
};

//----< test entry point >-------------------------------------

void main(int argc, char *argv[]) 
{
  std::cout << "\n  Testing Fileinfo objects "
            << "\n ==========================\n";

  // file names are unique, so use set

  typedef std::set< fileInfo, std::less<fileInfo>  > setNames;
  
  // lowercase file names, file dates, and file sizes
  // may not be unique, so use multiset

  typedef std::multiset< fileInfo, lessNoCase<std::string> > setLCNames;
  typedef std::multiset< fileInfo, earlier<std::string> > setDates;
  typedef std::multiset< fileInfo, smaller<std::string> > setSizes;

  fileInfo f;
  f.showSeconds(true);
  try
  {
    if(argc > 1)
      f.setPath(argv[1]);
  }
  catch(std::exception& e) 
  { 
    std::cout << "\n\n  " << e.what() << "\n\n";
    f.setPath("..");
  }
  std::cout << "\n  Files in C:\\Temp";
  if(f.firstFile("C:\\Temp","*.*"))
  {
    if(!f.isDirectory())
      std::cout << "\n  " << f.name();
    while(f.nextFile())
      if(!f.isDirectory())
        std::cout << "\n  " << f.name();
  }
  std::cout << "\n";
  std::cout << "\n  path = " << Convert::ToString(f.getPath()) << std::endl;
  std::cout << "\n  Files in FindNextFile Order\n";
  setNames sn;
  setLCNames sl;
  setDates sd;
  setSizes ss;

  if(!f.firstFile("*.*"))
    return;
  sn.insert(f);
  sl.insert(f);
  sd.insert(f);
  ss.insert(f);
  f.showData(std::cout);
  while(f.nextFile()) {
    sn.insert(f);
    sl.insert(f);
    sd.insert(f);
    ss.insert(f);
    f.showData(std::cout);
  }
  f.closeFile();

  std::cout << "\n  Files ordered alphabetically:\n";
  setNames::iterator fnIt;
  for(fnIt = sn.begin(); fnIt != sn.end(); fnIt++) {
    fnIt->showData(std::cout);
  }
  std::cout << "\n  Files with case insensitive alphabetic order:\n";
  setLCNames::iterator flIt;
  for(flIt = sl.begin(); flIt != sl.end(); flIt++) {
    flIt->showData(std::cout);
  }
  std::cout << "\n  Files ordered by date:\n";
  setDates::iterator fdIt;
  for(fdIt = sd.begin(); fdIt != sd.end(); fdIt++) {
    fdIt->showData(std::cout);
  }
  std::cout << "\n  Files ordered by size:\n";
  setSizes::iterator fsIt;
  for(fsIt = ss.begin(); fsIt != ss.end(); fsIt++) {
    fsIt->showData(std::cout);
  }
  std::cout << "\n\n";

  std::cout << "\n  Comparing file times:\n";
  setDates::iterator first = sd.begin();
  setDates::iterator second = first;
  ++second;
  fileInfo fiFirst = *first;
  fileInfo fiSecond = *second;
  std::cout << "\n  " << Convert::ToString(fiFirst.name()).c_str() << " has date: "
            << Convert::ToString(fiFirst.date()) << ", " 
            << Convert::ToString(fiFirst.time());
  std::cout << "\n  " << Convert::ToString(fiSecond.name()) << " has date: "
            << Convert::ToString(fiSecond.date()) << ", " 
            << Convert::ToString(fiSecond.time());
  int retVal = fiFirst.compareFileTime(fiSecond.getFILETIME());
  if(retVal > 0)
    std::cout << "\n  " << Convert::ToString((*first).name()) 
              << " is later than " << Convert::ToString((*second).name());
  if(retVal == 0)
    std::cout << "\n  " << Convert::ToString((*first).name()) 
              << " is same time as " << Convert::ToString((*second).name());
  if(retVal < 0)
    std::cout << "\n  " << Convert::ToString((*first).name()) 
              << " is earlier than " << Convert::ToString((*second).name());

  retVal = fiSecond.compareFileTime(fiFirst.getFILETIME());
  if(retVal > 0)
    std::cout << "\n  " << Convert::ToString((*second).name()) 
              << " is later than " << Convert::ToString((*first).name());
  if(retVal == 0)
    std::cout << "\n  " << Convert::ToString((*second).name()) 
              << " is same time as " << Convert::ToString((*first).name());
  if(retVal < 0)
    std::cout << "\n  " << Convert::ToString((*second).name()) 
              << " is earlier than " << Convert::ToString((*first).name());
  retVal = fiFirst.compareFileTime(fiFirst.getFILETIME());
  if(retVal > 0)
    std::cout << "\n  " << Convert::ToString(fiFirst.name()) 
              << " is later than " << Convert::ToString(fiFirst.name());
  if(retVal == 0)
    std::cout << "\n  " << Convert::ToString(fiFirst.name()) 
              << " is same time as " << Convert::ToString(fiFirst.name());
  if(retVal < 0)
    std::cout << "\n  " << Convert::ToString(fiFirst.name()) 
              << " is earlier than " << Convert::ToString(fiFirst.name());

  std::cout << "\n\n  Last search path = " << Convert::ToString(f.lastSetPath());
  std::cout << "\n";

  wfileInfo wfi;
  std::wcout << "\n  current path is: " << wfi.getPath();
  std::wcout << L"\n  files found here: ";
  if(wfi.firstFile())
    std::wcout << L"\n  " << wfi.name();
  while(wfi.nextFile())
    std::wcout << L"\n  " << wfi.name();
  std::cout << "\n\n";
}
#endif
