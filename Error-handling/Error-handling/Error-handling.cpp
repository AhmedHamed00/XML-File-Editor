#include <fstream>
#include <iostream>
#include "error_handling.h"
using namespace std;
int main()
{
    uint8_t x;
    if(find_errors("C:/Users/George/Downloads/sample.xml", x, 1))cout<<"Formatting fault"<<endl;
    cout << "Number of errors : " << error_list.size() << endl;
    for (auto a : error_list)
    {
        cout << a << endl;
    }
    solve_missingClose();
    writeFile();
    return 0;
}

