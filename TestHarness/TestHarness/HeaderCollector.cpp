///////////////////////////////////////////////////////////////////////
// XmlReader.cpp - Parse XML strings                                 //
// ver 2.1                                                           //
// Language:    Visual C++, Visual Studio 2010, SP1                  //
// platform:    Windows 7, Home Premium, SP1   //
// Application: OOD Projects1                                 //
//Author:       Prajwal Mahesh Mamledesai
//				MS Computer Engineering, pmamlede@syr.edu, (315)-600-6662
/////////////////////////////////////////////////////////////////////////

#include "FileMgr.h"
#include "HeaderCollector.h"
#include <vector>
#include "XmlReader.h"
#include "FileCollectors.h"
#include <fstream>
#include <string>

//<---------------------reads the headers of the files and puts the header in 2D vector---------------------->

void getHeader::Header(std::vector<std::string> fileNames){
std::string str; temp1=fileNames;
for(unsigned int i = 0; i < fileNames.size(); i++){
int r = strcmp(fileNames[i].c_str()," ");
if(r==1){
std::string value;int position=0;std::fstream file;
file.open(fileNames[i]);
std::ifstream f(fileNames[i]);
std::string line;int size=0;
while(!(file.eof()-1)){
size++;getline(file,line);
if(line.compare("")==0){
line.append("not blank");
}  }
int i=0;int j=0;
while(i<=(size-1)){
std::getline(f,value);
int q=strcmp(value.c_str()," ");
if(q != 0){
int length =value.length();
if(value[0]==35){
while(j<(length-1))
{j++;
while(value[j]==34){
while(value[++j] != 34){
str+=value[j];
}
temp.push_back(str);
str.clear();
break;
}
}
}
i++;
}
}
Headers.push_back(temp);
temp.clear();
}
}
}


//<----------------finds the root from 2D vector---------------->

int getHeader::RootFinding(){
int k=0; int count=0;
for(unsigned int k = 0; k < Headers.size(); k++){
for(unsigned int i = 0; i < Headers.size(); i++){
for(unsigned int j = 0; j < Headers[i].size(); j++){
std::cout << Headers[i][j].c_str()<<"\n" <<std::endl;
signed int	x=strcmp(Headers[i][j].c_str(),temp1[k].c_str());
if (x==0){
temp.push_back(temp1[k].c_str());}
}
}
}
if(temp.size()==0){
std::cout<<"\n"<<" roooooot is "<<temp1[0].c_str()<<"\n";
std::string head = "../Files/Header.txt";
std:: ofstream out( head,std::ios::app);
std::string putheader ="#include \"";
out<<"\n";
out<<putheader<<temp1[0].c_str()<<"\"";
}
for(unsigned int r=0;r<temp.size();r++){
for (unsigned int z=0;z<temp1.size();z++){
signed int	x1=strcmp(temp[r].c_str(),temp1[z].c_str());
if (x1!=0){
int len = temp1[z].length(); int l = len;
if(temp.size()==1){
std::cout<<"\n"<<" roooooot is "<<temp1[z].c_str()<<"\n";
std::string head = "../Files/Header.txt";
std:: ofstream out( head,std::ios::app);
std::string putheader ="#include \"";
out<<"\n";
out<<putheader<<temp1[0].c_str()<<"\"";
}
if(temp.size()!=1){
std::string a=temp[z];
if(a[l]==104){
std::cout<<"\n"<<" roooooot is "<<temp1[z].c_str()<<"\n";
std::string head = "../Files/Header.txt";
std:: ofstream out( head,std::ios::app);
std::string putheader ="#include \"";
out<<"\n";
out<<putheader<<temp1[0].c_str()<<"\"";
break;}
}
}
}
}
return 0;
}



/*<--------------Test Stub------------------->*/

#ifdef TEST_HeaderCollector

int main(int argc,char *argv[])
{
getHeader head;
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

std::vector<std::string> fileNames;                 //declaration of vectors for storing filenames------------>
fileNames = classpath.returnFiles();                //retreving the filenames------------------>
head.Header(fileNames);
head.RootFinding();           //<----------------root finding--------------------> 
}

#endif

