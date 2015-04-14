#ifndef WINTOOLS_H
#define WINTOOLS_H
/////////////////////////////////////////////////////////////////////
//  wintools.h  -  Win32 API-based helper functions                //
//  ver 4.1                                                        //
//  Language:      Visual C++, Visual Studio 2010, SP1             //
//  Platform:      Toshiba Portege R705, Win 7 Home Premium, SP1   //
//  Application:   Summer Projects, 2011                           //
//  Author:        Jim Fawcett, Syracuse University CST 4-187      //
//                 (315) 443-3948, jfawcett@twcny.rr.com           //
/////////////////////////////////////////////////////////////////////
/*
    Module WinTools Operations:
    ---------------------------
    This module defines classes:
    - Directory<stdStr> class supports directory management.
      Most functions in this class are static, are called using directory
      or wdirectory using std::string or std::wstring, respectively.
    - Convert class supports conversion between basic types.
      Now limited to conversions between std::string and std::wstring.
      All members are static.
    - Path<stdStr> class supports manipulation of file and directory paths.
      Most Path functions are static, and are called using path or wpath.
    - SystemError<stdStr> class provides system error reporting.
      The most important member is getLastMsg().
    - FileHandler<stdStr> class provides file open, close, read/write blocks
      of bytes, invoked using fileHandler or wfileHandler.
*/
///////////////////////////////////////////////////////////////
//                      maintenance page                     //
///////////////////////////////////////////////////////////////
//  Build Process                                            //
//                                                           //
//  Files Required:                                          //
//    wintools.h, wintools.cpp, fileInfo.h, fileInfo.cpp,    //
//    StringConversion.h, StringConversion.cpp               //
//                                                           //
//  Building with Visual C++ , 8.0, from command line:       //
//    cl /EHa /DTEST_WINTOOLS wintools.cpp fileInfo.cpp \    //
//                            StringConversion.cpp           //
//                                                           //
///////////////////////////////////////////////////////////////
/*
    Maintenance History
    ===================
    ver 4.1 : 26 Jan 12
    - fixed bugs relating to string sizes, made all member
      functions inline to above duplicate definitions when linking
    ver 4.0 : 02 Jul 11
    - converted to class parameterized on stdStr with expected
      values of std::string and std::wstring.  Resulted in
      many changes to code.
    ver 3.2 : 18 Jan 09
    - added Directory::GetFileInfos(path,pattern)
    - modified behavior of Path::getFullPath().  Still changes
      relative path to full path, but now does not strip off
      file name if attached.
    ver 3.1 : 13 Jan 09
    - fixed several bugs due to incomplete support for strings
      that are either ascii or unicode, started in ver 3.0
    - migrated most of the string support to StringConversion
      module
    ver 3.0 : 11 Jan 09
    - changed std::string to stdStr typedef
    - changed char to stdChar typedef
    ver 2.2 : 06 May 07
    - added test for no cpp files on search path for testing reading
      and writing in test stub
    ver 2.1 : 27 Mar 07
    - added FileHandler class
    ver 2.0 : 28 Feb 06
    - added Path members: fileSpec, getFullPath, toANSII, and toUnicode
    - added Directory member: CopyFiles
    - modified processing in getPath to take care of a special case
    ver 1.3 : 26 Feb 06
    - fixed bug in Directory<stdStr>::DirectoryExists(const stdStr& path);
    - added Path<stdStr>::toUpper utility function
    ver 1.2 : 21 Feb 06
    - modified WinTools::GetFiles to accept a pattern to 
      select specific files from the current directory
    ver 1.1 : 06 Feb 06
    - added Path class
    ver 1.0 : 05 Feb 06
    - first release
*/

#include <string>
#include <vector>
#include <Windows.h>
#include "fileInfo.h"

namespace Win32Tools
{
  ///////////////////////////////////////////////////////////////
  //  class SystemError - handle system errors

  template <typename stdStr=std::string>
  class SystemError
  {
    public:
      SystemError(int n=0);
      ~SystemError() {}
      int GetLastError();
      stdStr GetLastMsg();
      void ThrowString(const stdStr msg, const stdStr file, int line);
    private:
      stdStr _msg;
  };

