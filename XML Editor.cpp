
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
   // cout << testchars.size() << endl;
   minify(testchars);
   // compress(testchars);
   vector<char> copy = testchars;
   tree x;
   compress_hoffman_encoding(testchars, copy, x);
   // cout<<testchars.size()<<endl;
    for (unsigned int i = 0; i < testchars.size(); i++)
    {
        cout << copy[i];
    }


    //printf("%c", 129);
}