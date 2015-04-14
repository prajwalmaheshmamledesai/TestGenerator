#ifndef FILEINFO_H
#define FILEINFO_H
///////////////////////////////////////////////////////////////////////
//  fileInfo.h  -  manage file information extracted from Win32 API  //
//  ver 4.1                                                          //
//  Language:      Visual C++, Visual Studio 2010, SP1               //
//  Platform:      Toshiba Portege R705, Win 7 Home Premium, SP1     //
//  Application:   Summer Projects, 2011                             //
//  Author:        Jim Fawcett, Syracuse University CST 4-187        //
//                             (315) 443-3948      , jfawcett@twcny.rr.com             //
///////////////////////////////////////////////////////////////////////
/*
    class FileInfo Operations:
    --------------------------
    FileInfo provides a convenient way of finding all the files in
    a specific directory, as well as their associated attributes.

    The FileInfo class acquires file information from win32 API
    functions and returns parts of the information on demand.
    Specifically a FileInfo object extracts file name, size,
    and time and date of last modification for each file matching
    a pattern in the current directory.

    A FileInfo object saves the current directory path when created
    and its destructor restores that original path before the object
    is destroyed.  One function, setPath(const string&), will throw
    an exception if the path is invalid.  Handling of this exception
    is deferred to application code as FileInfo doesn't know what to
    do about that condition.  The test stub shows how an application
    can do that.

    Public Interface:
    -----------------
      fileInfo fi;                     void construct
      fileInfo fi(d:\test);            constructor setting path
      fileInfo fi2 = fi1;              copy constructor
      bool b = fi.firstFile("*.cpp");  find first file matching in
                                       current dir, true if exists
      bool b = fi.nextFile();          find next file matching
                                       pattern in current dir
      fi.closeFile();                  close file search
      string n = fi.name();            file name
      string d = fi.date();            file date last modified
      string t = fi.time();            file time last modified
      unsigned long int s = fi.size(); file size
      if(fi1 <  fi2) {...}             test file name order
      if(fi1 == fi2) {...}             test file name equality
      if(fi1.later(fi2)) {...}         test file time
      if(fi1.earlier(fi2)) {...}       test file time
      if(fi1.smaller(fi2)) {...}       test file size
      if(fi1.larger (fi2)) {...}       test file size
      fi.showData(cout);               display file data
      fi.showData(cout,'');            display with no return

      FILETIME ft = fi.getFILETIME();  return this file's FILETIME structure
      retVal = fi.compareFileTime(ft); return 1, 0, -1 if ft is earlier
                                         same, or later, respectively
      string path = fi.getPath();      get name of current dir
      fi.setPath(const string &path);  set current dir to path
      path = fi.getPath();             return current path
      path = fi.lastSetPath();         returns last path set

    See test stub for example of how to use fileInfo object with STL
    containers to get sets of files, sorted by name, date, or size.
*/
///////////////////////////////////////////////////////////////
//                      maintenance page                     //
///////////////////////////////////////////////////////////////
//  Build Process                                            //
//                                                           //
//  Files Required:                                          //
//    fileInfo.h, fileInfo.cpp                               //
//                                                           //
//  Building with Visual C++ , 8.0, from command line:       //
//    cl -EHsc -DTEST_FILEINFO fileInfo.cpp                  //
//                                                           //
///////////////////////////////////////////////////////////////
/*
    Maintenance History
    ===================
    ver 4.1 : 26 Jan 12
    - made all template functions inline to avoid duplicate 
      definitions at link time
    ver 4.0 : 02 Jul 11
    - converted to class parameterized on stdStr, with expected
      values of either std::string or std::wstring
    ver 3.3 : 02 May 11
    - Fixed bug reported by Phil Pratt-Szeliga by testing search 
      handle before closing search.
    ver 3.2 : 04 Jun 10
    - Had to replace every instance of the terms *first and *second
      which are iterators into a container of fileInfo objects with
      fiFirst and fiSecond where:
      fileInfo fiFirst = *first;
      fileInfo fiSecond = *second;
      Haven't figure out why that made FileInfo fail to compile with
      Visual Studio 2010.
    ver 3.1 : 18 Jan 09
    - fixed text handling bug in fileInfo<stdStr>::firstFile(path,pattern) 
    ver 3.0 : 13 Jan 09
    - added Win32Tools namespace
    - made a few scattered modifications to accomodate changes to
      WinTools module
    ver 2.9 : 06 May 07
    - modified to support either UNICODE or ASCII character sets
      by using typedefs, #defines, and writing a couple of string
      conversion routines.
    ver 2.8 : 10 Feb 06
    - moved three functor classes: lessNoCase, earlier, and smaller
      into the fileInfo.cpp test stub.  They are private, used only
      by the test stub, so belong there.
    ver 2.7 : 19 Jan 05
    - added case insensitive file name comparisons
    - modified display in showData
    - eliminated some warnings with an appropriate static_cast
    - fixed constructor and operator= initialization bugs
    ver 2.6 : 22 Jan 05
    - added manual page information about exception handling
    ver 2.5 : 17 Jan 05
    - added display options and made cosmetic changes
    ver 2.4 : 03 May 04
    - added lastSetPath() to retrieve the last path searched.
    ver 2.3 : 01 May 04
    - the last fix broke recursion, reported by Kurt Denhoff, now
      fixed and uploaded to class code folder in getfiles and 
      fileInfo subfolders
    ver 2.2 : 27 Apr 04
    - added support for extracting and comparing FILETIME structures
    - setPath(const std::string&) now throws an invalid_argument
      exception if the path does not exist
    ver 2.1 : 23 Jan 04
    - cosmetic changes - eliminated a couple of comments and an
      unused line of code.
    ver 2.0 : 04 Apr 01
    - changed design so that directories are never set.  Now uses
      a specified path for file finding without changing the
      current directory
    ver 1.5 : 12 Jun 00
      - added extraction of attributes
    ver 1.4 : 29 Jan 00
      - added tests for earlier and smaller member functions in 
        test stub
    ver 1.3 : 14 Jul 99
      - more cosmetic modification to header file
    ver 1.2 : 11 Jul 99
      - added member function fileClose()
      - cosmetic modifications to both header and implem. files.
    ver 1.1 : 05 Jul 99
      - modified member function args, removing WIN32_FIND_DATA
        structures from all member function parameter lists
      - added firstFile(...) and nextFile() functions
      - save and restore original path
    ver 1.0 : 07 Feb 98
      - first release
*/

