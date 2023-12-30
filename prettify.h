//============================================================================
// Name        : Prettify.h
// Author      : Noha Adel ~.~
// Description : Header file for a the prettify/formatting function, it's responsible for
//               formatting an xml file so that the output has correct indentation levels
//============================================================================

using namespace std;

#ifndef PRETTIFY_H_
#define PRETTIFY_H_

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

string removeLeadingTabs(const string& line) ;

/**
 * @name: removeLeadingTabs
 *
 * @brief: this function removes the white spaces at the beginning of a string.
 * it's used in the context of prettifying an XML file as a pre-step to
 * remove any unnecessary tabs and white spaces existing at the start of
 * a line in the file.
 *
 * @param: line --> this is the input string.
 *
 * @return: returns a string starting from the first non-white-space index.
 */

void separateTags(const string& inputFileName, const string& outputFileName);
/**
 * @name: separateTags
 *
 * @brief: this function mainly separates the content of an XML file such that:
 * each opening tag, closing tag, data is separated on a new line. This is also a pre-step for handling
 * any complex formatting cases of an XML file.
 * the output of this step is then used as the input to the last step in the formatting process.
 *
 * @param: inputFileName --> this is the directory of the input XML file we want to prettify.
 * @param: outputFileName --> this is the directory of an empty file to be used as an intermediate file to hold
 * the output of this step.
 *
 * @return: void
 */

void prettifyXML(const string& inputFileName, const string& outputFileName);
/**
 * @name: prettifyXML
 * @brief: this is the final step of the formatting process, in which we examine the contents of the file
 * and upon which we specify indentation levels that formats the XML file to be more presentable.
 * @param: inputFileName --> this is the directory of the file resulting from step 2 (output of the separateTags function).
 * @param: outputFileName --> this is the directory of an empty file to hold the final output of the formatting process :).
 * the output of this step.
 *
 * @return: void
 */



#endif /* PRETTIFY_H_ */
