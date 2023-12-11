
//This main function is provided for testing by Ahmed Hamed 
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <vector>
#include <string>
#include "Minify.h"
using namespace std;
int main()
{
    ifstream test;
    test.open("sample.xml");
    vector<char> testchars;
    char ch;
    while (test.get(ch)) {

        testchars.push_back(ch);
    }
    minify(testchars);
    for (int i = 0; i < testchars.size(); i++)
    {
        cout << testchars[i];
    }
}