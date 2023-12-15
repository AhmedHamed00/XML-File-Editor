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
bool compareBycount(node* a,node* b)
{
	return a->occure > b->occure;
}
void compress_hoffman_encoding(vector<char>& input, vector<char>& output,tree& hoffman)
{
	vector<node> letters_count;
	vector<node*> letters_count_copy;
	for (int j = 0; j < 127; j++)
	{
		int count = 0;
		for (int i = 0; i < input.size(); i++)
		{
			if (input[i] == j)
			{
				count++;
			}
		}
		if (count != 0)
		{
			node x; 
			x.data = j;
			x.occure = count;
			letters_count.push_back(x);
			//letters_count_copy.push_back(x);
		}
	}
	sort(letters_count.begin(), letters_count.end(), compareBycount);
	for (int i = 0; i < letters_count.size(); i++)
	{
		letters_count_copy.push_back(&letters_count[i]);
	}
	while (letters_count_copy.size() > 1)
	{
		node x;
		x.occure = letters_count_copy[letters_count_copy.size()-1]->occure+ letters_count_copy[letters_count_copy.size() - 2]->occure;
		x.left= letters_count_copy[letters_count_copy.size() - 1];
		letters_count_copy[letters_count_copy.size() - 1]->perant = &x;
		letters_count_copy[letters_count_copy.size() - 1]->direction = 0;
		x.right = letters_count_copy[letters_count_copy.size() - 2];
		letters_count_copy[letters_count_copy.size() - 2]->perant = &x;
		letters_count_copy[letters_count_copy.size() - 2]->direction = 1;
		letters_count_copy.pop_back();
		letters_count_copy.pop_back();
		letters_count_copy.push_back(&x);
		sort(letters_count_copy.begin(), letters_count_copy.end(), compareBycount);
	}
	hoffman.head = letters_count_copy[0];
	vector<vector <char>> encoding_list;
	for(int i=0;i<128;i++)
	{
		vector <char> y;
		y.push_back(-1);
		encoding_list.push_back(y);
	}
	for (int i = 0; i < letters_count_copy.size(); i++)
	{
		encoding_list[letters_count_copy[i]->data].pop_back();
		node* c=letters_count_copy[i];
		while (c->perant != hoffman.head)
		{
			encoding_list[letters_count_copy[i]->data].push_back(c->direction);
			c = c->perant;
		}
		encoding_list[letters_count_copy[i]->data].push_back(c->direction);
	}
	vector<char> bitss;
	for (int i = 0; i < encoding_list.size(); i++)
	{
		for (int j = 0; j < encoding_list[i].size(); j++)
		{
			bitss.push_back(encoding_list[i][j]);
		}
	}
	while (bitss.size() > 8)
	{
		char result=0;
		for (int i = 0; i < 8; ++i)
		{
			result |= (bitss[i] == '1') << (7 - i);
		}
		output.push_back(result);
		for (int i = 0; i < 8; ++i)
		{
			bitss.erase(bitss.begin());
		}
	}
	char result = 0;
	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < bitss.size(); j++, i++)
		{
			result |= (bitss[i] == '1') << (7 - i);
		}
		result |= (bitss[i] == '1') << (7 - i);
	}
	output.push_back(result);
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