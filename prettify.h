/*
 * prettify.h
 *
 *  Created on: Dec 28, 2023
 *      Author: noha3
 */

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
using namespace std;

#ifndef PRETTIFY_H_
#define PRETTIFY_H_


string removeLeadingTabs(const string& line) ;
void separateTags(const string& inputFileName);
void prettifyXML(const string& inputFileName);



#endif /* PRETTIFY_H_ */
