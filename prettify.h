/*
 * prettify.h
 *
 *  Created on: Dec 28, 2023
 *      Author: noha3
 */
#ifndef PRETTIFY_H_
#define PRETTIFY_H_

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
using namespace std;




string removeLeadingTabs(const string& line) ;
void separateTags(const string& inputFileName, const string& outputFileName);
void prettifyXML(const string& inputFileName, const string& outputFileName);



#endif /* PRETTIFY_H_ */
