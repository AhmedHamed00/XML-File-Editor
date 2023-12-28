#pragma once
#include <fstream>
#include <vector>
#include <string>
#include <regex>
#include <iostream>
#include <stack>
using namespace std;


enum class ERROR_TYPE :uint8_t
{
	M_OPEN, M_CLOSE, M_BRACKET, UNKNOWN_TAG, MISMATCH_TAG, UNKNOWN_ERROR, FILE_NOT_OPEN
};

enum class ERROR_MAIN_TYPE :uint8_t
{
	LOGICAL, SYNTAX
};
enum class TAG_TYPE :uint8_t
{
	OPENING_TAG, CLOSING_TAG, M_OPEN_BRACKET, M_CLOSE_BRACKET
};

class xml_error
{
public:
	ERROR_TYPE error_type;
	ERROR_MAIN_TYPE main_type;
	bool solvable;
	string msg;
	int line;

	xml_error(const ERROR_TYPE& error_type, const ERROR_MAIN_TYPE& main_type, bool solvable, int line, string msg)
		: error_type(error_type), main_type(main_type), line(line), solvable(solvable), msg(msg)
	{
	}
	friend ostream& operator << (ostream& out, const xml_error& c);
	xml_error() = default;
};

class xml_tag
{
public:
	string tag_name;
	TAG_TYPE type;
	int line;
	bool found;
	xml_tag(const string& tag_name, const TAG_TYPE& type, int line, bool found)
		: tag_name(tag_name), type(type), line(line), found(found)
	{
	}
	xml_tag() = default;
};
class xml_underCorrection_tag
{
public:
	string tag_name;
	TAG_TYPE type;
	int line;
	int priority;


	xml_underCorrection_tag(const string& tag_name, const TAG_TYPE& type, int line)
		: tag_name(tag_name), type(type), line(line)
	{
		if (tag_name == "id" || tag_name == "body" || tag_name == "topic" \
			|| tag_name == "name")this->priority = 0;
		if (tag_name == "follower" || tag_name == "topics")this->priority = 1;
		if (tag_name == "followers" || tag_name == "post")this->priority = 2;
		if (tag_name == "posts")this->priority = 3;
		if (tag_name == "user")this->priority = 4;
		if (tag_name == "users")this->priority = 5;
	}
};

extern vector<xml_error> error_list;

//this flag(return value) means that the errors are found but cant be corrected as the file formatting is hard to correct
uint8_t find_errors(string file_path, uint8_t& success, bool bracket_flag, bool spell_flag);

/****************************************************************************************************************************
*                                                   error correction
*****************************************************************************************************************************/

bool solve_errors();