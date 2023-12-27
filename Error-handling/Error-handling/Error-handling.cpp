#include <fstream>
#include <iostream>
#include "error_handling.h"
using namespace std;
int main()
{
    do
    {
        if (find_errors("D:/ASU/7/DS/Project notes/sample.xml", 1))cout << "Formatting fault" << endl;
        cout << "Number of errors : " << error_list.size() << endl;
        for (auto a : error_list)
        {
            cout << a << endl;
        }
        solve_errors();
    } while (error_list.size());
    return 0;
}

