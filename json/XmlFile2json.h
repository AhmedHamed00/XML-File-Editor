#ifndef XmlFile2json
#define XmlFile2json

using namespace std;
#include <iostream>
#include <fstream>
#include <string>
#include <stack>
#include <vector>
#include <queue>

struct Node {

    //initialization
    // Node structure representing a tree node with a name, an ID, and a vector of children nodes

    string name = "";

    string Id = "";
    vector<Node> children;
};
bool check_equivalence(Node node1, Node node2);
string trim(const string& str);
void remove_spaces(string filename);
void writeTreeToJSONFile(ofstream& outfile, Node& node, int count, bool equal, bool flag);
void xmlFileToJson(string Path);
#endif