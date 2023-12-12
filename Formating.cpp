#include "Formating.h"
/*
 * Description :
 * Functional responsible for Minifying the XML filr:
 * Inputs:
 *		  input-> the xml file read into a char vector
 */
void minify(vector<char>& input)
{
	for (int i = 0; i < input.size(); i++)
	{
		if (input[i]=='\n'|| input[i] == '\r' || input[i] == 9 || input[i] == ' ')
		{
			input.erase(input.begin() + i);
			i--;
		}
	}
}