  template <typename stdStr>
  inline SystemError<stdStr>::SystemError(int n) { ::SetLastError(n); }

  template <typename stdStr>
  inline int SystemError<stdStr>::GetLastError() { return ::GetLastError(); }

  //----< throw exception string >---------------------------------------

  template <>
  inline void SystemError<std::string>::ThrowString(const std::string msg, const std::string file, int line) 
  {
    std::ostringstream collect;
    collect << msg << ": " << GetLastMsg()
            << "  file: " << file 
            << ", line: " << line;
    throw collect.str();
  }

  template <>
  inline void SystemError<std::wstring>::ThrowString(const std::wstring msg, const std::wstring file, int line) 
  {
    std::wostringstream collect;
    collect << msg << L": " << GetLastMsg()
            << L"  file: " << file 
            << L", line: " << line;
    throw collect.str();
  }
  //----< get system error message string >----------------------

  template <>
  inline std::string SystemError<std::string>::GetLastMsg() 
  {
    // ask system what type of error occurred

    DWORD errorCode = GetLastError();
    if(errorCode == 0)
      return "no error";

    // map errorCode into a system defined error string
    
    DWORD dwFlags = 
      FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_ALLOCATE_BUFFER;
    LPCVOID lpSource = NULL;
    DWORD dwMessageID = errorCode;
    DWORD dwLanguageId = MAKELANGID(LANG_ENGLISH,SUBLANG_ENGLISH_US);
    char* lpBuffer;
    DWORD nSize = 0;
    va_list *Arguments = NULL;

    FormatMessageA(
      dwFlags,lpSource,dwMessageID,dwLanguageId, 
      (char*)&lpBuffer,nSize,Arguments
    );

    _msg = lpBuffer;
    LocalFree(lpBuffer);
    return _msg;
  }

  template <>
  inline std::wstring SystemError<std::wstring>::GetLastMsg() 
  {
    // ask system what type of error occurred

    DWORD errorCode = GetLastError();
    if(errorCode == 0)
      return L"no error";

    // map errorCode into a system defined error string
    
    DWORD dwFlags = 
      FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_ALLOCATE_BUFFER;
    LPCVOID lpSource = NULL;
    DWORD dwMessageID = errorCode;
    DWORD dwLanguageId = MAKELANGID(LANG_ENGLISH,SUBLANG_ENGLISH_US);
    wchar_t* lpBuffer;
    DWORD nSize = 0;
    va_list *Arguments = NULL;

    FormatMessageW(
      dwFlags,lpSource,dwMessageID,dwLanguageId, 
      (wchar_t*)&lpBuffer,nSize,Arguments
    );

    _msg = lpBuffer;
    LocalFree(lpBuffer);
    return _msg;
  }

  ///////////////////////////////////////////////////////////////
  // class Directory - manage directory operations

  template <typename stdStr>
  class Directory
  {
  public:
    Directory();
    ~Directory() { }
    bool SetRestorePath(const stdStr& RestorePath);
    bool RestoreFirstDirectory();
    static bool DirectoryExists(const stdStr& path);
    static bool CreateDirectory(const stdStr& path);
    static bool RemoveDirectory(const stdStr& path, bool confirm=true);
    static stdStr GetCurrentDirectory();
    static bool SetCurrentDirectory(const stdStr& path);
    static std::vector<stdStr> GetFiles(const stdStr& path, const stdStr& pattern);
    static std::vector<stdStr> GetFiles(const stdStr& pattern);
    static std::vector<stdStr> GetFiles();
    static std::vector<stdStr> GetDirectories(const stdStr& path, const stdStr& pattern);
    static std::vector<stdStr> GetDirectories(const stdStr& pattern);
    static std::vector<stdStr> GetDirectories();
    static std::vector<FileInfo<stdStr>> GetFileInfos(const stdStr& path, const stdStr& pattern);
    static std::vector<FileInfo<stdStr>> GetFileInfos(const stdStr& pattern);
    static std::vector<FileInfo<stdStr>> GetFileInfos();
    static bool CopyFiles(
                  const stdStr& from, 
                  const stdStr& toPath, 
                  bool FailIfExits = true
                );
  private:
    stdStr RestorePath_;
  };

  ///////////////////////////////////////////////////////////////
  // class Path - manage path operations

