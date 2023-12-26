//karim Elnekheily
//code:2000318
#include <iostream>
#include "XmlFile2json.h"



// Function to check equivalence of two nodes based on their names

bool check_equivalence(Node node1, Node node2) {
    // Check if the size of the name of the first node is less than or equal to the size of the name of the second node

    if (node1.name.size() <= node2.name.size()) {
        // If true, nodes are not equivalent

        return false;
    }
    else {
        // Iterate through the characters of the name of the second node

        for (int i = 0; i < node2.name.size() - 1; i++) {
            // If characters at the same position are not equal, nodes are not equivalent

            if (node1.name[i] != node2.name[i]) {
                return false;
            }
        }
        // If all characters are equal up to the size of the second node's name, nodes are considered equivalent

        return true;
    }

}
// Function to trim leading and trailing whitespaces (spaces, tabs, and newlines) from a given string

string trim(const string& str) {
    // Find the position of the first non-whitespace character in the string

    size_t start = str.find_first_not_of(" \t\r\n");
    // Find the position of the last non-whitespace character in the string

    size_t end = str.find_last_not_of(" \t\r\n");
    // Check if either the start or end position is not found, indicating an empty or all-whitespace string

    if (start == string::npos || end == string::npos) {
        // Return an empty string if the input string is empty or contains only whitespaces

        return "";
    }
    // Extract the substring between the first and last non-whitespace characters (inclusive)

    return str.substr(start, end - start + 1);
}
// function for removing spaces
void remove_spaces(string filename) {
    string first_line = "";
    string Second_line;
    string buffer = "";

    // Open input file for reading
    ifstream file_without_spaces(filename);

    // Open output file for writing processed lines
    ofstream outfile("sample3.txt");

    // Flag to track whether the current character between xml tag or not
    bool tag = false;

    // Read each line from the file
    while (getline(file_without_spaces, first_line)) {
        // Trim leading and trailing whitespaces from the line


        // Iterate through each character in the trimmed line
        for (int i = 0; i < first_line.length(); i++) {
            // Trim leading and trailing whitespaces from the line
            first_line = trim(first_line);
            // Exclude newline, carriage return, tab, and NULL characters
            if (first_line[i] != '\n' && first_line[i] != '\r' && first_line[i] != '\t' && first_line[i] != NULL) {
                // Concatenate non-excluded characters to second line
                Second_line += first_line[i];
            }
        }
    }

    // Process each character in the concatenated line2
    for (size_t i = 0; i < Second_line.length(); i++) {
        // Check if the current character is the opening tag '<'
        if (Second_line[i] == '<') {
            // If buffer contains content, write it to the output file
            if (buffer.length() != 0) {
                outfile << buffer << endl;
                buffer = "";
            }

            // Add the opening tag to the buffer and set inside_tag flag to true
            buffer += Second_line[i];
            tag = true;
        }
        else {
            // If not inside a tag, add the character to the buffer
            buffer += Second_line[i];
        }

        // Check if the current character is the closing tag '>'
        if (Second_line[i] == '>') {
            // Set inside_tag flag to false
            tag = false;

            // Write the buffer (containing the tag) to the output file
            outfile << buffer << endl;

            // Clear the buffer for the next iteration
            buffer = "";
        }
    }
}

