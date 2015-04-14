///////////////////////////////////////////////////////////////////////
// XmlReader.cpp - Parse XML strings                                 //
// ver 2.1                                                           //
// Language:    Visual C++, Visual Studio 2010, SP1                  //
// platform:    Toshiba Portege R705, Windows 7, Home Premium, SP1   //
// Application: OOD 2012 Project1                                 //
//Author:       Prajwal Mahesh Mamledesai
//				MS Computer Engineering, pmamlede@syr.edu, (315)-600-6662
///////////////////////////////////////////////////////////////////////


#include "FileMgr.h"
#include "FileCollectors.h"
#include "HeaderCollector.h"

//<-----------reads the xml and stores the files in a vector------>
void getClassPath::path(std :: string& input)
	{
		int length = input.length();
	char stringToChar[100];
	int c = strcmp(input.c_str(),"FilePath");
	
    for (signed int i=0; i<length ; i++)
        {
            stringToChar[i] = input[i];
	}
	while(stringToChar[0] != '<' && stringToChar[0] != '32')
	{ 
		int len = input.length();
		Classpath.push_back(input);
		break;
	}
		for(unsigned int i = 0; i < Classpath.size(); i++)
		{
        std::cout << Classpath[i].c_str()<<"-------" << std::endl;
        }
	}
  std::vector<std::string> getClassPath :: returnFiles()
  {  return  Classpath;
  }

 //<----------Test Stub--------------->

#ifdef Test FileCollector
  
  int main(int argc,char *argv[])
  {
getClassPath classpath;
std::string path=argv[1];
FileMgr fm(argc, argv);
FileMgr::fileSet fs = fm.FindFiles(path, "*.xml");//<-----using file manager to find the XML----------->
int positions=0;
std::fstream x;
std::string values;
std::string line;
x.open(path + "\\" +fs[0]); 
std::ifstream s(path + "\\" +fs[0]);
//<---------------reading XML---------------------->
while(getline(x, line)){
values+=line;
}
std::string test1 = values;
XmlReader rdr(test1);
std::string ident;
size_t position = 1;
do
{
ident = rdr.extractIdentifier(position);
} while(ident.size() > 0);
rdr.reset();
while(rdr.next())
{
std::string Body=rdr.body().c_str();
classpath.path(Body);                               //<---------------------reading xml and storing file in a vector-------->
XmlReader::attribElems attribs = rdr.attributes();
for(size_t i=0; i<attribs.size(); ++i)
std::cout << "\n  attributes: " << attribs[i].first.c_str() << ", " << attribs[i].second.c_str();
}

  }
#endif
