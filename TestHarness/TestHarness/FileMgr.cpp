/////////////////////////////////////////////////
// FileMgr.cpp - process files for Project #1
//
// will eventually put some XML processing here

#include <string>
#include <vector>
#include <iostream>
#include "WinTools.h"
#include "FileMgr.h"

using namespace Win32Tools;

FileMgr::FileMgr(int argc, char* argv[])
{
  _argc = argc;
  _argv = argv;
}

FileMgr::fileSet FileMgr::FindFiles(const std::string& path, const std::string& pattern)
{
  // eventually will use XML config file to possibly modify fileSet

  return directory::GetFiles(path, pattern);
}
//---- test stub ---------------------------------

#ifdef TEST_FILEMGR

int main(int argc, char* argv[])
{
  // assumes path may be named by argv[1]
  // assumes patterns may be named by argv[2], argv{3}, ...

  std::string path = ".";
  std::string pattern = "*.*";
  if(argc > 1)
    path = argv[1];
  if(argc > 2)
    pattern = argv[2];

  FileMgr fm(argc, argv);
  FileMgr::fileSet fs = fm.FindFiles(path, pattern);

  int count = 2;
  while(count < argc)
  {
    pattern = argv[count++];
    FileMgr::fileSet files = fm.FindFiles(path, pattern);

    for(size_t i=0; i<files.size(); ++i)
      std::cout << "\n  " << files[i];
    std::cout << "\n\n";
	
  }
}
#endif