///////////////////////////////////////////////////////////////
//                         Declarations                      //
///////////////////////////////////////////////////////////////

#include <windows.h>
#include <string>
#include <iomanip>
#include "StringConversion.h"

namespace Win32Tools
{
  template<typename stdStr>
  class FileInfo 
  {
  public:

    // object management
    FileInfo();
    FileInfo(const FileInfo<stdStr> &fi);
    FileInfo(const stdStr &path);
    ~FileInfo();
    FileInfo<stdStr>& operator=(const FileInfo<stdStr>& fi);

    // search
    bool firstFile(stdStr path, const stdStr& filePattern);
    bool firstFile(const stdStr& filePattern);
    bool firstFile();
    bool nextFile();
    void closeFile();

    // file information
    stdStr       name() const;
    unsigned long int size() const;
    stdStr       date() const;
    stdStr       time() const;
    FILETIME     getFILETIME();
    int          compareFileTime(FILETIME& ft);
    stdStr       attributes() const;
    bool isArchive()    const;
    bool isCompressed() const;
    bool isDirectory()  const;
    bool isEncrypted()  const;
    bool isHidden()     const;
    bool isNormal()     const;
    bool isOffLine()    const;
    bool isReadOnly()   const;
    bool isSystem()     const;
    bool isTemporary()  const;

    // name comparisons
    bool operator< (const FileInfo<stdStr> &fi) const;
    bool operator==(const FileInfo<stdStr> &fi) const;

    // time comparisons
    bool earlier   (const FileInfo<stdStr> &fi) const;
    bool later     (const FileInfo<stdStr> &fi) const;

    // size comparisons
    bool smaller   (const FileInfo<stdStr> &fi) const;
    bool larger    (const FileInfo<stdStr> &fi) const;

    // directory management
    stdStr lastSetPath();
    static stdStr getPath();
    void setPath(const stdStr& s);

    // quick display
    void showData(std::ostream &out,char ch='\n',int width=20) const;
    void showData(std::wostream &out,wchar_t ch=L'\n',int width=20) const;
    void showSize(bool show_size=true);
    void showSeconds(bool show_seconds=false);
    void showTime(bool show_time=true);
    void showDate(bool show_date=true);
    void showAttributes(bool show_attrib=true);

  private:
    WIN32_FIND_DATAA data;
    WIN32_FIND_DATAW wdata;
    HANDLE _handle;
    SYSTEMTIME DateAndTime() const;
    stdStr _OrigPath;
    stdStr _CurrPath;
    bool showSize_;
    bool showSeconds_;
    bool showTime_;
    bool showDate_;
    bool showAttrib_;
  };

  //----< return last path searched >----------------------------

