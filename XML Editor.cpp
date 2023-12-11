
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <vector>
#include <string>
using namespace std;
int main()
{
    cout << "please enter file name \n";
    string file_name;
    getline(cin, file_name);
    ifstream test;
    test.open(file_name);
    vector<char> testchars;
    char ch;
    while (test.get(ch)) {

        testchars.push_back(ch);
    }
    for (int i = 0; i < testchars.size(); i++)
    {
        cout << testchars[i];
    }
}