#include "Formating.h"
#include <iostream>
/*
 * Description :
 * Functional responsible for Minifying the XML filr:
 * Inputs:
 *		  input-> the xml file read into a char vector
 */
void minify(vector<char>& input)
{
	for (unsigned int i = 0; i < input.size(); i++)
	{
		if(input[i]< (input.size()-5) && input[i] =='<' && input[i+1] == 'b' && input[i+2] == 'o' && input[i+3] == 'd' && input[i+4] == 'y')
		{
			i++;
			while (input[i] != '<')
			{
				if (input[i] == '\n')
				{
					input.erase(input.begin() + i);
				}
				i++;
			}
		}
		if (input[i]=='\n'|| input[i] == '\r' || input[i] == 9 || input[i] == ' ')
		{
			input.erase(input.begin() + i);
			i--;
		}
	}
}

void compress(vector<char>& input)
{
	char count = -1;
	vector<vector<char>> key;
	vector<char> value;
	
	char general_flag = 1;
	while (general_flag == 1)
	{
		general_flag = 0;

		for (unsigned int i = 0; i < input.size() - 4; i++)
		{
			char addition_flag = 0;
			vector<char> temp{ input[i],input[i + 1] };
			for (unsigned int j = i + 2; j < input.size() - 1; j++)
			{
				if (temp[0] == input[j] && temp[1] == input[j + 1])
				{
					if (addition_flag == 0)
					{
						addition_flag = 1;
						value.push_back(count);
						key.push_back(temp);
					}
					input[j] = count;
					input.erase(input.begin() + j + 1);
				}
			}
			if (addition_flag == 1)
			{
				input[i] = count;
				input.erase(input.begin() + i + 1);
				count--;
				general_flag = 1;
			}
			if (count < -127)
			{
				break;
			}

		}
	}

}
void save_compressed_file(vector<char>& input, vector<char [2]> key, vector<vector<int> > index)
{

}