  template <typename stdStr>
  inline stdStr FileInfo<stdStr>::lastSetPath() { return _CurrPath; }

  //----< return file name >-------------------------------------

  template <>
  inline std::string FileInfo<std::string>::name() const { return data.cFileName; }

  template <>
  inline std::wstring FileInfo<std::wstring>::name() const { return wdata.cFileName; }

  //----< less than operator >-----------------------------------

  template <>
  inline bool FileInfo<std::string>::operator<(const FileInfo<std::string> &fd) const 
  {
    return (strcmp(data.cFileName,fd.data.cFileName) == -1);
  }

  template <>
  inline bool FileInfo<std::wstring>::operator<(const FileInfo<std::wstring> &fd) const 
  {
    return (wcscmp(wdata.cFileName,fd.wdata.cFileName) == -1);
  }

  //----< comparison operator >----------------------------------

  template <>
  inline bool FileInfo<std::string>::operator==(const FileInfo<std::string> &fi) const 
  {  
    return (strcmp(data.cFileName,fi.data.cFileName) == 0);
  }

  template <>
  inline bool FileInfo<std::wstring>::operator==(const FileInfo<std::wstring> &fi) const 
  {  
    return (wcscmp(wdata.cFileName,fi.wdata.cFileName) == 0);
  }
  //----< comparison of file time and date >---------------------

  template <>
  inline bool FileInfo<std::string>::earlier(const FileInfo<std::string> &fi) const 
  {
    return (
      CompareFileTime(&data.ftLastWriteTime,&fi.data.ftLastWriteTime) == -1
    );
  }

  template <>
  inline bool FileInfo<std::wstring>::earlier(const FileInfo<std::wstring> &fi) const 
  {
    return (
      CompareFileTime(&wdata.ftLastWriteTime,&fi.wdata.ftLastWriteTime) == -1
    );
  }
  //----< comparison of file time and date >---------------------

  template <>
  inline bool FileInfo<std::string>::later(const FileInfo<std::string> &fi) const 
  {
    return (
      CompareFileTime(&data.ftLastWriteTime,&fi.data.ftLastWriteTime) == 1
    );
  }
  
  template <>
  inline bool FileInfo<std::wstring>::later(const FileInfo<std::wstring> &fi) const 
  {
    return (
      CompareFileTime(&wdata.ftLastWriteTime,&fi.wdata.ftLastWriteTime) == 1
    );
  }
  //----< display size? >----------------------------------------

  template <typename stdStr>
  inline void FileInfo<stdStr>::showSize(bool show_size)
  {
    showSize_ = show_size;
  }
  //----< display time? >----------------------------------------

  template <typename stdStr>
  inline void FileInfo<stdStr>::showTime(bool show_time)
  {
    showTime_ = show_time;
  }
  //----< display seconds in file time? >------------------------

  template <typename stdStr>
  inline void FileInfo<stdStr>::showSeconds(bool show_seconds)
  {
    showSeconds_ = show_seconds;
  }
  //----< display date? >----------------------------------------

  template <typename stdStr>
  inline void FileInfo<stdStr>::showDate(bool show_date)
  {
    showDate_ = show_date;
  }
  //----< display attributes? >----------------------------------

  template <typename stdStr>
  inline void FileInfo<stdStr>::showAttributes(bool show_attrib)
  {
    showAttrib_ = show_attrib;
  }

  //----< void constructor >-------------------------------------

  template <typename stdStr>
  inline FileInfo<stdStr>::FileInfo()
          : showSeconds_(false), showTime_(true), showDate_(true),
            showSize_(true), showAttrib_(true)
  { 
    _CurrPath = _OrigPath = getPath();
    _handle = INVALID_HANDLE_VALUE;
  }
  //----< constructor taking path >------------------------------

  template <typename stdStr>
  inline FileInfo<stdStr>::FileInfo(const stdStr &path) 
          : showSeconds_(false), showTime_(true), showDate_(true),
            showSize_(true), showAttrib_(true)
  {
    _handle = INVALID_HANDLE_VALUE;
    _OrigPath = getPath();
    setPath(path);
  }
  //----< copy constructor >-------------------------------------