  template <typename stdStr>
  class Path
  {
  public:
    static stdStr getPath(const stdStr& fileSpec);
    static stdStr getFullPath(const stdStr& fileSpec);
    static stdStr getName(const stdStr& fileSpec);
    static stdStr getExt(const stdStr& fileSpec);
    static stdStr fileSpec(const stdStr& path, const stdStr& name);
    static stdStr toLower(const stdStr& src);
    static stdStr toUpper(const stdStr& src);
    static std::wstring toUnicode(const stdStr& src);
    static std::string toANSII(const stdStr& src);
  };
  
  //----< set path to return to on call to RestorePath() >---------------

  template <typename stdStr>
  inline bool Directory<stdStr>::SetRestorePath(const stdStr& path)
  {
    if(Directory<stdStr>::DirectoryExists(path))
    {
      RestorePath_ = path;
      return true;
    }
    return false;
  }
  //----< does this directory exist? >-----------------------------------

  template <typename stdStr>
  inline bool Directory<stdStr>::DirectoryExists(const stdStr& path)
  {
    stdStr temp = GetCurrentDirectory();
    if(SetCurrentDirectory(path))
    {
      SetCurrentDirectory(temp);
      return true;
    }
    return false;
  }
  //----< create directory >---------------------------------------------

  template <>
  inline bool Directory<std::string>::CreateDirectory(const std::string& path)
  {
    return (::CreateDirectoryA(path.c_str(),0) != 0);
  }

  template <>
  inline bool Directory<std::wstring>::CreateDirectory(const std::wstring& path)
  {
    return (::CreateDirectoryW(path.c_str(),0) != 0);
  }
  //----< remove directory >---------------------------------------------

  template <>
  inline bool Directory<std::string>::RemoveDirectory(const std::string &path, bool confirm)
  {
    return (::RemoveDirectoryA(path.c_str()) != 0);
  }
  template <>
  inline bool Directory<std::wstring>::RemoveDirectory(const std::wstring &path, bool confirm)
  {
    return (::RemoveDirectoryW(path.c_str()) != 0);
  }
  //----< get path of current directory >--------------------------------

  template <>
  inline std::string Directory<std::string>::GetCurrentDirectory()
  {
    const size_t BufSize = 256;
    char buffer[BufSize];
    ::GetCurrentDirectoryA(BufSize, buffer);
    return std::string(buffer);
  }

  template <>
  inline std::wstring Directory<std::wstring>::GetCurrentDirectory()
  {
    const size_t BufSize = 256;
    wchar_t buffer[BufSize];
    ::GetCurrentDirectoryW(BufSize, buffer);
    return std::wstring(buffer);
  }
  //----< set path of current directory >--------------------------------

  template <>
  inline bool Directory<std::string>::SetCurrentDirectory(const std::string& path)
  {
    return (::SetCurrentDirectoryA(path.c_str()) != 0);
  }

  template <>
  inline bool Directory<std::wstring>::SetCurrentDirectory(const std::wstring& path)
  {
    return (::SetCurrentDirectoryW(path.c_str()) != 0);
  }
  //----< get files >----------------------------------------------------

  template <typename stdStr>
  inline std::vector<stdStr> Directory<stdStr>::GetFiles(const stdStr& path, const stdStr& pattern)
  {
    std::vector<stdStr> files;
    FileInfo<stdStr> fi;
    if(fi.firstFile(path,pattern))
    {
      if(!fi.isDirectory())
        files.push_back(fi.name());
      while(fi.nextFile())
        if(!fi.isDirectory())
          files.push_back(fi.name());
    }
    return files;
  }
  //----< get files >----------------------------------------------------

  template <typename stdStr>
  inline std::vector<stdStr> Directory<stdStr>::GetFiles(const stdStr& pattern)
  {
    return GetFiles(GetCurrentDirectory(),pattern);
  }
  //----< get files >----------------------------------------------------

  template <>
  inline std::vector<std::string> Directory<std::string>::GetFiles()
  {
    return Directory<std::string>::GetFiles("*.*");
  }

  template <>
  inline std::vector<std::wstring> Directory<std::wstring>::GetFiles()
  {
    return Directory<std::wstring>::GetFiles(L"*.*");
  }
  //----< get directories >----------------------------------------------

