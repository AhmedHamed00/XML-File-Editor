
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
    Compress_Using_Hoffman_Coding("sample.xml");
   vector<char> copy ;
    Decompress_Hoffman_Coding( copy, "Hoffman_Tree.txt","Hoffman_Coded_Output.txt");
    for ( int i = 0; i < copy.size(); i++)
    {
        printf("%c", copy[i]);
    }
    //ofstream test;
    //test.open("i want to see data.txt", std::ios_base::out | std::ios::binary);
    //for (int i = 0; i < copy.size(); i++)
    //{
    //    test << copy[i];
    //}
    //test.close();
    //cout <<endl<<copy.size();
    //printf("%c", 129);
}