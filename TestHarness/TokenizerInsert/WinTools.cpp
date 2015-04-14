/////////////////////////////////////////////////////////////////////
//  wintools.cpp - Win32 API-based helper functions                //
//  ver 4.1                                                        //
//  Language:      Visual C++, Visual Studio 2010, SP1             //
//  Platform:      Toshiba Portege R705, Win 7 Home Premium, SP1   //
//  Application:   Summer Projects, 2011                           //
//  Author:        Jim Fawcett, Syracuse University CST 4-187      //
//                 (315) 443-3948, jfawcett@twcny.rr.com           //
/////////////////////////////////////////////////////////////////////

#ifdef TEST_WINTOOLS

//----< test stub >----------------------------------------------------

#include <iostream>
#include <sstream>
#include <iomanip>
#include <iostream>
#include "wintools.h"

using namespace Win32Tools;


int main() {

  std::cout << "\n  Testing WinTools ";
  std::cout << "\n ==================\n";

  systemError se;
  std::cout << "\n  Last error code = " << ::GetLastError();
  std::cout << "\n  Last error message = " << se.GetLastMsg();
  std::cout << std::endl;

  std::cout << "\n  testing directory management ";
  std::cout << "\n ------------------------------";

  directory dir;
  if(dir.CreateDirectory("foobar"))
    std::cout << "\n  created directory foobar";
  else
    std::cout << "\n  failed to create directory foobar";
  
  wdirectory wdir;
  if(wdir.CreateDirectory(L"foobarwide"))
    std::cout << "\n  created directory foobarwide";
  else
    std::cout << "\n  failed to create directory foobarwide";

  if(dir.DirectoryExists("foobar"))
    if(dir.RemoveDirectory("foobar"))
      std::cout << "\n  directory foobar removed";

  if(wdir.DirectoryExists(L"foobarwide"))
    if(wdir.RemoveDirectory(L"foobarwide"))
      std::cout << "\n  directory foobarwide removed";
  std::cout << "\n";

  if(dir.DirectoryExists("test"))
  {
    std::cout << "\n  Directory test exists";
    if(dir.RemoveDirectory("test"))
      std::cout << "\n  successfully removed directory test";
  }
  else
  {
    if(dir.CreateDirectory("test"))
      std::cout << "\n  successfully created directory test";
    else
      std::cout << "\n  Directory creation failed";
  }
  std::cout << "\n  Current directory is: " << dir.GetCurrentDirectory();
  if(dir.SetCurrentDirectory("./test"))
  {
    std::cout << "\n  changed to: "  << dir.GetCurrentDirectory();
    dir.SetCurrentDirectory("..");
    std::cout << "\n  changed back to: " << dir.GetCurrentDirectory();
  }
  std::cout << std::endl;

  std::vector<std::wstring> wfiles = wdirectory::GetFiles();
  std::cout << "\n  files in this directory are:";
  for(size_t i=0; i<wfiles.size(); ++i)
    std::wcout << "\n    " << wfiles[i];
  std::cout << std::endl;

  std::vector<std::string> files = directory::GetFiles("*.h*");
  std::cout << "\n  *.h* files in this directory are:";
  for(size_t i=0; i<files.size(); ++i)
    std::cout << "\n    " << files[i];
  std::cout << std::endl;

  std::vector<std::wstring> dirs = wdirectory::GetDirectories();
  std::cout << "\n  directories in this directory are:";
  for(size_t i=0; i<dirs.size(); ++i)
    std::wcout << L"\n    " << dirs[i];
  std::cout << std::endl;

  std::cout << "\n  testing Path management ";
  std::cout << "\n -------------------------";

  std::string paths[] = { "aFile", "../../aFile", "test/aFile", "../../" };
  for(int i=0; i<4; ++i)
  {
    std::cout << "\n  fileSpec: " << std::setw(12) << paths[i];
    std::cout << ", name: " << path::getName(paths[i]);
    std::cout << "\n  fileSpec: " << std::setw(12) << paths[i];
    std::wcout << L", path: " << wpath::getPath(Convert::ToWstring(paths[i]));
  }
  std::cout << std::endl;

  std::cout << "\n  testing error messages ";
  std::cout << "\n ------------------------";

  int err = GetLastError();
  std::cout << "\n  Last error code = " << err;

  try {
    se.ThrowString("throw message",Convert::ToString(__FILE__),__LINE__);
  }
  catch(const std::string &msg)
  { 
    std::cout << "\n  " << msg; 
  }
  std::cout << "\n";

  std::cout << "\n  test writing to \"Program Files\" Folder ";
  std::cout << "\n ------------------------------------------";

  std::string CurrDir = path::getFullPath(".");
  std::string fileSpec = CurrDir + "*.*";

  if(directory::SetCurrentDirectory("C:/Program Files"))
  {
    std::cout << "\n  sucessfully set directory to \"C:\\Program Files\"";
    if(directory::DirectoryExists("foobar stuff"))
    {
      std::cout << "\n  \"C:\\Program Files\\foobar stuff\" exists";
      if(!directory::CopyFiles(fileSpec,"C:\\Program Files\\foobar stuff"))
        std::cout << "\n  one or more file copy operations failed";
      else
        std::cout << "\n  all file copy operations succeeded";
    }
    else if(directory::CreateDirectory("foobar stuff"))
    {
      std::cout << "\n  successfully created \".\\foobar stuff\"";
      if(!directory::CopyFiles(fileSpec,"C:\\Program Files\\foobar stuff"))
        std::cout << "\n  one or more file copy operations failed";
      else
        std::cout << "\n  all file copy operations succeeded";
    }
  }

  std::cout << "\n";
  std::cout << "\n  File operations: ";
  std::cout << "\n ------------------";

  std::string path = "C:\\temp";
  if(!directory::SetCurrentDirectory(path))
  {
    std::cout << "\n\n  invalid path " << path << "\n\n";
    return 1;
  }
  std::vector<std::string> file = directory::GetFiles();

  // find a fairly large cpp file to read

  std::string displayFile = "";
  fileInfo fi;
  size_t fileSize = 0;
  for(size_t i=0; i<file.size(); ++i)
  {
    std::cout << "\n  " << file[i];
    if("cpp" == path::getExt(file[i]))
    {
      fi.firstFile(file[i]);
      if(fileSize < fi.size() && fi.size() < 50000)
      {
        fileSize = fi.size();
        displayFile = file[i];
      }
    }
  }

  if(displayFile == "")
  {
    std::cout << "\n  no *.cpp files in " << path.c_str() << "\n\n";
    return 1;
  }

  std::cout << "\n\n  Reading Blocks";
  std::cout << "\n ----------------\n";

  if(file.size() > 0)
  {
    fileHandler fh;
    fh.setReadPath(path);
    if(fh.openFileReader(displayFile))
    {
      std::cout << "\n  opening file " << displayFile << "\n\n";
      const size_t size = 1024;
      size_t bytesRead;
      byte_ buffer[size];
      do
      {
        bytesRead = fh.getBlock(buffer,size);
        std::cout << fh.blockToString(buffer,bytesRead);
      } while(bytesRead == size);
    }
    fh.closeFileReader();
  }

  std::wstring wpath = L"C:\\temp";

  std::cout << "\n\n  Writing Blocks";
  std::cout << "\n ----------------";

  std::wstring wdisplayFile = Convert::ToWstring(displayFile);

  if(file.size() > 0)
  {
    wfileHandler fh;
    fh.setReadPath(wpath);
    fh.setWritePath(wpath);
    if(!fh.openFileReader(wdisplayFile))
    {
      std::wcout << L"\n  open " << wdisplayFile << L" failed\n\n";
      return 1;
    }
    else
      std::wcout << L"\n  opening file " << wdisplayFile << L" for reading";
    if(fh.openFileWriter(L"test.txt"))
    {
      std::wcout << L"\n  opening file " << L"test.txt" << L" for writing";
      const size_t size = 1024;
      size_t bytesRead;
      byte_ buffer[size];
      int count = 0;
      do
      {
        std::wcout << L"\n    writing block #" << ++count;
        bytesRead = fh.getBlock(buffer,size);
        fh.putBlock(buffer,bytesRead);
      } while(bytesRead == size);
    }
    std::wcout << L"\n  closing write file";
    fh.closeFileWriter();
    std::wcout << L"\n  closing read file";
    fh.closeFileReader();
    std::cout << "\n\n";
  }
}

#endif
