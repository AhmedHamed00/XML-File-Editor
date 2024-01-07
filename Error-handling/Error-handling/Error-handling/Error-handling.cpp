#include <fstream>
#include <iostream>
#include "error_handling.h"
using namespace std;
int main()
{
    bool check_bracket = true;
    bool check_spelling = true;
    uint8_t x = 1;
    if(find_errors("C:/Users/George/Downloads/sample.xml",x ,check_bracket,check_spelling))cout<<"Formatting fault"<<endl;
    cout << "Number of errors : " << error_list.size() << endl;
    for (auto a : error_list)
    {
        cout << a << endl;
    }
    solve_errors();
    return 0;
}

