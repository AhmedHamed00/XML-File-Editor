//============================================================================
// Name        : DataStructureProjectPrettifyingTrial.cpp
// Author      : Noha Adel ~.~
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
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
void separateTags(const string& inputFileName) {
	ifstream inputFile(inputFileName);
	ofstream outputFile(inputFileName);

	if (!inputFile.is_open() || !outputFile.is_open()) {
		cerr << "Error opening files." << std::endl;
		return;
	}

	string line;
	string openingTag;
	string data;
	string new_file;

	while (getline(inputFile, line)) {
		// Remove leading tabs and whitespaces
		string trimmedLine = removeLeadingTabs(line);

		// Separate tags onto individual lines
		size_t pos = 0;
		while (pos < trimmedLine.size()) {
			size_t openingBracketPos = trimmedLine.find('<', pos);
			size_t closingBracketPos = trimmedLine.find('>', openingBracketPos);
			if(openingBracketPos == pos){
				if (openingBracketPos != string::npos && closingBracketPos != string::npos) {
					// Separate opening and closing tags onto individual lines
					openingTag = trimmedLine.substr(openingBracketPos, closingBracketPos - openingBracketPos + 1);
					new_file += openingTag + "\n";
					//outputFile << openingTag << endl;

					pos = closingBracketPos + 1;
				} else {
					// No tags found, print the rest of the line
					new_file +=  trimmedLine.substr(pos);
					//outputFile << trimmedLine.substr(pos);
					break;
				}
			}
			else{
				// we can handle here the case where the data length is too short e.g. id and name
					//that you can make it with its opening and closing tag in the same line bas msh adra walahy
				data = trimmedLine.substr(pos, openingBracketPos-pos);
				new_file += data + "\n";
				//outputFile << data <<endl;
				pos = openingBracketPos;

			}
		}


	}
	outputFile << new_file;

	inputFile.close();
	outputFile.close();
}

void prettifyXML(const string& inputFileName) {

	ifstream inputFile(inputFileName);
	ofstream outputFile(inputFileName);


	//ofstream tempOutputFile("src/temp.xml");
	if (!inputFile.is_open() || !outputFile.is_open()) {
		cerr << "Error opening files." << std::endl;
		return;
	}

	string line;
	string tag;
	string new_line ="";

	int indentationLevel = 0;

	while (getline(inputFile, line))
	{
		int startTagPos = line.find('<');
		int endTagPos1 = line.find('>');
		int closingStartTagPos = line.find("</");
		int endTagPos2 = 0;

		//el case bta3t start-data-end
		if (startTagPos>=0 && endTagPos1 >=0 && closingStartTagPos!=0)
		{
			for (int i = 0; i < indentationLevel; i++) {
				new_line+="    ";
			}

			//law la'et bedayet el close tag
			if(closingStartTagPos >=0 ){
				//indentationLevel--;
				endTagPos2 = line.find('>' , endTagPos1+1); //hatly nehayetha el close tag fen
				tag = line.substr(startTagPos,endTagPos2 - startTagPos + 1);  //extract el line b'a 3la b3do mn awel el start-data-close
			}
			//law el close tag msh mawgoda y3ny start bas aw start-data bas
			else{
				indentationLevel++;
				tag = line.substr(startTagPos, endTagPos1 - startTagPos + 1);
			}

			new_line+= tag + "\n";

		} else if(closingStartTagPos == 0 ){
			indentationLevel--;
			tag = line.substr(closingStartTagPos ,endTagPos1 +1);
			for (int i = 0; i < indentationLevel; i++) {
				new_line+="    ";
			}
			new_line+= tag + "\n";

			startTagPos = line.find('<' , endTagPos1+1);
			if(startTagPos >0){
				endTagPos2 = line.find('>' , startTagPos+1);
				tag = line.substr(startTagPos , endTagPos2+1);
				for (int i = 0; i < indentationLevel; i++) {
					new_line+="    ";
				}
				indentationLevel++;
				new_line+= tag + "\n";
			}
		}
		else
		{
			// Non-tag content
			for (int i = 0; i < indentationLevel; i++)
			{
				new_line+="    ";
			}
			new_line+= line + "\n";
		}
	}
	outputFile << new_line;

	//remove("src/temp.xml");
	inputFile.close();
	outputFile.close();

}

/*
int main() {

	separateTags("src/input.xml");
	prettifyXML("src/input.xml");

	return 0;
}
*/
