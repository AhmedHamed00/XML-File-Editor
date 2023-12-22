
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
    ifstream test;
    test.open("sample.xml");
    vector<char> testchars;// {'a', 'a', 'b', 'c', 'f', 'a', 'a', 'b', 'f', 'g'};
    char ch;
    while (test.get(ch)) {

        testchars.push_back(ch);
    }
    cout << endl << testchars.size() << endl;
   //_Minify(testchars);
   // compress(testchars);
   testchars.push_back('\n');
   for (unsigned int i = 0; i < testchars.size(); i++)
   {
       cout << testchars[i];
   }
   test.close();
   vector<char> copy ;
   vector<char> copy2;
   tree x;
   Compress_Using_Hoffman_Coding(testchars, copy, x);
   // cout<<testchars.size()<<endl;
   for (unsigned int i = 0; i < copy.size(); i++)
   {
       cout << copy[i];
   }
   cout << endl << copy.size() << endl;
    cout << endl;
    
    Decompress_Hoffman_Coding( copy2, "Hoffman_Tree.txt","Hoffman_Coded_Output.txt");
    for (unsigned int i = 0; i < copy2.size(); i++)
    {
        cout << copy2[i];
    }
    cout << endl<<testchars.size() << endl;
    cout << copy2.size() << endl;
    //printf("%c", 129);
}