  template <>
  inline std::vector<std::string> Directory<std::string>::GetDirectories(const std::string& path, const std::string& pattern)
  {
    std::vector<std::string> files;
    FileInfo<std::string> fi;
    if(fi.firstFile(path,pattern))
    {
      if(fi.isDirectory())
      {
        if(fi.name() != std::string(".") && fi.name() != std::string(".."))
          files.push_back(Path<std::string>::fileSpec(path,fi.name()));
      }
      while(fi.nextFile())
        if(fi.isDirectory())
          if(fi.name() != std::string(".") && fi.name() != std::string(".."))
            files.push_back(Path<std::string>::fileSpec(path,fi.name()));
    }
    return files;
  }
 
  template <>
  inline std::vector<std::wstring> Directory<std::wstring>::GetDirectories(const std::wstring& path, const std::wstring& pattern)
  {
    std::vector<std::wstring> files;
    FileInfo<std::wstring> fi;
    if(fi.firstFile(path,pattern))
    {
      if(fi.isDirectory())
      {
        if(fi.name() != std::wstring(L".") && fi.name() != std::wstring(L".."))
          files.push_back(Path<std::wstring>::fileSpec(path,fi.name()));
      }
      while(fi.nextFile())
        if(fi.isDirectory())
          if(fi.name() != std::wstring(L".") && fi.name() != std::wstring(L".."))
            files.push_back(Path<std::wstring>::fileSpec(path,fi.name()));
    }
    return files;
  }
  //----< get directories >----------------------------------------------

  template <typename stdStr>
  inline std::vector<stdStr> Directory<stdStr>::GetDirectories(const stdStr& pattern)
  {
    return GetDirectories(GetCurrentDirectory(),pattern);
  }
  //----< get directories >----------------------------------------------

  template <>
  inline std::vector<std::string> Directory<std::string>::GetDirectories()
  {
    return Directory<std::string>::GetDirectories("*.*");
  }

  template <>
  inline std::vector<std::wstring> Directory<std::wstring>::GetDirectories()
  {
    return Directory<std::wstring>::GetDirectories(L"*.*");
  }
  //----< get fileInfo objects >-----------------------------------------

  template <typename stdStr>
  inline std::vector<FileInfo<stdStr>> Directory<stdStr>::GetFileInfos(const stdStr& path, const stdStr& pattern)
  {
    std::vector<FileInfo<stdStr>> dinfos;
    FileInfo<stdStr> fi;
    if(fi.firstFile(path,pattern))
    {
      dinfos.push_back(fi);
      while(fi.nextFile())
        dinfos.push_back(fi);
    }
    return dinfos;
  }
  //----< get fileInfo objects >-----------------------------------------

  template <typename stdStr>
  inline std::vector<FileInfo<stdStr>> Directory<stdStr>::GetFileInfos(const stdStr& pattern)
  {
    return GetFileInfos(GetCurrentDirectory(),pattern);
  }
  //----< get fileInfo objects >-----------------------------------------

  template <>
  inline std::vector<FileInfo<std::string>> Directory<std::string>::GetFileInfos()
  {
    return Directory<std::string>::GetFileInfos(GetCurrentDirectory(),"*.*");
  }

  template <>
  inline std::vector<FileInfo<std::wstring>> Directory<std::wstring>::GetFileInfos()
  {
    return Directory<std::wstring>::GetFileInfos(GetCurrentDirectory(),L"*.*");
  }
  //----< copy files matching from filespec to toPath >----------------

  template <>
  inline bool Directory<std::string>::CopyFiles(
         const std::string& from, 
         const std::string& toPath,
         bool FailIfExists
       )
  {
    std::string SrcDir = Path<std::string>::getFullPath(from);
    std::string CurrDir = Directory<std::string>::GetCurrentDirectory();
    bool ok = true;
    if(Directory<std::string>::SetCurrentDirectory(SrcDir))
    {
      std::vector<std::string> files 
        = Directory<std::string>::GetFiles(Path<std::string>::getName(from));
      if(files.size() == 0)
        ok = false;
      for(size_t i=0; i<files.size(); ++i)
      {
        std::string toFileSpec = Path<std::string>::fileSpec(toPath,files[i]);
        std::string fromFileSpec = Path<std::string>::fileSpec(SrcDir,files[i]);
        if(!::CopyFileA(fromFileSpec.c_str(),toFileSpec.c_str(),FailIfExists))
          ok = false;
      }
    }
    else
      ok = false;
    Directory<std::string>::SetCurrentDirectory(CurrDir);
    return ok;
  }

