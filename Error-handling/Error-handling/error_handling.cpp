#include "error_handling.h"
using namespace std;
vector<xml_error> g_error_list;

static bool find_bracket_error(ifstream& file, vector<xml_error>& error_list)
{
	string parsed;
	smatch s;
	regex search("(<\/?[a-z]+)(?![a-z]*>)|([ \n\t]\/?[a-z1-9]+>)|(>[a-z1-9 ]+\/[a-z]+>)");
	int line = 0;
	while (file)
	{
		vector<string> m;
		getline(file, parsed);
		while (regex_search(parsed, s, search))
		{
			m.push_back(static_cast<string>(s[0]));
			parsed = static_cast<string>(s.suffix());
		}
		for(string x:m)
		{
			string err = "Missing Tag bracket : " + x;
			error_list.push_back(xml_error(ERROR_TYPE::M_BRACKET, ERROR_MAIN_TYPE::SYNTAX, true, line,err));
		}
		line++;
	}
	file.seekg(0, ios::beg);
	return error_list.size() ? true : false;
}

static vector<string> get_tags(istream& file)
{
	smatch s;
	regex tag_finder("<\/?[a-z]+>");
	string search_text;
	getline(file, search_text);
	vector<string> m;
	while (regex_search(search_text, s, tag_finder))
	{
		m.push_back(static_cast<string>(s[0]));
		search_text = static_cast<string>(s.suffix());
	}
	return m;
}

void find_errors(string file_path, uint8_t& success, int check_flag)
{
	ifstream file(file_path);
	vector<xml_error> error_list;
	if (!file)
	{
		string err = "Problem openning file: " + file_path;
		error_list.push_back(xml_error(ERROR_TYPE::FILE_NOT_OPEN,ERROR_MAIN_TYPE::LOGICAL,false,0,err));
		g_error_list = error_list;
		success = 0;
		return;
	}
	if (check_flag)
	{
		if (find_bracket_error(file, error_list))
		{
			success = 0;
			return;
		}
	}
	int line = 0;
	stack<xml_tag> tags;
	vector<string> string_tags;
	pair<xml_tag, xml_tag> tag_pair;
	vector<pair<xml_tag, xml_tag>> mismatch;
	vector<pair<xml_tag, xml_tag>> missing_opening;
	vector<pair<xml_tag, xml_tag>> missing_closing;
	xml_tag current_tag;
	while (file)
	{
		string_tags = get_tags(file);
		for (string string_tag : string_tags)
		{
			//get tag type and create a tag object
			if (string_tag[1] == '/')
				current_tag = xml_tag(string_tag.substr(2, string_tag.length()), TAG_TYPE::CLOSING_TAG, line, true);
			else
				current_tag = xml_tag(string_tag.substr(2, string_tag.length()), TAG_TYPE::OPENING_TAG, line, true);

			//check if it is a closing tag or openning tag
			if (current_tag.type == TAG_TYPE::OPENING_TAG)
				tags.push(current_tag);

			else
			{
				//if the same tag is on top of the stack just pop the tag
				if (tags.top().tag_name == current_tag.tag_name)
					tags.pop();
				else
				{
					//if a tag mismatch at the same line
					if (tags.top().line == current_tag.line)
					{
						tag_pair.first = tags.top();
						tag_pair.second = current_tag;
						mismatch.push_back(tag_pair);
						string err = "Tags <" + tags.top().tag_name + "> and </" + current_tag.tag_name + "mismatch";
						error_list.push_back(xml_error(ERROR_TYPE::MISMATCH_TAG, ERROR_MAIN_TYPE::LOGICAL, tags.top().line, true, err));
						tags.pop();
					}
					//if a tag mismatch not at the same line declare missing open error
					else
					{
						tag_pair.first=xml_tag(current_tag.tag_name, TAG_TYPE::OPENING_TAG, tags.top().line, false);
						tag_pair.second = current_tag;
						missing_opening.push_back(tag_pair);
					}
				}
			}
		}
		line++;
	}

	while (!tags.empty())
	{
		string err = "Unkown error at tag \"" + tags.top().tag_name + "\"";
		error_list.push_back(xml_error(ERROR_TYPE::UNKNOWN_ERROR, ERROR_MAIN_TYPE::LOGICAL, tags.top().line, false, err));
		tags.pop();
	}
	success = 1;
	g_error_list = error_list;
}
ostream& operator << (ostream& out, const ERROR_TYPE& c)
{
	if (c == ERROR_TYPE::MISMATCH_TAG)
		out << "Missmatching tags";
	else if (c == ERROR_TYPE::M_BRACKET)
		out << "Missing brackets '<>'";
	else if (c == ERROR_TYPE::M_CLOSE)
		out << "Missing closing tag";
	else if (c == ERROR_TYPE::M_OPEN)
		out << "Missing opening tag";
	else if (c == ERROR_TYPE::UNKNOWN_ERROR)
		out << "Unknown Error found";
	else if (c == ERROR_TYPE::UNKNOWN_TAG)
		out << "Tag not in tag list";
	else if (c == ERROR_TYPE::FILE_NOT_OPEN)
		out << "Couldn't open file";
	return out;
}
ostream& operator << (ostream& out, const ERROR_MAIN_TYPE& c)
{
	if (c == ERROR_MAIN_TYPE::LOGICAL)
		out << "Logical error : ";
	else
		out << "Syntax error  : ";
	return out;
}
ostream& operator << (ostream& out, const xml_error& c)
{
	out << "Line : " << c.line << "\t" << c.main_type << c.error_type \
		<< "\tSolvabilty : " << (c.solvable ? "can be solved" : "can't be solved");
	return out;
}
