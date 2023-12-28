#include "Formating.h"
#include <iostream>
#define GET_BIT(REG,BIT) ( ( REG & (1<<BIT) ) >> BIT )
/*
 * Description :
 * Functional responsible for Minifying(removing all excess chars exept for body) the XML filr:
 * Inputs:
 *		  input-> the xml file read into a char vector
 */
void _Minify(vector<char>& input)
{
	for (unsigned int i = 0; i < input.size(); i++)
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
/*
 * Description :
 * Functional responsible for comparing two nodes by count:
 * Inputs:
 *		  a: first node
 *		  b: second node
 */
bool compareBycount(node* a,node* b)
{
	return a->occure > b->occure;
}
/*
 * Description :
 * Functional responsible for compressing a file using hoffman code and saving the file into two files (the compressed data and decoding tree)
 *	with total space less than original file:
 * Inputs:
 *		  input: the name of the file including bath
 */
long long Compress_Using_Hoffman_Coding(string input_name)
{
	ifstream test;
	test.open(input_name, std::ios_base::in | std::ios::binary);
	vector<char> input;
	char ch;
	while (test.get(ch)) {
		input.push_back(ch);
	}
	test.close();
	_Minify(input);
	vector<char> output;
	tree hoffman;
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
			letters_nodes[letters_nodes.size() - 1]->valid_data = 2;
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
			encoding_list[letters_nodes_copy[i]->data].insert(encoding_list[letters_nodes_copy[i]->data].begin(),c->direction);
			c = c->perant;
		}
		encoding_list[letters_nodes_copy[i]->data].insert(encoding_list[letters_nodes_copy[i]->data].begin(), c->direction);
	}
	vector<char> bitss;
	
	for (long long i = 0; i < input.size(); i++)
	{
		
		for (int j = 0; j < encoding_list[input[i]].size(); j++)
		{
			bitss.push_back(encoding_list[input[i]][j]);
		}
	
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
	char result = 0;
	
		for (int j = 0; j < bitss.size(); j++)
		{
			result |= (bitss[j] ) << (7 - j);
		}
		
	output.push_back(result);
	output.push_back(8-bitss.size());
	vector<char> saved_tree;
	for (int i = 0; i < numbre_of_nodes; i++)
	{
		saved_tree.push_back(letters_nodes_copy[i]->data);
		if (letters_nodes_copy[i]->occure<255)
		{
			saved_tree.push_back(1);
			saved_tree.push_back(letters_nodes_copy[i]->occure);
		}
		else if (letters_nodes_copy[i]->occure < 25563)
		{
			saved_tree.push_back(2);
			saved_tree.push_back(letters_nodes_copy[i]->occure&0x0000FFFF );
			saved_tree.push_back(letters_nodes_copy[i]->occure>>8);

		}
		else if (letters_nodes_copy[i]->occure < 255)
		{
			saved_tree.push_back(4);
			saved_tree.push_back((letters_nodes_copy[i]->occure & 0x000000000000FFFF));
			saved_tree.push_back((letters_nodes_copy[i]->occure & 0x00000000FFFF0000 )>> 8);
			saved_tree.push_back((letters_nodes_copy[i]->occure & 0x0000FFFF00000000)>>16);
			saved_tree.push_back(letters_nodes_copy[i]->occure >> 24);

		}
		
	}
	Save_Hoffman_Tree_data(saved_tree,output, "Hoffman_Tree.txt","Hoffman_Coded_Output.txt");
	for (int i=0;i< letters_nodes_copy.size();i++)
	{
		free(letters_nodes_copy[i]);
	}
	return saved_tree.size() + output.size();
}
/*
 * Description :
 * Functional responsible for decompressing a file using hoffman code 
 * Inputs:
 *		  output: the name of the file including bath
 *		  tree_file_name: the file incloding hoffman tree
 *		  coded_text: the text compressed by hoffman code
 */