  template <>
  inline bool Directory<std::wstring>::CopyFiles(
         const std::wstring& from, 
         const std::wstring& toPath,
         bool FailIfExists
       )
  {
    std::wstring SrcDir = Path<std::wstring>::getFullPath(from);
    std::wstring CurrDir = Directory<std::wstring>::GetCurrentDirectory();
    bool ok = true;
    if(Directory<std::wstring>::SetCurrentDirectory(SrcDir))
    {
      std::vector<std::wstring> files 
        = Directory<std::wstring>::GetFiles(Path<std::wstring>::getName(from));
      if(files.size() == 0)
        ok = false;
      for(size_t i=0; i<files.size(); ++i)
      {
        std::wstring toFileSpec = Path<std::wstring>::fileSpec(toPath,files[i]);
        std::wstring fromFileSpec = Path<std::wstring>::fileSpec(SrcDir,files[i]);
        if(!::CopyFileW(fromFileSpec.c_str(),toFileSpec.c_str(),FailIfExists))
          ok = false;
      }
    }
    else
      ok = false;
    Directory<std::wstring>::SetCurrentDirectory(CurrDir);
    return ok;
  }
  //----< Directory<stdStr> ctor >-----------------------------------

  template <typename stdStr>
  inline Directory<stdStr>::Directory() 
                  : RestorePath_(Directory<stdStr>::GetCurrentDirectory()) {}

  template <typename stdStr>
  inline bool Directory<stdStr>::RestoreFirstDirectory()
  { 
    return SetCurrentDirectory(RestorePath_); 
  };

  //----< convert string to lower case chars >---------------------------

  template <typename stdStr>
  inline stdStr Path<stdStr>::toLower(const stdStr& src)
  {
    stdStr temp;
    for(size_t i=0; i<src.length(); ++i)
      temp += tolower(src[i]);
    return temp;
  }
  //----< convert string to upper case chars >---------------------------

  template <typename stdStr>
  inline stdStr Path<stdStr>::toUpper(const stdStr& src)
  {
    stdStr temp;
    for(size_t i=0; i<src.length(); ++i)
      temp += toupper(src[i]);
    return temp;
  }
  //----< convert from ANSII to Unicode >--------------------------------

  template <typename stdStr>
  inline std::wstring toUnicode(const stdStr& src)
  {
    std::wstring temp;
    for(size_t i=0; i<src.length(); ++i)
      temp += static_cast<wchar_t>(src[i]);
    return temp;
  }
  //----< convert from Unicode to ANSII >--------------------------------

  template <typename stdStr>
  inline stdStr toANSII(const std::wstring& src)
  {
    stdStr temp;
    for(size_t i=0; i<src.length(); ++i)
      temp += static_cast<char>(src[i]);
    return temp;
  }

  //----< get path from fileSpec >---------------------------------------

  template <>
  inline std::string Path<std::string>::getName(const std::string &fileSpec)
  {
    size_t pos = fileSpec.find_last_of("/");
    if(pos >= fileSpec.length())
      pos = fileSpec.find_last_of("\\");
    if(pos >= fileSpec.length())
      return toLower(fileSpec);
    return toLower(fileSpec.substr(pos+1,fileSpec.length()-pos));
  }

  template <>
  inline std::wstring Path<std::wstring>::getName(const std::wstring &fileSpec)
  {
    size_t pos = fileSpec.find_last_of(L"/");
    if(pos >= fileSpec.length())
      pos = fileSpec.find_last_of(L"\\");
    if(pos >= fileSpec.length())
      return toLower(fileSpec);
    return toLower(fileSpec.substr(pos+1,fileSpec.length()-pos));
  }
  //----< get extension from fileSpec >----------------------------------