  template <>
  inline FileInfo<std::string>::FileInfo(const FileInfo<std::string> &fi) 
  : showSeconds_(fi.showSeconds_), showTime_(fi.showTime_),
  showDate_(fi.showDate_), showSize_(fi.showSize_), showAttrib_(fi.showAttrib_)
  {
    data.dwFileAttributes    = fi.data.dwFileAttributes;
    data.ftCreationTime      = fi.data.ftCreationTime;  
    data.ftLastAccessTime    = fi.data.ftLastAccessTime;
    data.ftLastWriteTime     = fi.data.ftLastWriteTime; 
    data.nFileSizeHigh       = fi.data.nFileSizeHigh;
    data.nFileSizeLow        = fi.data.nFileSizeLow; 
    data.dwReserved0         = fi.data.dwReserved0;
    data.dwReserved1         = fi.data.dwReserved1;
    int i;
    for(i=0; i<MAX_PATH; i++) 
    {
      data.cFileName[i]      = fi.data.cFileName[i];
      if(fi.data.cFileName[i] == '\0')
        break;
    }
    for(i=0; i<14; i++)
      data.cAlternateFileName[i] = fi.data.cAlternateFileName[i];

    _OrigPath = fi._OrigPath;
    _CurrPath = fi._CurrPath;
    showSeconds_ = fi.showSeconds_;
    _handle = fi._handle;
  }

  template <>
  inline FileInfo<std::wstring>::FileInfo(const FileInfo<std::wstring> &fi) 
  : showSeconds_(fi.showSeconds_), showTime_(fi.showTime_),
  showDate_(fi.showDate_), showSize_(fi.showSize_), showAttrib_(fi.showAttrib_)
  {
    wdata.dwFileAttributes    = fi.wdata.dwFileAttributes;
    wdata.ftCreationTime      = fi.wdata.ftCreationTime;  
    wdata.ftLastAccessTime    = fi.wdata.ftLastAccessTime;
    wdata.ftLastWriteTime     = fi.wdata.ftLastWriteTime; 
    wdata.nFileSizeHigh       = fi.wdata.nFileSizeHigh;
    wdata.nFileSizeLow        = fi.wdata.nFileSizeLow; 
    wdata.dwReserved0         = fi.wdata.dwReserved0;
    wdata.dwReserved1         = fi.wdata.dwReserved1;
    int i;
    for(i=0; i<MAX_PATH; i++) 
    {
      wdata.cFileName[i]      = fi.wdata.cFileName[i];
      if(fi.wdata.cFileName[i] == L'\0')
        break;
    }
    for(i=0; i<14; i++)
      wdata.cAlternateFileName[i] = fi.wdata.cAlternateFileName[i];

    _OrigPath = fi._OrigPath;
    _CurrPath = fi._CurrPath;
    showSeconds_ = fi.showSeconds_;
    _handle = fi._handle;
  }
  //----< destructor >-------------------------------------------

  template <typename stdStr>
  inline FileInfo<stdStr>::~FileInfo() { setPath(_OrigPath); }

  //----< assignment operator >----------------------------------

  template <>
  inline FileInfo<std::string>& FileInfo<std::string>::operator=(const FileInfo<std::string> &fi) 
  {
    if(this == &fi) return *this;
    data.dwFileAttributes    = fi.data.dwFileAttributes;
    data.ftCreationTime      = fi.data.ftCreationTime;  
    data.ftLastAccessTime    = fi.data.ftLastAccessTime;
    data.ftLastWriteTime     = fi.data.ftLastWriteTime; 
    data.nFileSizeHigh       = fi.data.nFileSizeHigh;
    data.nFileSizeLow        = fi.data.nFileSizeLow; 
    data.dwReserved0         = fi.data.dwReserved0;
    data.dwReserved1         = fi.data.dwReserved1;
    showSize_ = fi.showSize_;
    showSeconds_ = fi.showSeconds_;
    showTime_ = fi.showTime_;
    showDate_ = fi.showDate_;
    showAttrib_ = fi.showAttrib_;
    int i;
    for(i=0; i<MAX_PATH; i++)
      data.cFileName[i] = fi.data.cFileName[i];
    for(i=0; i<14; i++)
      data.cAlternateFileName[i] = fi.data.cAlternateFileName[i];
    _OrigPath = fi._OrigPath;
    _CurrPath = fi._CurrPath;

    return *this;
  }

  template <>
  inline FileInfo<std::wstring>& FileInfo<std::wstring>::operator=(const FileInfo<std::wstring> &fi) 
  {
    if(this == &fi) return *this;
    wdata.dwFileAttributes    = fi.wdata.dwFileAttributes;
    wdata.ftCreationTime      = fi.wdata.ftCreationTime;  
    wdata.ftLastAccessTime    = fi.wdata.ftLastAccessTime;
    wdata.ftLastWriteTime     = fi.wdata.ftLastWriteTime; 
    wdata.nFileSizeHigh       = fi.wdata.nFileSizeHigh;
    wdata.nFileSizeLow        = fi.wdata.nFileSizeLow; 
    wdata.dwReserved0         = fi.wdata.dwReserved0;
    wdata.dwReserved1         = fi.wdata.dwReserved1;
    showSize_ = fi.showSize_;
    showSeconds_ = fi.showSeconds_;
    showTime_ = fi.showTime_;
    showDate_ = fi.showDate_;
    showAttrib_ = fi.showAttrib_;
    int i;
    for(i=0; i<MAX_PATH; i++)
      wdata.cFileName[i]      = fi.wdata.cFileName[i];
    for(i=0; i<14; i++)
      wdata.cAlternateFileName[i] = fi.wdata.cAlternateFileName[i];
    _OrigPath = fi._OrigPath;
    _CurrPath = fi._CurrPath;

    return *this;
  }
  //----< get file size >----------------------------------------

