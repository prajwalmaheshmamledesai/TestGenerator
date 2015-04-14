#ifndef GenPro_H
#define GenPro_H

/////////////////////////////////////////////////////////////////////////
// GenProj.cpp - demonstrates how to build project file from template
//
// 
//  Author:       Prajwal Mahesh Mamledesai
//				  MS Computer Engineering, pmamlede@syr.edu, (315)-600-6662
//  Reference:    Jim Fawcett, CSE687 - Object Oriented Design, Spring 2012	
// //////////////////////////////////////////////////////////////////////////


/*
  Module Operations: 
  ==================
  This module defines the class GenerateProject class and it 
  generates the Vcx Project

   Public Interface:
  =================
    int GenProj();
	std::string ReadToEnd(std::ifstream& in);

  Build Process:
  ==============
  Required files
    - GenerateProject.h, GenerateProject.cpp

  Maintenance History:
  ====================
  Ver 1.0 - 10th Feb 2012

  */
#include <fstream>
#include <string>
#include <iostream>
#include "XmlReader.h"
#include "XmlWriter.h"



class GenerateProject

{
public:
	int GenProj();
	std::string ReadToEnd(std::ifstream& in);
};
#endif