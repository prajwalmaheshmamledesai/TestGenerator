/////////////////////////////////////////////////////////////////////////
// GenProj.cpp - demonstrates how to build project file from template
//
// 
//  Author:       Prajwal Mahesh Mamledesai
//				  MS Computer Engineering, pmamlede@syr.edu, (315)-600-6662
//  Reference:    Jim Fawcett, CSE687 - Object Oriented Design, Spring 2012	
// //////////////////////////////////////////////////////////////////////////

#include <fstream>
#include <string>
#include <iostream>
#include "XmlReader.h"
#include "XmlWriter.h"

//----< read all text from the in stream into a returned string >--------

#include "GenerateProject.h"


std::string GenerateProject::ReadToEnd(std::ifstream& in)
{
  std::string temp;

  while(in.good())
  {
    temp += in.get();
  }
  return temp;
}

int GenerateProject::GenProj()
{
	const std::string testfile = "../Client/template.vcxproj";  
  std::ifstream in(testfile);
  if(!in.good())
  {
    std::cout << "\n  could not open file \"" << testfile << "\"\n\n";
    return 1;
  }
  std::string xmlStr = ReadToEnd(in);
  XmlReader rdr(xmlStr);
  while(rdr.next())
  {
	      if(rdr.tag() == "ClCompile")
    {
      size_t start = rdr.position() - 1;
      if((rdr.attributes().size() == 0) || (rdr.attributes()[0].first != "Include"))
        continue;
      size_t end = xmlStr.find("/>",start) + 2;
      rdr.position(start);

	  std::string dummyCpp = "<ClCompile Include=\"Client.cpp\" />";
      xmlStr = xmlStr.replace(start,end-start,dummyCpp.c_str());
      rdr.next();
    }
    else if(rdr.tag() == "ClInclude")
    {
      size_t start = rdr.position() - 1;
      if((rdr.attributes().size() == 0) || (rdr.attributes()[0].first != "Include"))
        continue;
      size_t end = xmlStr.find("/>",start) + 2;
      rdr.position(start);
      std::string dummyCpp = "<ClCompile Include=\"ITest.h\" />";
      xmlStr = xmlStr.replace(start,end-start,dummyCpp.c_str());
      rdr.next();

	  std::string dummyCpp1 = "<ClCompile Include=\"IGenerator.h\" />";
      xmlStr = xmlStr.replace(start,end-start,dummyCpp.c_str());
      rdr.next();
	  std::string dummyCpp2 = "<ClCompile Include=\"ILogger.h\" />";
      xmlStr = xmlStr.replace(start,end-start,dummyCpp.c_str());
      rdr.next();
    }
  }
  // to create your new project file.
  std::cout << "\n" << xmlStr.c_str() << "\n\n";
return 0;
}

//----< process template file to generate new project file >-------------
#ifdef TEST_XMLREADER
int main()
{
  // the template is a copy of GenProj.vcxproj with all the cpp files
  // and h files replaced with dummy elements

  const std::string testfile = "template.vcxproj";  
  std::ifstream in(testfile);
  if(!in.good())
  {
    std::cout << "\n  could not open file \"" << testfile << "\"\n\n";
    return 1;
  }
  std::string xmlStr = ReadToEnd(in);
  XmlReader rdr(xmlStr);

  // - find template's <ClCompile Include="dummy.cpp"> element
  //   and replace with <ClCompile Include="your.cpp"> elements
  // - find template's <ClInclude Include="dummy.h"> element
  //   and replace with <ClInclude Include="your.h"> elements
  //
  // - in this demo we will only do one substition for each

  while(rdr.next())
  {
    if(rdr.tag() == "ClCompile")
    {
      // does this element have "Include" attribute
      size_t start = rdr.position() - 1;
      if((rdr.attributes().size() == 0) || (rdr.attributes()[0].first != "Include"))
        continue;
      size_t end = xmlStr.find("/>",start) + 2;
      rdr.position(start);

      // here's where you insert your cpp files

      std::string dummyCpp = "<ClCompile Include=\"Client.cpp\" />";
      xmlStr = xmlStr.replace(start,end-start,dummyCpp.c_str());
      //std::cout << "\n" << xmlStr.c_str() << "\n\n";
      //return 1;
      rdr.next();
    }
    else if(rdr.tag() == "ClInclude")
    {
      // does this element have "Include" attribute
      size_t start = rdr.position() - 1;
      if((rdr.attributes().size() == 0) || (rdr.attributes()[0].first != "Include"))
        continue;
      size_t end = xmlStr.find("/>",start) + 2;
      rdr.position(start);

      // here's where you insert your h files

      std::string dummyCpp = "<ClCompile Include=\"ITest.h\" />";
      xmlStr = xmlStr.replace(start,end-start,dummyCpp.c_str());
      //std::cout << "\n" << xmlStr.c_str() << "\n\n";
      //return 1;
      rdr.next();
	  std::string dummyCpp1 = "<ClCompile Include=\"IGenerator.h\" />";
      xmlStr = xmlStr.replace(start,end-start,dummyCpp.c_str());
      //std::cout << "\n" << xmlStr.c_str() << "\n\n";
      //return 1;
      rdr.next();
	  std::string dummyCpp2 = "<ClCompile Include=\"ILogger.h\" />";
      xmlStr = xmlStr.replace(start,end-start,dummyCpp.c_str());
      //std::cout << "\n" << xmlStr.c_str() << "\n\n";
      //return 1;
      rdr.next();
    }
  }
  // Here, you should open an output file, and write xmlStr into it
  // to create your new project file.

  std::cout << "\n" << xmlStr.c_str() << "\n\n";

}
#endif