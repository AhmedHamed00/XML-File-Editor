#include <fstream>
#include <iostream>
#include "error_handling.h"
using namespace std;
int main()
{
    uint8_t x;
    if(find_errors("D:/ASU/7/DS/Project notes/sample.xml", x, 0))cout<<"Formatting fault"<<endl;
    cout << "Number of errors : " << error_list.size() << endl;
    for (auto a : error_list)
    {
        cout << a << endl;
    }
    fstream file("D:/ASU/7/DS/Project notes/sample.xml");
    solve_missingBracket(file);
    file.close();
    return 0;
}

