#include "error_handling.h"
using namespace std;

//vector of errors this is used to display the errors
vector<xml_error> error_list;

//vectors to hold each type of solvable error
vector<pair<xml_tag, xml_tag>> mismatch_error;
vector<pair<xml_tag, xml_tag>> missing_opening;
vector<pair<xml_tag, xml_tag>> missing_closing;
vector<xml_tag> missing_bracket;
vector<pair<xml_tag, string>> misspelled_tags;

//the variable holds the last parsed line in the file
static string search_text;
//the file path
static string input_file_path;
//known tag bool
vector<string> tag_list = { "users","user","id","name","posts","post","topics","topic","body","followers","follower" };

/****************************************************************************************************************************
*												   error detection
*****************************************************************************************************************************/

//this function calculates the Levenshtein distance between two strings 
static int lev_dist(string str1,string str2)
{
	int m = str1.length();
	int n = str2.length();
	//initialize matrix with zero
	vector<vector<int> > levMat(m + 1, vector<int>(n + 1, 0));

	//fill the matrix with prefixes cost of 1 /*BASE COST*/
	for (int i = 0; i <= m; i++)
		levMat[i][0] = i;
	for (int j = 0; j <= n; j++)
		levMat[0][j] = j;

	for (int i = 1; i <= m; i++) 
	{
		for (int j = 1; j <= n; j++) 
		{
			//if the same characters no extra cost
			if (str1[i - 1] == str2[j - 1]) 
			{
				levMat[i][j] = levMat[i - 1][j - 1];
			}
			//if they differ look in the adjacent cells and find the minimum value + 1 
			else 
			{
				levMat[i][j] = 1+ min(levMat[i][j - 1],min(levMat[i - 1][j],levMat[i - 1][j - 1]));
			}
		}
	}

	return levMat[m][n];
}

//this funciton checks if a unkown tag could be corrected
static bool spelling_error_check(xml_tag& tag)
{
	string str = tag.tag_name;
	//remove white spaces in tag
	for (int i = 0; i < str.size(); i++)
		if (str[i] == ' ')
			str.erase(str.begin() + i);
	//find the distance to all the known tags
	vector<int> dist;
	for (int i = 0; i < tag_list.size(); i++)
		dist.push_back(lev_dist(str, tag_list[i]));
	//find the min distance
	int min = INT_MAX,min_i=dist.size();
	for (int i = 0; i < dist.size(); i++)
	{
		if (dist[i] < min)
		{
			min_i = i;
			min = dist[i];
		}
	}
	//if the min distance is less than 3 it could be corrected
	if (min <= 3)
	{
		string err = "Tag : \"" + tag.tag_name + "\" is unkown, did you mean \""+tag_list[min_i]+"\" ?";
		error_list.push_back(xml_error(ERROR_TYPE::UNKNOWN_TAG, ERROR_MAIN_TYPE::LOGICAL, true, tag.line, err));
		pair<xml_tag, string> pair;
		pair.first = tag;
		pair.second = tag_list[min_i];
		misspelled_tags.push_back(pair);
		tag.tag_name = tag_list[min_i];
		return true;
	}
	return false;
}

//This function is used to find the missing bracket in opeening or closing tags
static bool find_bracket_error(string path)
{
	ifstream file(path);
	if (!file.is_open())
	{
		string err = "Problem openning file: " + path;
		error_list.push_back(xml_error(ERROR_TYPE::FILE_NOT_OPEN, ERROR_MAIN_TYPE::LOGICAL, false, 0, err));
		file.close();
		return false;
	}
	string parsed;
	smatch s;
	regex search("(<\/?[a-zA-Z]+)(?![a-zA-Z]*>)|([ \t]\/?[a-zA-Z]+>)|(>[ a-zA-Z1-9]+\/?[a-zA-A]+>)");
	int line = 0;
	while (file)
	{
		vector<string> m;
		getline(file, search_text);
		parsed = search_text;
		while (regex_search(parsed, s, search))
		{
			m.push_back(static_cast<string>(s[0]));
			parsed = static_cast<string>(s.suffix());
		}

		for(string x:m)
		{
			string err = "Missing Tag bracket : " + x;
			error_list.push_back(xml_error(ERROR_TYPE::M_BRACKET, ERROR_MAIN_TYPE::SYNTAX, true, line,err));
			if(x[0]=='<')
				missing_bracket.push_back(xml_tag(x,TAG_TYPE::M_CLOSE_BRACKET,line,true, \
					search_text.size(),file.tellg()));
			else
				missing_bracket.push_back(xml_tag(x, TAG_TYPE::M_OPEN_BRACKET, line, true, \
					search_text.size(), file.tellg()));
		}
		line++;
	}
	file.seekg(0, ios::beg);
	if (error_list.empty())return false;
	return true;
}