void Decompress_Hoffman_Coding( string outt,string tree_file_name,string coded_text)
{
	vector<char> output;
	ifstream test;
	test.open(coded_text, std::ios_base::in | std::ios::binary);
	vector<char> input;
	char ch;
	while (test.get(ch)) {
		input.push_back(ch);
	}
	test.close();
	ifstream test2;
	test2.open(tree_file_name, std::ios_base::in | std::ios::binary);
	vector<char> tree_input;
	while (test2.get(ch)) {
		tree_input.push_back(ch);
	}
	test2.close();
	vector<node*> letters_nodes;
	vector<node*> letters_nodes_copy;
	for(int i=0;i<tree_input.size();i++)
	{
		letters_nodes.push_back((node*)malloc(sizeof(node)));
		letters_nodes_copy.push_back(letters_nodes[letters_nodes.size() - 1]);
		letters_nodes[letters_nodes.size() - 1]->data = tree_input[i];
		letters_nodes[letters_nodes.size() - 1]->valid_data = 2;
		long long temp=0;
		if (tree_input[i + 1] == 1)
		{
			temp |= (unsigned char)tree_input[i + 2];
			i += 2;
		}
		else if (tree_input[i + 1] == 2)
		{
			temp |= (unsigned char)tree_input[i + 2];
			temp |= (unsigned char)tree_input[i + 3]<<8;
			i += 3;
		}
		else if (tree_input[i+1]==4)
		{
			temp |= (unsigned char)tree_input[i + 2];
			temp |= (unsigned char)tree_input[i + 3] << 8;
			temp |= (unsigned char)tree_input[i + 4]<<16;
			temp |= (unsigned char)tree_input[i + 5] <<24;
			i += 5;
		}
		letters_nodes[letters_nodes.size() - 1]->occure = temp;
	}
	int numbre_of_nodes = letters_nodes.size();
	sort(letters_nodes.begin(), letters_nodes.end(), compareBycount);
	while (letters_nodes.size() > 1)
	{
		node* x = (node*)malloc(sizeof(node));
		x->data = -1;
		x->occure = letters_nodes[letters_nodes.size() - 1]->occure + letters_nodes[letters_nodes.size() - 2]->occure;
		x->left = letters_nodes[letters_nodes.size() - 1];
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
	vector<char> x;
	for (int i = 0; i < input.size()-1; i++)
	{
		for (int j = 7; j >=0; j--)
		{
			x.insert(x.end(),GET_BIT(input[i], j));
		}
	}
	node* z =letters_nodes[0];
	for (long long i = 0; i <= x.size()- input[input.size() - 1]; i++)
	{
		if (z->data < 0)
		{
			if (x[i] == 0)
			{
				if(z->left!= nullptr)
				z = z->left;
			}
			else
			{
				if (z->left != nullptr)
				z = z->right;
			}
		}
		else if(z->valid_data==2)
		{
			if (z->left != 0)
			output.insert(output.end(),z->data);
			z = letters_nodes[0];
			i--;
		}
	}
	ofstream test1;
	test1.open(outt, std::ios_base::out | std::ios::binary);
	for (int i = 0; i < output.size(); i++)
	{
		test1 << output[i];
	}
	test1.close();
}
/*
 * Description :
 * Functional responsible for save hoffman coded file and hoffmantreetree
 * Inputs:
 *		  input_tree: the hoffman tree data
 *		  input_data: the hofman coded data
 *		  hoffman_tree_file: hoffman tree file name
 *		  hoffman_coded_file: coded data file name
 */
void Save_Hoffman_Tree_data(vector<char>& input_tree, vector<char>& input_data,string hoffman_tree_file, string hoffman_coded_fle)
{
	ofstream Hoffman_Coding_File;
	Hoffman_Coding_File.open(hoffman_coded_fle, std::ios_base::out | std::ios::binary);
	for (int i = 0; i < input_data.size(); i++)
	{
		Hoffman_Coding_File << input_data[i];
	}
	Hoffman_Coding_File.close();
	ofstream Hoffman_Tree_File;
	Hoffman_Tree_File.open(hoffman_tree_file, std::ios_base::out | std::ios::binary);
	for (int i =0;i< input_tree.size();i++)
	{
		Hoffman_Tree_File << input_tree[i];
	}
	Hoffman_Tree_File.close();
}