  template <typename stdStr>
  inline stdStr Path<stdStr>::getExt(const stdStr& fileSpec)
  {
    size_t pos = fileSpec.find_last_of(stdStr("."));
    if(pos < fileSpec.length())
      return toLower(fileSpec.substr(pos+1,fileSpec.length()-pos));
    return stdStr("");
  }
  //----< get path from fileSpec >---------------------------------------

  template <>
  inline std::string Path<std::string>::getPath(const std::string &fileSpec)
  {
    size_t pos = fileSpec.find_last_of("/");
    if(pos >= fileSpec.length())
      pos = fileSpec.find_last_of("\\");
    if(pos >= fileSpec.length())
      return ".";
    if(fileSpec.find(".",pos+1))
      return toLower(fileSpec.substr(0,pos+1));
    return fileSpec;
  }

  template <>
  inline std::wstring Path<std::wstring>::getPath(const std::wstring &fileSpec)
  {
    size_t pos = fileSpec.find_last_of(L"/");
    if(pos >= fileSpec.length())
      pos = fileSpec.find_last_of(L"\\");
    if(pos >= fileSpec.length())
      return L".";
    if(fileSpec.find(L".",pos+1))
      return toLower(fileSpec.substr(0,pos+1));
    return fileSpec;
  }
  //----< get absoluth path from fileSpec >------------------------------

  template <>
  inline std::string Path<std::string>::getFullPath(const std::string &fileSpec)
  {
    const size_t BufSize = 256;
    char buffer[BufSize];
    char filebuffer[BufSize];  // don't use but GetFullPathName will
    char* name = filebuffer;
    ::GetFullPathNameA(fileSpec.c_str(),BufSize, buffer, &name);
    return std::string(buffer);
  }

  template <>
  inline std::wstring Path<std::wstring>::getFullPath(const std::wstring &fileSpec)
  {
    const size_t BufSize = 256;
    wchar_t buffer[BufSize];
    wchar_t filebuffer[BufSize];  // don't use but GetFullPathName will
    wchar_t* name = filebuffer;
    ::GetFullPathNameW(fileSpec.c_str(),BufSize, buffer, &name);
    return std::wstring(buffer);
  }

  //----< create file spec from path and name >--------------------------

  template <>
  inline std::string Path<std::string>::fileSpec(const std::string &path, const std::string &name)
  {
    std::string fs;
    size_t len = path.size();
    if(path[len-1] == '/' || path[len-1] == '\\')
      fs = path + name;
    else
    {
      if(path.find("/") < path.size())
        fs = path + "/" + name;
      else if(path.find("\\") < path.size())
        fs = path + "\\" + name;
    }
    return fs;
  }

  template <>
  inline std::wstring Path<std::wstring>::fileSpec(const std::wstring &path, const std::wstring &name)
  {
    std::wstring fs;
    size_t len = path.size();
    if(path[len-1] == L'/' || path[len-1] == L'\\')
      fs = path + name;
    else
    {
      if(path.find(L"/") < path.size())
        fs = path + L"/" + name;
      else if(path.find(L"\\") < path.size())
        fs = path + L"\\" + name;
    }
    return fs;
  }

  ///////////////////////////////////////////////////////////////
  // class FileHandler - manage file operations

  typedef char byte_;

  template <typename stdStr>
  class FileHandler
  {
  public:
    void setReadPath(const stdStr& path);
    void setWritePath(const stdStr& path);
    bool openFileReader(const stdStr& file);
    bool openFileWriter(const stdStr& file);
    size_t getBlock(byte_* pBlock, size_t size);
    size_t putBlock(byte_* pBlock, size_t size);
    void closeFileReader();
    void closeFileWriter();
    stdStr blockToString(byte_* pBlock, size_t size);
  private:
    stdStr _ReadPath;
    stdStr _WritePath;
    HANDLE _hReadFile;
    HANDLE _hWriteFile;
  };
  //----< save a path for reading files >------------------------------

  template <typename stdStr>
  inline void FileHandler<stdStr>::setReadPath(const stdStr& path)
  {
    _ReadPath = path;
  }
  //----< save a path for writing files >------------------------------

  template <typename stdStr>
  inline void FileHandler<stdStr>::setWritePath(const stdStr& path)
  {
    _WritePath = path;
  }
  //----< open file for shared reading >-------------------------------