//this funciton is used to parse all the tags from the next line of the file and return a vector of them
static vector<string> get_tags(istream& file)
{
	string parsed;
	smatch s;
	regex tag_finder("<\/?[a-zA-Z _0-9]+>");
	getline(file, search_text);
	parsed = search_text;
	vector<string> m;
	while (regex_search(parsed, s, tag_finder))
	{
		m.push_back(static_cast<string>(s[0]));
		parsed = static_cast<string>(s.suffix());
	}
	return m;
}

//this funciton checks if the tag is in the list of tags used for the social media xml app
static bool is_inTagList(string tag_name)
{
	for (string tag_name_ : tag_list)
		if (tag_name_ == tag_name)return true;
	return false;
}

//this functions removes a missing opening error and a missing closing error at specific indexes at creates a mismatch error
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

//this function clears all global variables for checking for new erros
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
	missing_bracket.clear();
	missing_bracket.shrink_to_fit();
	search_text.clear();
	search_text.shrink_to_fit();
	misspelled_tags.clear();
	misspelled_tags.shrink_to_fit();
}

//this function creates erros based on the remaning elements in the missing opening and closing tags vectors
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

//this is the main error detection funciton
uint8_t find_errors(string file_path, uint8_t& success, int check_flag)
{
	uint8_t multiTagLine_ERR = false;//this flag means that the errors are found but cant be corrected as the\
	 file formatting is hard to correct
	clear_vectorsData();
	input_file_path = file_path;
	if (check_flag)
	{
		if (find_bracket_error(file_path))
		{
			success = 0;
			return multiTagLine_ERR;
		}
	}
	ifstream file(file_path);
	if (!file.is_open())
	{
		string err = "Problem openning file: " + file_path;
		error_list.push_back(xml_error(ERROR_TYPE::FILE_NOT_OPEN, ERROR_MAIN_TYPE::LOGICAL, false, 0, err));
		success = 0;
		file.close();
		return multiTagLine_ERR;
	}
	int line = 0;
	stack<xml_tag> tags;
	vector<string> string_tags;
	pair<xml_tag, xml_tag> tag_pair;
	xml_tag current_tag;
	while (file)
	{
		string_tags = get_tags(file);
		if (string_tags.size() > 2)multiTagLine_ERR = true;
		for (string string_tag : string_tags)
		{
			//get tag type and create a tag object
			if (string_tag[1] == '/')
				current_tag = xml_tag(string_tag.substr(2, string_tag.size()-3), TAG_TYPE::CLOSING_TAG, line, true,\
					search_text.size(),file.tellg());
			else
				current_tag = xml_tag(string_tag.substr(1, string_tag.length()-2), TAG_TYPE::OPENING_TAG, line, true,\
					search_text.size(), file.tellg());

			//check if tag is in the tag list
			if (!is_inTagList(current_tag.tag_name))
			{
				if (!spelling_error_check(current_tag))
				{
					string err = "Tag : \"" + current_tag.tag_name + "\" is not in the tag list";
					error_list.push_back(xml_error(ERROR_TYPE::UNKNOWN_TAG, ERROR_MAIN_TYPE::LOGICAL, false, line, err));
				}
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
					if (!tags.empty())
						tag_pair.first = xml_tag(current_tag.tag_name, TAG_TYPE::OPENING_TAG, tags.top().line, false, \
							search_text.size(), file.tellg());
					else
						tag_pair.first = xml_tag(current_tag.tag_name, TAG_TYPE::OPENING_TAG, 0, false, \
							search_text.size(), file.tellg());
					tag_pair.second = current_tag;
					missing_opening.push_back(tag_pair);
				}
			}
		}
		line++;
	}
	int line_limit = line;
	int eq_flag = false;
	while (!tags.empty())
	{
		//if no errors was found
		if (missing_opening.empty())
		{
			//TODO:check again for the line numbers when correcting
			//add error to mising closing 
			tag_pair.first = tags.top();
			tag_pair.second = xml_tag(tags.top().tag_name, TAG_TYPE::CLOSING_TAG, tags.top().line, \
				false, tags.top().line_size,tags.top().line_add );
			missing_closing.push_back(tag_pair);
			tags.pop();
		}
		//look for a missing opening tag with the same line as the top of the stack
		for (int i=0;i<missing_opening.size();i++)
		{
			//if found it is a missing closing tag
			if (missing_opening[i].first.line >= tags.top().line)
			{
				if (missing_opening[i].first.line == tags.top().line)eq_flag = true;
				else eq_flag = false;
				//add error to mising closing and remove it from missing opening 
				tag_pair.first = tags.top();
				tag_pair.second = xml_tag(tags.top().tag_name, TAG_TYPE::CLOSING_TAG, missing_opening[i].second.line, false,\
					missing_opening[i].second.line_size, missing_opening[i].second.line_add);
				missing_closing.push_back(tag_pair);
				line_limit = tags.top().line;
				tags.pop();
				//try to pop the stack
				if (tags.empty())break;
				for (int j = missing_opening.size()-1; j>=i && j< missing_opening.size(); j--)
				{
					if ((missing_opening[j].first.line > line_limit) && eq_flag)
						continue;
					//if you found a matching tag pop the stack and remove the error
					if (tags.top().tag_name == missing_opening[j].first.tag_name)
					{
						tags.pop();
						missing_opening.erase(missing_opening.begin() + j);
						j++;
					}
				}
				break;
			}
			if (tags.empty())break;
		}
	}
	//sort the arrays to use binary search
	sort(missing_opening.begin(), missing_opening.end());
	sort(missing_closing.begin(), missing_closing.end());
	//find mismatches
	//a mismatch is when the same tag is a missing open and another tag is missing close and both are in the same line
	for (int i = 0;i<missing_opening.size();i++)
	{
		int l = 0, r = missing_closing.size() - 1;
		int j = -1;
		int m = ceil(l + static_cast<float>(r - l) / 2);
		while (l <= r)
		{
			m = ceil(l + static_cast<float>(r - l) / 2);
			if (missing_closing[m].first.line == missing_opening[i].first.line)
			{
				if (missing_closing[m].second.line == missing_opening[i].second.line)
				{
					j = m;
					break;
				}
				else if (missing_closing[m].second.line < missing_opening[i].second.line)
					m++;
				else if (missing_closing[m].second.line < missing_opening[i].second.line)
					m--;
				if (m > r || m < l)
					break;
			}
			if (missing_closing[m].first.line < missing_opening[i].first.line)
				l = m+1;
			else
				r = m-1;
		}
		if (j != -1)
		{
			create_mismatchError(i, j);
			i--;
		}
	}
	create_error();
	success = 1;
	file.close();
	return multiTagLine_ERR;
}

