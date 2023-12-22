#ifndef Formating
#define Formating

#include <vector>
#include <algorithm>
#include <iostream>
#include <fstream>
using namespace std;
typedef struct node {
    char valid_data = 0;
    char data=-1;
    long long occure = 0;
    node* perant = 0;
    char direction;
    node* left=NULL;
    node* right = NULL;
} node;
typedef struct tree {
    node* head;
} tree;
/*
 * Description :
 * Functional responsible for Minifying the XML filr:
 * Inputs:
 *		  input-> the xml file read into a char vector
 */
void _Minify(vector<char>& input);
void Save_Hoffman_Tree(vector<char>& input, vector<char>& input2, string name, string name2);
void Compress_Using_Hoffman_Coding(vector<char>& input, vector<char>& output, tree& hoffman);
void Decompress_Hoffman_Coding(vector<char>& output, string tree_file_name, string coded_text);


#endif /* Formating */