  template <>
  inline unsigned long int FileInfo<std::string>::size() const 
  {
    DWORDLONG myDWL = (static_cast<DWORDLONG>(data.nFileSizeHigh) << 32);
    myDWL += (data.nFileSizeLow & 0xFFFFFFFF);
    return static_cast<unsigned long int>(myDWL);
  }

  template <>
  inline unsigned long int FileInfo<std::wstring>::size() const 
  {
    DWORDLONG myDWL = (static_cast<DWORDLONG>(wdata.nFileSizeHigh) << 32);
    myDWL += (wdata.nFileSizeLow & 0xFFFFFFFF);
    return static_cast<unsigned long int>(myDWL);
  }
  //----< is my size smaller? >----------------------------------

  template <>
  inline bool FileInfo<std::string>::smaller(const FileInfo<std::string> &fi) const 
  {  
    DWORDLONG myDWL = (static_cast<DWORDLONG>(data.nFileSizeHigh) << 32);
    myDWL += data.nFileSizeLow & 0xFFFFFFFF;
    DWORDLONG fiDWL = (static_cast<DWORDLONG>(fi.data.nFileSizeHigh) << 32);
    fiDWL += fi.data.nFileSizeLow & 0xFFFFFFFF;
    return (myDWL < fiDWL);
  }

  template <>
  inline bool FileInfo<std::wstring>::smaller(const FileInfo<std::wstring> &fi) const 
  {  
    DWORDLONG myDWL = (static_cast<DWORDLONG>(wdata.nFileSizeHigh) << 32);
    myDWL += wdata.nFileSizeLow & 0xFFFFFFFF;
    DWORDLONG fiDWL = (static_cast<DWORDLONG>(fi.wdata.nFileSizeHigh) << 32);
    fiDWL += fi.wdata.nFileSizeLow & 0xFFFFFFFF;
    return (myDWL < fiDWL);
  }
  //----< is my size larger? >-----------------------------------

  template <>
  inline bool FileInfo<std::string>::larger(const FileInfo<std::string> &fi) const 
  {  
    DWORDLONG myDWL = (static_cast<DWORDLONG>(data.nFileSizeHigh) << 32);
    myDWL += data.nFileSizeLow & 0xFFFFFFFF;
    DWORDLONG fiDWL = (static_cast<DWORDLONG>(fi.data.nFileSizeHigh) << 32);
    fiDWL += fi.data.nFileSizeLow & 0xFFFFFFFF;
    return (myDWL > fiDWL);
  }

  template <>
  inline bool FileInfo<std::wstring>::larger(const FileInfo<std::wstring> &fi) const 
  {  
    DWORDLONG myDWL = (static_cast<DWORDLONG>(wdata.nFileSizeHigh) << 32);
    myDWL += wdata.nFileSizeLow & 0xFFFFFFFF;
    DWORDLONG fiDWL = (static_cast<DWORDLONG>(fi.wdata.nFileSizeHigh) << 32);
    fiDWL += fi.wdata.nFileSizeLow & 0xFFFFFFFF;
    return (myDWL > fiDWL);
  }
  //----< return FILETIME >--------------------------------------
  //
  //  typedef struct _FILETIME
  //  { DWORD dwLowDateTime;  DWORD dwHighDateTime; } FILETIME;
  //
  template <>
  inline FILETIME FileInfo<std::string>::getFILETIME()
  {
    FILETIME lft;
    FileTimeToLocalFileTime(&data.ftLastWriteTime,&lft);
    return lft;
  }

  template <>
  inline FILETIME FileInfo<std::wstring>::getFILETIME()
  {
    FILETIME lft;
    FileTimeToLocalFileTime(&wdata.ftLastWriteTime,&lft);
    return lft;
  }
  //----< compare file times >-----------------------------------
  //
  //  returns 1 if my FILETIME is later than ft
  //          0 if my FILETIME equals ft
  //         -1 if my FILETIME is earlier than ft
  //
  template <typename stdStr>
  inline int FileInfo<stdStr>::compareFileTime(FILETIME& ft)
  {
    FILETIME myft = getFILETIME();
    return (int)::CompareFileTime(&myft,&ft);
  }
  //----< private date and time extraction >---------------------

