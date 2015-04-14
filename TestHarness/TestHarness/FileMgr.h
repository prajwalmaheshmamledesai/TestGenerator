#ifndef FILEMGR_H
#define FILEMGR_H
///////////////////////////////////////////////////////
// FileMgr.h - Process files for Project #1
//
// Jim Fawcett

#include <vector>
#include <string>

class FileMgr
{
public:
  typedef std::vector<std::string> fileSet;
  FileMgr(int argc, char* argv[]);
  fileSet FindFiles(const std::string& path, const std::string& pattern);

private:
  int _argc;
  char** _argv;
};

#endif
