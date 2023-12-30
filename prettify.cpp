//============================================================================
// Name        : Prettify.cpp
// Author      : Noha Adel ~.~
// Description : CPP file for a the prettify/formatting function, it's responsible for
//               formatting an xml file so that the output has correct indentation levels
//============================================================================

#include "prettify.h"
using namespace std;

// Function to remove leading whitespaces (including tabs) from a string
string removeLeadingTabs(const string& line) {
	int start = line.find_first_not_of(" \t");

	if (start >= 0) {
		return line.substr(start);
	} else {
		return "";
	}
}

// Function to separate tags onto individual lines
void separateTags(const string& inputFileName, const string& outputFileName) {
	ifstream inputFile(inputFileName);
	ofstream outputFile(outputFileName);

	if (!inputFile.is_open() || !outputFile.is_open()) {
		cerr << "Error opening files." << std::endl;
		return;
	}

	string line;
	string openingTag;
	string data;

	string trimmedLine;
	size_t pos=0;
	size_t openingBracketPos=0;
	size_t closingBracketPos=0;

	while (getline(inputFile, line)) {
		// Remove leading tabs and whitespaces
		trimmedLine = removeLeadingTabs(line);

		// Separate tags onto individual lines
		pos = 0;
		while (pos < trimmedLine.size()) {
			openingBracketPos = trimmedLine.find('<', pos);
			closingBracketPos = trimmedLine.find('>', openingBracketPos);
			if(openingBracketPos == pos){
				if (openingBracketPos != string::npos && closingBracketPos != string::npos) {
					// Separate opening and closing tags onto individual lines
					openingTag = trimmedLine.substr(openingBracketPos, closingBracketPos - openingBracketPos + 1);
					outputFile << openingTag << endl;

					pos = closingBracketPos + 1;
				} else {
					// No tags found, print the rest of the line
					outputFile << trimmedLine.substr(pos);
					break;
				}
			}
			else{
				data = trimmedLine.substr(pos, openingBracketPos-pos);
				outputFile << data <<endl;
				pos = openingBracketPos;

			}
		}


	}

	inputFile.close();
	outputFile.close();
}

void prettifyXML(const string& inputFileName, const string& outputFileName) {

	ifstream inputFile(inputFileName);
	ofstream outputFile(outputFileName);


	//ofstream tempOutputFile("src/temp.xml");
	if (!inputFile.is_open() || !outputFile.is_open()) {
		cerr << "Error opening files." << std::endl;
		return;
	}

	string line;
	string tag;
	int indentationLevel = 0;
	int startTagPos;
	int endTagPos1;
	int closingStartTagPos;
	int endTagPos2;

	while (getline(inputFile, line))
	{
		startTagPos = line.find('<');
		endTagPos1 = line.find('>');
		closingStartTagPos = line.find("</");
		endTagPos2 = 0;

		//case of  opening tag-data-closing tag
		if (startTagPos>=0 && endTagPos1 >=0 && closingStartTagPos!=0)
		{
			for (int i = 0; i < indentationLevel; i++) {
				outputFile << "    ";
			}

			//if a closing tag is found
			if(closingStartTagPos >=0 ){
				//indentationLevel--;
				endTagPos2 = line.find('>' , endTagPos1+1); //hatly nehayetha el close tag fen
				tag = line.substr(startTagPos,endTagPos2 - startTagPos + 1);  //extract el line b'a 3la b3do mn awel el start-data-close
			}
			//if closing tag doesn't exist, meaing opening tag only or opening tag-data
			else{
				indentationLevel++;
				tag = line.substr(startTagPos, endTagPos1 - startTagPos + 1);
			}

			outputFile << tag <<endl;

		} else if(closingStartTagPos == 0 ){
			indentationLevel--;
			tag = line.substr(closingStartTagPos ,endTagPos1 +1);
			for (int i = 0; i < indentationLevel; i++) {
				outputFile << "    ";
			}
			outputFile << tag<<endl;

			startTagPos = line.find('<' , endTagPos1+1);
			if(startTagPos >0){
				endTagPos2 = line.find('>' , startTagPos+1);
				tag = line.substr(startTagPos , endTagPos2+1);
				for (int i = 0; i < indentationLevel; i++) {
					outputFile << "    ";
				}
				indentationLevel++;
				outputFile << tag<<endl;
			}
		}
		else
		{
			// Non-tag content
			for (int i = 0; i < indentationLevel; i++)
			{
				outputFile << "    ";
			}
			outputFile << line<< endl;
		}
	}

	remove("src/temp.xml");
	inputFile.close();
	outputFile.close();

}