  template <>
  inline SYSTEMTIME FileInfo<std::string>::DateAndTime() const 
  {
   SYSTEMTIME st;
   FILETIME  lft;
   FileTimeToLocalFileTime(&data.ftLastWriteTime,&lft);
   FileTimeToSystemTime(&lft,&st);
   return st;
  }

  template <>
  inline SYSTEMTIME FileInfo<std::wstring>::DateAndTime() const 
  {
   SYSTEMTIME st;
   FILETIME  lft;
   FileTimeToLocalFileTime(&wdata.ftLastWriteTime,&lft);
   FileTimeToSystemTime(&lft,&st);
   return st;
  }
  //----< get file date string >---------------------------------

  template <>
  inline std::string FileInfo<std::string>::date() const 
  {
    SYSTEMTIME st = DateAndTime();
    std::string date;
    date.resize(0);
    date += '0' + (st.wMonth / 10);
    date += '0' + (st.wMonth % 10);
    date += '/';
    date += '0' + (st.wDay / 10);
    date += '0' + (st.wDay % 10);
    date += '/';
    int tmp = st.wYear;
    date += '0' + (tmp/1000);
    tmp  %= 1000;
    date += '0' + (tmp / 100);
    tmp %= 100;
    date += '0' + (tmp / 10);
    tmp %= 10;
    date += '0' + (tmp);
    return date;
  }

  template <>
  inline std::wstring FileInfo<std::wstring>::date() const 
  {
    SYSTEMTIME st = DateAndTime();
    std::wstring date;
    date.resize(0);
    date += L'0' + (st.wMonth / 10);
    date += L'0' + (st.wMonth % 10);
    date += L'/';
    date += L'0' + (st.wDay / 10);
    date += L'0' + (st.wDay % 10);
    date += L'/';
    int tmp = st.wYear;
    date += L'0' + (tmp/1000);
    tmp  %= 1000;
    date += L'0' + (tmp / 100);
    tmp %= 100;
    date += L'0' + (tmp / 10);
    tmp %= 10;
    date += L'0' + (tmp);
    return date;
  }
  //----< get file time string >---------------------------------

  template <>
  inline std::string FileInfo<std::string>::time() const 
  {
    SYSTEMTIME st = DateAndTime();
    std::string time;
    std::string AMPM = "am";
    if(st.wHour > 12) 
    {
      st.wHour -= 12;
      AMPM = "pm";
    }
    time.erase();
    time += '0' + (st.wHour   / 10);
    time += '0' + (st.wHour   % 10);
    time += ':';
    time += '0' + (st.wMinute / 10);
    time += '0' + (st.wMinute % 10);
    if(showSeconds_)
    {
      time += ':';
      time += '0' + (st.wSecond / 10);
      time += '0' + (st.wSecond % 10);
    }
    time += " ";
    time += AMPM;
    return time;
  }

  template <>
  inline std::wstring FileInfo<std::wstring>::time() const 
  {
    SYSTEMTIME st = DateAndTime();
    std::wstring time;
    std::wstring AMPM = L"am";
    if(st.wHour > 12) 
    {
      st.wHour -= 12;
      AMPM = L"pm";
    }
    time.erase();
    time += L'0' + (st.wHour   / 10);
    time += L'0' + (st.wHour   % 10);
    time += L':';
    time += L'0' + (st.wMinute / 10);
    time += L'0' + (st.wMinute % 10);
    if(showSeconds_)
    {
      time += L':';
      time += L'0' + (st.wSecond / 10);
      time += L'0' + (st.wSecond % 10);
    }
    time += L" ";
    time += AMPM;
    return time;
  }
  //----< make attributes string >-------------------------------

  template <typename stdStr>
  inline stdStr FileInfo<stdStr>::attributes() const 
  {
    stdStr temp;
    if(isArchive()   ) temp += 'A';
    if(isCompressed()) temp += 'C';
    if(isDirectory() ) temp += 'D';
    if(isEncrypted() ) temp += 'E';
    if(isHidden()    ) temp += 'H';
    if(isOffLine()   ) temp += 'O';
    if(isReadOnly()  ) temp += 'R';
    if(isSystem()    ) temp += 'S';
    if(isTemporary() ) temp += 'T';
    return temp;
  }
  //----< pragma needed to disable performance warning >---------
  //
  //  for some reason casts don't inhibit warning as they should

