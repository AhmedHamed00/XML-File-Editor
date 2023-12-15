#include "Formating.h"
#include <iostream>
#define GET_BIT(REG,BIT) ( ( REG & (1<<BIT) ) >> BIT )
/*
 * Description :
 * Functional responsible for Minifying the XML filr:
 * Inputs:
 *		  input-> the xml file read into a char vector
 */

void minify(vector<char>& input)
{
	for (unsigned int i = 0; i < input.size()-9; i++)
	{
		if(i< (input.size()-5) && input[i] =='<' && input[i+1] == 'b' && input[i+2] == 'o' && input[i+3] == 'd' && input[i+4] == 'y')
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
	vector<node *> letters_nodes;
	vector<node *> letters_nodes_copy;
	for (int j = 0; j < 127; j++)
	{
		long long count = 0;
		for (long long i = 0; i < input.size(); i++)
		{
			if (input[i] == j)
			{
				count++;
			}
		}
		if (count != 0)
		{
			letters_nodes.push_back((node*)malloc(sizeof(node)));
			letters_nodes_copy.push_back(letters_nodes[letters_nodes.size() - 1]);
			letters_nodes[letters_nodes.size() - 1]->data = j;
			letters_nodes[letters_nodes.size() - 1]->valid_data = 1;
			letters_nodes[letters_nodes.size() - 1]->occure = count;
		}
	}
	int numbre_of_nodes = letters_nodes.size();
	sort(letters_nodes.begin(), letters_nodes.end(), compareBycount);
	while (letters_nodes.size() > 1)
	{
		node* x= (node*)malloc(sizeof(node));
		x->data = -1;
		x->occure = letters_nodes[letters_nodes.size()-1]->occure+ letters_nodes[letters_nodes.size() - 2]->occure;
		x->left= letters_nodes[letters_nodes.size() - 1];
		letters_nodes[letters_nodes.size() - 1]->perant = x;
		letters_nodes[letters_nodes.size() - 1]->direction = 0;
		x->right = letters_nodes[letters_nodes.size() - 2];
		letters_nodes[letters_nodes.size() - 2]->perant = x;
		letters_nodes[letters_nodes.size() - 2]->direction = 1;
		letters_nodes.pop_back();
		letters_nodes.pop_back();
		letters_nodes.push_back(x);
		letters_nodes_copy.push_back(x);
		sort(letters_nodes.begin(), letters_nodes.end(), compareBycount);
	}
	hoffman.head = letters_nodes[0];
	vector<vector <char>> encoding_list;
	for(int i=0;i<128;i++)
	{
		vector <char> y;
		y.push_back(-1);
		encoding_list.push_back(y);
	}
	for (int i = 0; i < numbre_of_nodes; i++)
	{
		encoding_list[letters_nodes_copy[i]->data].clear();
		node* c=letters_nodes_copy[i];
		while (c->perant != hoffman.head)
		{
			encoding_list[letters_nodes_copy[i]->data].push_back(c->direction);
			c = c->perant;
		}
		encoding_list[letters_nodes_copy[i]->data].push_back(c->direction);
	}
	vector<char> bitss;
	
	for (long long i = 0; i < input.size(); i++)
	{
		
		for (int j = 0; j < encoding_list[input[i]].size(); j++)
		{
			bitss.push_back(encoding_list[input[i]][j]);
		}
		while (bitss.size() >= 8)
		{
			char result = 0;
			for (int i = 0; i < 8; ++i)
			{
				result |= (bitss[i]) << (7 - i);
			}
			output.push_back(result);
			for (int i = 0; i < 8; ++i)
			{
				bitss.erase(bitss.begin());
			}
		}
	}
	
	char result = 0;
	
		for (int j = 0; j < bitss.size(); j++)
		{
			result |= (bitss[j] ) << (7 - j);
		}
		
	output.push_back(result);
}
void decompress(vector<char>& input, vector<char>& output, tree& hoffman)
{
	vector<char> x;
	for (int i = 0; i < input.size(); i++)
	{
		for (int j = 7; j >=0; j--)
		{
			x.insert(x.begin(),GET_BIT(input[i], j));
		}
	}
	node* z = hoffman.head;
	for (long long i = 0; i < x.size(); i++)
	{
		if (z->data < 0)
		{
			if (x[i] == 0)
			{
				z = z->left;
			}
			else
			{
				z = z->right;
			}
		}
		else if(z->valid_data==1)
		{
			output.insert(output.begin(),z->data);
			z = hoffman.head;
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