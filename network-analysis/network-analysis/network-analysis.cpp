#include <iostream>
#include <string>
#include "network.h"
using namespace std;
int main()
{
	string path = "D:/ASU/7/DS/Project notes/sample.xml";
	networking_analysis _network(path);
	cout << _network;
}

