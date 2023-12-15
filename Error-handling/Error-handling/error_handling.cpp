#include "error_handling.h"
using namespace std;
vector<xml_error> error_list;
vector<pair<xml_tag, xml_tag>> mismatch_error;
vector<pair<xml_tag, xml_tag>> missing_opening;
vector<pair<xml_tag, xml_tag>> missing_closing;
vector<string> tag_list = { "users","user","id","name","posts","post","topics","topic","body","followers","follower" };
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

static bool is_inTagList(string tag_name)
{
	for (string tag_name_ : tag_list)
		if (tag_name_ == tag_name)return true;
	return false;
}

static void create_mismatchError(int missing_opening_index, int missing_closing_index)
{
	pair<xml_tag, xml_tag> tag_pair;
	tag_pair.first = missing_closing[missing_closing_index].first;
	tag_pair.second = missing_opening[missing_opening_index].second;
	mismatch_error.push_back(tag_pair);
	string err = "Tags <" + tag_pair.first.tag_name + "> and </" + tag_pair.second.tag_name + "> mismatch";
	error_list.push_back(xml_error(ERROR_TYPE::MISMATCH_TAG, ERROR_MAIN_TYPE::LOGICAL, true,tag_pair.first.line, err));
	missing_closing.erase(missing_closing.begin() + missing_closing_index);
	missing_opening.erase(missing_opening.begin() + missing_opening_index);
}

static void clear_vectorsData()
{
	error_list.clear();
	error_list.shrink_to_fit();
	mismatch_error.clear();
	mismatch_error.shrink_to_fit();
	missing_closing.clear();
	missing_closing.shrink_to_fit();
	missing_opening.clear();
	missing_opening.shrink_to_fit();
}

static void create_error()
{
	for (int i = 0; i < missing_closing.size(); i++)
	{
		string err = "Tag : <" + missing_closing[i].first.tag_name + "> was not closed";
		error_list.push_back(xml_error(ERROR_TYPE::M_CLOSE, ERROR_MAIN_TYPE::SYNTAX, true, missing_closing[i].first.line, err));
	}
	for (int i = 0; i < missing_opening.size(); i++)
	{
		string err = "Tag : </" + missing_opening[i].second.tag_name + "> was not opened";
		error_list.push_back(xml_error(ERROR_TYPE::M_OPEN, ERROR_MAIN_TYPE::SYNTAX, true, missing_opening[i].second.line, err));
	}
}

void find_errors(string file_path, uint8_t& success, int check_flag)
{
	clear_vectorsData();
	ifstream file(file_path);
	if (!file)
	{
		string err = "Problem openning file: " + file_path;
		error_list.push_back(xml_error(ERROR_TYPE::FILE_NOT_OPEN,ERROR_MAIN_TYPE::LOGICAL,false,0,err));
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
	xml_tag current_tag;
	while (file)
	{
		string_tags = get_tags(file);
		for (string string_tag : string_tags)
		{
			//get tag type and create a tag object
			if (string_tag[1] == '/')
				current_tag = xml_tag(string_tag.substr(2, string_tag.size()-3), TAG_TYPE::CLOSING_TAG, line, true);
			else
				current_tag = xml_tag(string_tag.substr(1, string_tag.length()-2), TAG_TYPE::OPENING_TAG, line, true);

			//check if tag is in the tag list
			if (!is_inTagList(current_tag.tag_name))
			{
				string err = "Tag : \"" + current_tag.tag_name + "\" is not in the tag list";
				error_list.push_back(xml_error(ERROR_TYPE::UNKNOWN_TAG, ERROR_MAIN_TYPE::LOGICAL, false, line, err));
			}

			//check if it is a closing tag or openning tag
			if (current_tag.type == TAG_TYPE::OPENING_TAG)
				tags.push(current_tag);
			else
			{
				//if the same tag is on top of the stack just pop the tag
				if (!tags.empty() && tags.top().tag_name == current_tag.tag_name)
					tags.pop();
				else
				{
					//if a tag mismatch at the same line
					if (!tags.empty() && tags.top().line == current_tag.line)
					{
						tag_pair.first = tags.top();
						tag_pair.second = current_tag;
						mismatch_error.push_back(tag_pair);
						string err = "Tags <" + tags.top().tag_name + "> and </" + current_tag.tag_name + "> mismatch";
						error_list.push_back(xml_error(ERROR_TYPE::MISMATCH_TAG, ERROR_MAIN_TYPE::LOGICAL, true, tags.top().line, err));
						tags.pop();
					}
					//if a tag mismatch not at the same line declare missing open error
					else
					{
						if(!tags.empty())
							tag_pair.first=xml_tag(current_tag.tag_name, TAG_TYPE::OPENING_TAG, tags.top().line, false);
						else
							tag_pair.first = xml_tag(current_tag.tag_name, TAG_TYPE::OPENING_TAG, current_tag.line, false);
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
		//if no errors was found
		if (missing_opening.empty())
		{
			//add error to mising closing and remove it from missing opening
			tag_pair.first = tags.top();
			tag_pair.second = xml_tag(tags.top().tag_name, TAG_TYPE::CLOSING_TAG, tags.top().line, false);
			missing_closing.push_back(tag_pair);
			tags.pop();
		}
		//look for a missing opening tag with the same line as the top of the stack
		for (int i=0;i<missing_opening.size();i++)
		{
			//if found it is a missing closing tag
			if (missing_opening[i].first.line >= tags.top().line)
			{
				//add error to mising closing and remove it from missing opening 
				tag_pair.first = tags.top();
				tag_pair.second = xml_tag(tags.top().tag_name, TAG_TYPE::CLOSING_TAG, missing_opening[i].second.line, false);
				missing_closing.push_back(tag_pair);
				tags.pop();
				//try to pop the stack
				if (tags.empty())break;
				for (int j = i; j < missing_opening.size(); j++)
				{
					//if you found a matching tag pop the stack and remove the error
					if (tags.top().tag_name == missing_opening[j].first.tag_name)
					{
						tags.pop();
						missing_opening.erase(missing_opening.begin() + j);
						j--;
					}
				}
				break;
			}
			if (tags.empty())break;
		}
	}
	for (int i = 0;i<missing_opening.size();i++)
	{
		int l = 0, r = missing_closing.size() - 1;
		int j = -1;
		while (l <= r)
		{
			int m = ceil(l + static_cast<float>(r - l) / 2);
			if (missing_closing[m].first.line == missing_opening[i].first.line)
			{
				if (missing_closing[m].second.line == missing_opening[i].second.line)
				{
					j = m;
					break;
				}
			}
			if (missing_closing[m].first.line < missing_opening[i].first.line)
				l = m + 1;
			else
				r = m - 1;
		}
		if (j != -1)
		{
			create_mismatchError(i, j);
			i--;
		}
	}
	create_error();
	success = 1;
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
	out << "Line : " << c.line << "\t" << c.msg << endl <<'\t' << c.main_type << c.error_type \
		<< "\tSolvabilty : " << (c.solvable ? "can be solved" : "can't be solved");
	return out;
}