// Recursive function to serialize a tree structure into a JSON-like format and write it to an output file
void writeTreeToJSONFile(ofstream& outfile, Node& node, int count = 0, bool equal = false, bool flag = false) {
    // Flag to check if the current call is the initial call (root node)
    bool h = false;

    // Check if the current call is the initial call (root node)
    if (flag == false) {
        // Recursively call the function for each child node
        for (Node child : node.children) {
            writeTreeToJSONFile(outfile, child, count + 1, equal, true);
        }

        // End the function for the initial call
        return;
    }

    // Check if the current node has children
    if (node.children.size() > 0) {
        // Indentation for nested levels
        for (int i = 0; i < count; i++) {
            outfile << "    ";
        }

        // Check if the node's children are equivalent
        if (equal) {
            // If equivalent, continue indentation
        }
        else {
            // If not equivalent, write the node's name and a colon
            outfile << char(34) << node.name << char(34) << ": ";
            h = true;
        }

        // Check if all children nodes are equivalent
        bool k = true;
        for (Node child : node.children) {
            k = check_equivalence(node, child);
            if (k == false) {
                break;
            }
        }

        // Check if not all children nodes are equivalent
        if (k == false) {
            // Increase the indentation level, write an opening curly brace, and process each child
            count++;
            outfile << "{" << endl;
            for (int i = 0; i < node.children.size(); i++) {
                writeTreeToJSONFile(outfile, node.children[i], count, k, flag);
                if (i != node.children.size() - 1) {
                    outfile << "," << endl;
                }
                else {
                    outfile << endl;
                }
            }

            // End the block with closing curly brace and decrease the indentation level
            for (int i = 0; i < count - 1; i++) {
                outfile << "    ";
            }
            outfile << "}";
            count--;
        }
        else {
            // If all children nodes are equivalent, write an opening square bracket and process each child
            count++;
            outfile << "[" << endl;
            for (int i = 0; i < node.children.size(); i++) {
                writeTreeToJSONFile(outfile, node.children[i], count, k, flag);
                if (i != node.children.size() - 1) {
                    outfile << "," << endl;
                }
                else {
                    outfile << endl;
                }
            }

            // End the block with closing square bracket and decrease the indentation level
            for (int i = 0; i < count - 1; i++) {
                outfile << "    ";
            }
            outfile << "]";
            count--;
        }
    }
    else {
        // If the node has no children, write the node's ID or name: ID pair
        if (equal) {
            // If equivalent, write only the ID
            for (int i = 0; i < count; i++) {
                outfile << "    ";
            }
            outfile << char(34) << node.Id << char(34);
        }
        else {
            // If not equivalent, write the node's name, a colon, and the ID
            for (int i = 0; i < count; i++) {
                outfile << "    ";
            }
            outfile << char(34) << node.name << char(34) << ": " << char(34) << node.Id << char(34);
        }
    }
}
void  xmlFileToJson(string Path)
{
    // Remove spaces from the input path
    remove_spaces(Path);

    // Open the file with the provided path
    ifstream file("sample3.txt");

    // Stack to keep track of XML nodes
    stack<Node> node_stack;

    // Read the file line by line
    string line;
    Node imaginary_firstnode;
    node_stack.push(imaginary_firstnode);
    int cnt_line_number = 0;

    while (getline(file, line)) {
        // Trim leading and trailing whitespaces from the line
        line = trim(line);

        // Skip empty lines
        if (line.empty()) {
            continue;
        }

        cnt_line_number++;

        if (line.find("<") != string::npos) {
            // Extract the tag from the line
            string tag = line.substr(line.find("<") + 1, line.find(">") - line.find("<") - 1);

            if (tag[0] == '/') {
                // Closing tag encountered

                // Ensure there is at least one node in the stack
                if (node_stack.size() < 2) {
                    return; // Error: mismatched closing tag
                }

                // Get the child node and remove it from the stack
                Node child = node_stack.top();
                if (child.name != tag.substr(1)) {
                    return; // Error: mismatched closing tag
                }
                node_stack.pop();

                // Get the parent node from the stack
                Node& parent = node_stack.top();

                // Add the child node to the parent's children
                parent.children.push_back(child);
            }
            else {
                // Opening tag encountered

                // Create a new node and push it onto the stack
                Node node;
                node.name = tag;
                node_stack.push(node);
            }
        }
        else {
            // Text content encountered

            // Ensure there is at least one node in the stack
            if (node_stack.size() < 2) {
                // Do nothing or handle the case as needed
            }

            // Get the top node from the stack and append the text content
            Node& node = node_stack.top();
            node.Id += line;
        }
    }

    // Ensure that all opening tags have corresponding closing tags
    if (node_stack.size() > 1) {
        return; // Error: mismatched opening tag(s)
    }

    // Open the output file for writing JSON
    // an example to test
    ofstream outputfile("C:/Users/karim/OneDrive/Documents/XmltoJson/XmlFile2json/sample.json");
    //ofstream outputjson( path_json);
    outputfile << '{' << endl;

    // Write the JSON representation of the XML tree to the output file
    writeTreeToJSONFile(outputfile, node_stack.top());

    outputfile << endl << '}';

}


int main(void) {

    // to test this code
    xmlFileToJson("C:/Users/karim/OneDrive/Documents/XmltoJson/XmlFile2json/Sample.xml");


    return 0;
}