  #pragma warning(disable : 4800)

  //----< is this file Archive? >--------------------------------

  template <>
  inline bool FileInfo<std::string>::isArchive() const 
  { 
    return (data.dwFileAttributes & FILE_ATTRIBUTE_ARCHIVE);
  }

  template <>
  inline bool FileInfo<std::wstring>::isArchive() const 
  { 
    return (wdata.dwFileAttributes & FILE_ATTRIBUTE_ARCHIVE);
  }
  //----< is this file Compressed? >-----------------------------

  template <>
  inline bool FileInfo<std::string>::isCompressed() const 
  {   
    return (data.dwFileAttributes & FILE_ATTRIBUTE_COMPRESSED); 
  }

  template <>
  inline bool FileInfo<std::wstring>::isCompressed() const 
  {   
    return (wdata.dwFileAttributes & FILE_ATTRIBUTE_COMPRESSED); 
  }
  //----< is this file Directory? >------------------------------

  template <>
  inline bool FileInfo<std::string>::isDirectory() const 
  {   
    return (data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY); 
  }

  template <>
  inline bool FileInfo<std::wstring>::isDirectory() const 
  {   
    return (wdata.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY); 
  }
  //----< is this file Encrypted? >------------------------------

  template <>
  inline bool FileInfo<std::string>::isEncrypted() const 
  {   
    return (data.dwFileAttributes & FILE_ATTRIBUTE_ENCRYPTED); 
  }

  template <>
  inline bool FileInfo<std::wstring>::isEncrypted() const 
  {   
    return (wdata.dwFileAttributes & FILE_ATTRIBUTE_ENCRYPTED); 
  }
  //----< is this file Hidden? >---------------------------------

  template <>
  inline bool FileInfo<std::string>::isHidden() const 
  {   
    return (data.dwFileAttributes & FILE_ATTRIBUTE_HIDDEN); 
  }

  template <>
  inline bool FileInfo<std::wstring>::isHidden() const 
  {   
    return (wdata.dwFileAttributes & FILE_ATTRIBUTE_HIDDEN); 
  }
  //----< is this file Normal? >---------------------------------

  template <>
  inline bool FileInfo<std::string>::isNormal() const 
  {   
    return (data.dwFileAttributes & FILE_ATTRIBUTE_NORMAL); 
  }

  template <>
  inline bool FileInfo<std::wstring>::isNormal() const 
  {   
    return (wdata.dwFileAttributes & FILE_ATTRIBUTE_NORMAL); 
  }
  //----< is this file OffLine? >--------------------------------

  template <>
  inline bool FileInfo<std::string>::isOffLine() const 
  {   
    return (data.dwFileAttributes & FILE_ATTRIBUTE_OFFLINE); 
  }

  template <>
  inline bool FileInfo<std::wstring>::isOffLine() const 
  {   
    return (wdata.dwFileAttributes & FILE_ATTRIBUTE_OFFLINE); 
  }
  //----< is this file ReadOnly? >-------------------------------

  template <>
  inline bool FileInfo<std::string>::isReadOnly() const 
  {   
    return (data.dwFileAttributes & FILE_ATTRIBUTE_READONLY); 
  }

  template <>
  inline bool FileInfo<std::wstring>::isReadOnly() const 
  {   
    return (wdata.dwFileAttributes & FILE_ATTRIBUTE_READONLY); 
  }
  //----< is this file System? >---------------------------------

  template <>
  inline bool FileInfo<std::string>::isSystem() const 
  {   
    return (data.dwFileAttributes & FILE_ATTRIBUTE_SYSTEM); 
  }

  template <>
  inline bool FileInfo<std::wstring>::isSystem() const 
  {   
    return (wdata.dwFileAttributes & FILE_ATTRIBUTE_SYSTEM); 
  }
  //----< is this file Temporary? >------------------------------

  template <>
  inline bool FileInfo<std::string>::isTemporary() const 
  {   
    return (data.dwFileAttributes & FILE_ATTRIBUTE_TEMPORARY); 
  }

  template <>
  inline bool FileInfo<std::wstring>::isTemporary() const 
  {   
    return (wdata.dwFileAttributes & FILE_ATTRIBUTE_TEMPORARY); 
  }
  //----< display line of file data >----------------------------

