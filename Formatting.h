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

void _Minify(std::vector<char>& input);
void Save_Hoffman_Tree_data(std::vector<char>& input_tree, std::vector<char>& input_data, std::string hoffman_tree_file, std::string hoffman_coded_fle);
long long Compress_Using_Hoffman_Coding(std::string input_name, std::string Hoffman_Tree,std::string Hoffman_Coded_Output);
void Decompress_Hoffman_Coding(std::string outt, std::string tree_file_name, std::string coded_text);


#endif /* Formating */
