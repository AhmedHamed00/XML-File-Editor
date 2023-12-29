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

void _Minify(string input_name);
void Save_Hoffman_Tree_data(vector<char>& input_tree, vector<char>& input_data, string hoffman_tree_file, string hoffman_coded_fle);
long long Compress_Using_Hoffman_Coding(string input_name, string Hoffman_Tree, string Hoffman_Coded_Output);
void Decompress_Hoffman_Coding(string outt, string tree_file_name, string coded_text);


#endif /* Formating */