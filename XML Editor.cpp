
//This main function is provided for testing by Ahmed Hamed 
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <vector>
#include <string>
#include "Formating.h"
using namespace std;
int main()
{
    _Minify("sample.xml");
   cout<< Compress_Using_Hoffman_Coding("sample.xml", "Hoffman_Tree.txt", "Hoffman_Coded_Output.txt");
   vector<char> copy ;
    Decompress_Hoffman_Coding( "sdaf45", "Hoffman_Tree.txt","Hoffman_Coded_Output.txt");
    for ( int i = 0; i < copy.size(); i++)
    {
        printf("%c", copy[i]);
    }
 
}