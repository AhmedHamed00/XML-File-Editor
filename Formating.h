#ifndef Formating
#define Formating

#include <vector>
#include <algorithm>
using namespace std;
typedef struct node {
    char valid_data = 0;
    char data=-1;
    long long occure = 0;
    node* perant = 0;
    char direction;
    node* left=0;
    node* right = 0;
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
void minify(vector<char>& input);

/*
 * Description :
 * Functional responsible for comoressing the XML filr:
 * Inputs:
 *		  input-> the xml file read into a char vector
 */
void compress(vector<char>& input);
void save_compressed_file(vector<char>& input, vector<char [2]> key, vector<vector<int> > index);
void compress_hoffman_encoding(vector<char>& input, vector<char>& output, tree& hoffman);
void decompress(vector<char>& input, vector<char>& output, tree& hoffman);


#endif /* Formating */