  template <>
  inline bool FileHandler<std::string>::openFileReader(const std::string& file)
  {
    std::string _file(_ReadPath);
    _file.append("/");
    _file.append(file);
    _hReadFile = ::CreateFileA(
                             _file.c_str(),GENERIC_READ,
                             FILE_SHARE_READ,NULL,OPEN_EXISTING,
                             FILE_FLAG_SEQUENTIAL_SCAN,NULL
                           );
    if(_hReadFile != INVALID_HANDLE_VALUE)
    {
      return true;
    }
    return false;
  }

  template <>
  inline bool FileHandler<std::wstring>::openFileReader(const std::wstring& file)
  {
    std::wstring _file(_ReadPath);
    _file.append(L"/");
    _file.append(file);
    _hReadFile = ::CreateFileW(
                             _file.c_str(),GENERIC_READ,
                             FILE_SHARE_READ,NULL,OPEN_EXISTING,
                             FILE_FLAG_SEQUENTIAL_SCAN,NULL
                           );
    if(_hReadFile != INVALID_HANDLE_VALUE)
    {
      return true;
    }
    return false;
  }
  //----< open file for writing >--------------------------------------

  template <>
  inline bool FileHandler<std::string>::openFileWriter(const std::string& file)
  {
    std::string _file(_WritePath);
    _file.append("/");
    _file.append(file);
    _hWriteFile = CreateFileA(
                              _file.c_str(),GENERIC_WRITE,
                              0,NULL,CREATE_ALWAYS,
                              FILE_FLAG_SEQUENTIAL_SCAN,NULL
                            );
    if(_hWriteFile != INVALID_HANDLE_VALUE)
    {
      return true;
    }
    return false;
  }

  template <>
  inline bool FileHandler<std::wstring>::openFileWriter(const std::wstring& file)
  {
    std::wstring _file(_WritePath);
    _file.append(L"/");
    _file.append(file);
    _hWriteFile = CreateFileW(
                              _file.c_str(),GENERIC_WRITE,
                              0,NULL,CREATE_ALWAYS,
                              FILE_FLAG_SEQUENTIAL_SCAN,NULL
                            );
    if(_hWriteFile != INVALID_HANDLE_VALUE)
    {
      return true;
    }
    return false;
  }

  //----< read block of bytes from file, returning number read >-------

  template <typename stdStr>
  inline size_t FileHandler<stdStr>::getBlock(byte_* pBlock, size_t size)
  {
    DWORD numBytesRead;
    ::ReadFile(_hReadFile,pBlock,(DWORD)size,&numBytesRead,NULL);
    return (size_t)numBytesRead;
  }
  //----< write block of bytes to file, returning number written >-----

  template <typename stdStr>
  inline size_t FileHandler<stdStr>::putBlock(byte_* pBlock, size_t size)
  {
    DWORD numBytesWritten;
    ::WriteFile(_hWriteFile,pBlock,(DWORD)size,&numBytesWritten,NULL);
    return (size_t)numBytesWritten;
  }
  //----< close FileReader >-------------------------------------------

  template <typename stdStr>
  inline void FileHandler<stdStr>::closeFileReader()
  {
    CloseHandle(_hReadFile);
  }
  //----< close FileWriter >-------------------------------------------

  template <typename stdStr>
  inline void FileHandler<stdStr>::closeFileWriter()
  {
  
    CloseHandle(_hWriteFile);
  }
  //----< convert block of bytes to a string >-------------------------

  template <typename stdStr>
  inline stdStr FileHandler<stdStr>::blockToString(byte_* pBlock, size_t size)
  {
    stdStr temp;
    for(size_t i=0; i<size; ++i)
      temp += static_cast<char>(pBlock[i]);
    return temp;
  }
  /////////////////////////////////////////////////////////
  // type aliases

  typedef Directory<std::string> directory;
  typedef Directory<std::wstring> wdirectory;
  typedef Path<std::string> path;
  typedef Path<std::wstring> wpath;
  typedef FileHandler<std::string> fileHandler;
  typedef FileHandler<std::wstring> wfileHandler;
  typedef SystemError<std::string> systemError;
  typedef SystemError<std::wstring> wsystemError;
}
#endif