  template <>
  inline void FileInfo<std::string>::showData(std::ostream &out, char ch, int width) const 
  {
    long save = out.flags();
    out.setf(std::ios::right, std::ios::adjustfield);
    if(showSize_)
      out << std::setw(10) << size();
    if(showDate_)
      out << "  " << date().c_str();
    if(showTime_ && showDate_)
      out << "  " << time().c_str();
    if(showAttrib_)
      out << "  " << attributes().c_str();
    out.setf(std::ios::left, std::ios::adjustfield);
    out << "  " << std::setw(width) << name().c_str();
    out << ch;
    out.flush();
    out.flags(save);
  }

  template <>
  inline void FileInfo<std::wstring>::showData(std::wostream &out, wchar_t ch, int width) const 
  {
    long save = out.flags();
    out.setf(std::ios::right, std::ios::adjustfield);
    if(showSize_)
      out << std::setw(10) << size();
    if(showDate_)
      out << L"  " << date().c_str();
    if(showTime_ && showDate_)
      out << L"  " << time().c_str();
    if(showAttrib_)
      out << L"  " << attributes().c_str();
    out.setf(std::ios::left, std::ios::adjustfield);
    out << L"  " << std::setw(width) << name().c_str();
    out << ch;
    out.flush();
    out.flags(save);
  }
  //----< get current searchPath >-------------------------------
  //
  //  fill with current directory path if empty
  //
  template <>
  inline std::string FileInfo<std::string>::getPath() 
  {
    char buffer[256];
    ::GetCurrentDirectoryA(256,buffer);
    return std::string(buffer);
  }

  template <>
  inline std::wstring FileInfo<std::wstring>::getPath() 
  {
    wchar_t buffer[256];
    ::GetCurrentDirectoryW(256,buffer);
    return std::wstring(buffer);
  }
  //----< set current directory >--------------------------------

  template <>
  inline void FileInfo<std::string>::setPath(const std::string& s) 
  {
    if(::SetCurrentDirectoryA(s.c_str()) == 0)
    {
      std::string temp = s + " - path not found";
      throw std::invalid_argument(temp.c_str());
    }
    _CurrPath = s;
  }

  template <>
  inline void FileInfo<std::wstring>::setPath(const std::wstring& s) 
  {
    if(::SetCurrentDirectoryW(s.c_str()) == 0)
    {
      std::string temp = Convert::ToString(s) + " - path not found";
      throw std::invalid_argument(temp.c_str());
    }
    _CurrPath = s;
  }
  //----< find first file >--------------------------------------

  template <>
  inline bool FileInfo<std::string>::firstFile(std::string path, const std::string& filePattern) 
  {
    if(_handle != INVALID_HANDLE_VALUE)
      closeFile();    // close any previous search
    if(path[path.size()-1] != '\\')
      path.append("\\");
    path.append(filePattern);
    _handle = ::FindFirstFileA(path.c_str(),&data);
    return (_handle != INVALID_HANDLE_VALUE);
  }

  template <>
  inline bool FileInfo<std::wstring>::firstFile(std::wstring path, const std::wstring& filePattern) 
  {
    if(_handle != INVALID_HANDLE_VALUE)
      closeFile();    // close any previous search
    if(path[path.size()-1] != L'\\')
      path.append(L"\\");
    path.append(filePattern);
    std::wstring tempPath = path;
    _handle = ::FindFirstFileW(tempPath.c_str(),&wdata);
    return (_handle != INVALID_HANDLE_VALUE);
  }
  //----< find first file >--------------------------------------

  template <typename stdStr>
  inline bool FileInfo<stdStr>::firstFile(const stdStr &filePattern) 
  {
    stdStr path = getPath();
    return firstFile(path,filePattern);
  }

  //----< find first file >--------------------------------------

  template <>
  inline bool FileInfo<std::string>::firstFile() 
  {
    std::string path = FileInfo<std::string>::getPath();
    return firstFile(path,"*.*");
  }

  template <>
  inline bool FileInfo<std::wstring>::firstFile() 
  {
    std::wstring path = FileInfo<std::wstring>::getPath();
    return firstFile(path,L"*.*");
  }
  //----< find next file >---------------------------------------

  template <>
  inline bool FileInfo<std::string>::nextFile() 
  {
    if(_handle == INVALID_HANDLE_VALUE)
      return false;
    return (::FindNextFileA(_handle,&data) == TRUE);
  }

  template <>
  inline bool FileInfo<std::wstring>::nextFile() 
  {
    if(_handle == INVALID_HANDLE_VALUE)
      return false;
    return (::FindNextFileW(_handle,&wdata) == TRUE);
  }
  //----< close search for current file >------------------------

  template <typename stdStr>
  inline void FileInfo<stdStr>::closeFile() {  FindClose(_handle); }

  typedef FileInfo<std::string> fileInfo;
  typedef FileInfo<std::wstring> wfileInfo;
}
#endif