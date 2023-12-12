#ifndef Formating
#define Formating

#include <vector>

using namespace std;
/*
 * Description :
 * Functional responsible for Minifying the XML filr:
 * Inputs:
 *		  input-> the xml file read into a char vector
 */
void minify(vector<char>& input);

/*
 * Description :
 * Functional responsible for comoressing the XML filr:
 * Inputs:
 *		  input-> the xml file read into a char vector
 */
void compress(vector<char>& input);
void save_compressed_file(vector<char>& input, vector<char [2]> key, vector<vector<int> > index);

#endif /* Formating */