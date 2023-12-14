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
	M_OPEN, M_CLOSE, M_BRACKET, M_SLASH, UNKNOWN_TAG, NESTING_ERROR, MISMATCH_TAG, UNKNOWN_ERROR, FILE_NOT_OPEN, NO_ROOT_TAG
};

enum class ERROR_MAIN_TYPE :uint8_t
{
	LOGICAL, SYNTAX
};

enum class ERROR_TAG :uint8_t
{
	M_OPEN_OR_MISMATCH_OPEN, MISMATCH_OPEN_OR_UNKNOWN, ROOT_ERROR, NO_ERROR
};

class xml_error
{
public:
	ERROR_TYPE error_type;
	ERROR_MAIN_TYPE main_type;
	int line;
	bool solvable;

	xml_error(const ERROR_TYPE& error_type, const ERROR_MAIN_TYPE& main_type, int line, bool solvable)
		: error_type(error_type), main_type(main_type), line(line), solvable(solvable)
	{
	}
	friend ostream& operator << (ostream& out, const xml_error& c);
};

class xml_base_tag
{
public:
	string tag_name;
	bool optional;
	bool unique;
	vector<xml_base_tag> nestting_list;

	xml_base_tag() = default;

	xml_base_tag(const string& tag_name, bool optional, bool unique, vector<xml_base_tag> nestting_list = static_cast<vector<xml_base_tag>>(NULL))
		:tag_name(tag_name), optional(optional), unique(unique), nestting_list(nestting_list)
	{
	}

};
class xml_tag
{
public:
	bool open = true;
	ERROR_TAG error = ERROR_TAG::NO_ERROR;
	vector<xml_base_tag>* excp_type_list = nullptr;
	xml_base_tag* type;
	int line;
	bool in_order;

	xml_tag() = default;

	xml_tag(xml_base_tag* type, int line)
	{
		this->type = type;
		this->line = line;
	}
	string get_type_name()
	{
		return type->tag_name;
	}
	bool in_nestting_set_of(vector<xml_base_tag>* type_vector)
	{
		vector<xml_base_tag> type_vector_ = *type_vector;
		for (auto type : type_vector_)
		{
			if (this->type->tag_name == type.tag_name)
				return true;
		}
		return false;
	}
};


vector<xml_error> find_errors(string file_path, uint8_t& success, int check_flag);