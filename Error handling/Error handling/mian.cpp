#include <fstream>
#include <iostream>
#include "error_handling.h"
using namespace std;
int main()
{
    uint8_t x;
    vector<xml_error> test = find_errors("D:/ASU/7/DS/Project notes/New_Text_Document.xml", x, 0);
    for (auto a : test)
    {
        cout << a << endl;
    }
    return 0;
}
