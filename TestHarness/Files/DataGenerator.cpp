///////////////////////////////////////////////////////////////////////
// XmlReader.cpp - Parse XML strings                                 //
// ver 2.1                                                           //
// Language:    Visual C++, Visual Studio 2010, SP1                  //
// platform:    Windows 7, Home Premium, SP1   //
// Application: OOD Projects1                                 //
//Author:       Prajwal Mahesh Mamledesai
//				MS Computer Engineering, pmamlede@syr.edu, (315)-600-6662
/////////////////////////////////////////////////////////////////////////


#include <iostream>
#include <fstream>
#include "DataGenerator.h"
#include <string>
#include <time.h>

//<--------------taking file as a input and reads it and converts it in to string and returns it--->
std::string Generators :: StringGenerator(std :: string& files)
{	int position=0;
	std::fstream x;
	x.open(files);
    std::ifstream s(files);
	while(!x.eof()){
		value.push_back((char) x.get());
		x.seekg(++position);
	}
		std :: cout << value;
	
	return value;
}

//<--------------------logs the data in to the log files---------->
void Generators :: Datalogger(std :: string& input)
{
	
 
	time_t now = time(0);
    struct tm* tm = localtime(&now);    
   std:: ofstream out( "../Files/log.txt",std::ios::app);
    out << tm->tm_year << '/' << tm->tm_mon << '/' << tm->tm_mday
         << ' ' << tm->tm_hour << ':' << tm->tm_min << ':' << tm->tm_sec << ": ";
    out << input << "\n";
	 out.close();
	 std::cout << tm->tm_year << '/' << tm->tm_mon << '/' << tm->tm_mday
         << ' ' << tm->tm_hour << ':' << tm->tm_min << ':' << tm->tm_sec << ": ";
	 std::cout << input <<"\n";

}

/*-----------Test stub-------------*/
#ifdef Test
int main(int argc,char *argv[]){
	FileMgr fm(argc, argv);
	Generator check;
	std::string path=argv[1];
FileMgr::fileSet fs1 = fm.FindFiles(path, "*.txt");
std::string output= check.StringGenerator(fs1[0]);
check.Datalogger(output);
}
#endif