//operator overloading for printing the errors
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

//comparision operators to enable the use of the built in sort funciton
bool operator>(const xml_tag& a, const xml_tag& b)
{
	if (a.line > b.line)
		return true;
	return false;
}
bool operator<(const xml_tag& a, const xml_tag& b)
{
	if (a.line < b.line)
		return true;
	return false;
}

/****************************************************************************************************************************
*											The end of error detection
*****************************************************************************************************************************/

/****************************************************************************************************************************
*												   error correction
*****************************************************************************************************************************/
void solve_missingBracket(fstream& file)
{
	int prev_line = missing_bracket[0].line;
	file.seekg(missing_bracket[0].line_add-static_cast<streampos>(missing_bracket[0].line_size + 2));
	string full_line;
	getline(file, full_line);
	file.seekg(missing_bracket[0].line_add - static_cast<streampos>(missing_bracket[0].line_size + 2));
	int last_in = 0;
	for (int i = 0; i < missing_bracket.size(); i++)
	{
		if (missing_bracket[i].line != prev_line)
		{
			file << full_line;
			prev_line = missing_bracket[i].line;
			file.seekg(missing_bracket[i].line_add - static_cast<streampos>(missing_bracket[i].line_size + 2));
			getline(file, full_line);
			file.seekg(missing_bracket[i].line_add - static_cast<streampos>(missing_bracket[i].line_size + 2));
			last_in = 0;
		}
		//find the tag with the missing bracket in the full line
		int insert_at = full_line.find(missing_bracket[i].tag_name, last_in);
		if (missing_bracket[i].type == TAG_TYPE::M_CLOSE_BRACKET)
		{
			full_line.insert(full_line.begin() + insert_at + missing_bracket[i].tag_name.size(), '>');
			last_in += insert_at+ missing_bracket[i].tag_name.size();
		}
		else
		{//missing open bracket
			//either found a space or found a closing bracket
			if (missing_bracket[i].tag_name.find('>',0) != -1)
			{//if found a closing bracket
				//look for either a space or a /
				if (full_line.find('/', insert_at) != -1)
				{
					insert_at = full_line.find('/', insert_at);
					//insert before the /
					full_line.insert(full_line.begin() + insert_at, '<');
				}
				else
				{//find the last space
					insert_at = full_line.find_last_of(' ', insert_at);
					//insert after the last space
					full_line.insert(full_line.begin() + insert_at + 1, '<');
				}
			}
			else
			{//insert after the space
				//find the last space
				insert_at = full_line.find_last_of(' ', insert_at);
				//insert after the last space
				full_line.insert(full_line.begin() + insert_at + 1, '<');
			}
			last_in += insert_at + missing_bracket[i].tag_name.size();
		}
	}
	file << full_line;
}