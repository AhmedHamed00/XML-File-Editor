#include "error_handling.h"
using namespace std;

//vector of errors this is used to display the errors
vector<xml_error> error_list;
//the file
vector<string> string_file;

//vectors to hold each type of solvable error
vector<pair<xml_tag, xml_tag>> mismatch_error;
vector<xml_tag> missing_opening;
vector<xml_tag> missing_closing;
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
		if (str[i] == ' ' || str[i]=='\t')
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
	if (min <= 2)
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
	regex search("(</?[a-zA-Z]+)(?![a-zA-Z]*>)|([ \t]/?[a-zA-Z/0-9]+>)|(>[ a-zA-Z1-9]+/?[a-zA-Z0-9/]+>)");
	int line = 0;
	while (file)
	{
		vector<string> m;
		getline(file, search_text);
		string_file.push_back(search_text);
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
				missing_bracket.push_back(xml_tag(x,TAG_TYPE::M_CLOSE_BRACKET,line));
			else
				missing_bracket.push_back(xml_tag(x, TAG_TYPE::M_OPEN_BRACKET, line));
		}
		line++;
	}
	file.close();
	if (error_list.empty())return false;
	return true;
}

//this funciton is used to parse all the tags from the next line of the file and return a vector of them
static vector<string> get_tags(string& search_text)
{
	string parsed;
	smatch s;
	regex tag_finder("<\/?[a-zA-Z _0-9]+>");
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
	string_file.clear();
	string_file.shrink_to_fit();
}

//this function makes sure that the alst line is empty
static void ensure_newLine(string file_path)
{
	fstream file(file_path);
	char s;
	file.seekg(-1, ios_base::end);
	s = file.get();
	if(s!='\n')
		file << endl;
	file.close();
}

static void parse_file(ifstream &file)
{
	//clear the string file
	string_file.clear();
	string_file.shrink_to_fit();
	stack<xml_tag> tags;
	pair<xml_tag, xml_tag> mismatch_tag_pair;
	int line = 0;
	while (file)
	{
		//read line
		getline(file, search_text);
		//add line to string file
		string_file.push_back(search_text);
		//get tags in line
		vector<string> string_tags = get_tags(search_text);
		xml_tag current_tag;
		for (string tag : string_tags)
		{
			if (tag[1] == '/')//closing tag
				current_tag = xml_tag(tag.substr(2, tag.size() - 3), TAG_TYPE::CLOSING_TAG, line);
			else
				current_tag = xml_tag(tag.substr(1, tag.size() - 2), TAG_TYPE::OPENING_TAG, line);

			if (current_tag.type == TAG_TYPE::OPENING_TAG)
				tags.push(current_tag);
			else
			{
				if (tags.empty())
				{
					mismatch_tag_pair.second = current_tag;
					mismatch_tag_pair.first = xml_tag("", TAG_TYPE::NULL_TAG, line);
					mismatch_error.push_back(mismatch_tag_pair);
				}
				else
				{
					if (tags.top().tag_name == current_tag.tag_name)
						tags.pop();
					else
					{
						mismatch_tag_pair.first = tags.top();
						mismatch_tag_pair.second = current_tag;
						mismatch_error.push_back(mismatch_tag_pair);
						tags.pop();
					}
				}
			}
		}
		line++;
	}
	while (!tags.empty())
	{
		mismatch_tag_pair.first = tags.top();
		mismatch_tag_pair.second = xml_tag("",TAG_TYPE::NULL_TAG,tags.top().line);
		mismatch_error.push_back(mismatch_tag_pair);
		tags.pop();
	}
}
static bool remove_nulls()
{
	bool rem = false;
	for (int i = 0; i < mismatch_error.size(); i++)
	{
		if (mismatch_error[i].first.type == TAG_TYPE::NULL_TAG && mismatch_error[i].second.type == TAG_TYPE::NULL_TAG)
		{//if both tags are null remove it
			mismatch_error.erase(mismatch_error.begin() + i);
			i--;
			rem = true;
		}
	}
	return rem;
}
static void parse_misMatch()
{
	for (int i = 0; i < mismatch_error.size(); i++)
	{
		if ((i + 1 < mismatch_error.size())\
			&& (mismatch_error[i].first.type != TAG_TYPE::NULL_TAG)\
			&& mismatch_error[i + 1].second.type != TAG_TYPE::NULL_TAG)
		{
			if (mismatch_error[i].first.tag_name == mismatch_error[i + 1].second.tag_name)
			{
				mismatch_error[i].first.type = TAG_TYPE::NULL_TAG;
				mismatch_error[i+1].second.type = TAG_TYPE::NULL_TAG;
			}
		}
		//reached the end of the vector
		if (mismatch_error[i].second.type == TAG_TYPE::NULL_TAG)break;
		for (int j = i+1; j < mismatch_error.size(); j++)
		{
			if (mismatch_error[j].first.line > mismatch_error[i].first.line)break;
			if (mismatch_error[j].first.type == TAG_TYPE::NULL_TAG)continue;
			if (mismatch_error[j].first.tag_name == mismatch_error[i].second.tag_name)
			{//they can be poped
				mismatch_error[j].first.type = TAG_TYPE::NULL_TAG;
				mismatch_error[i].second.type = TAG_TYPE::NULL_TAG;
				break;
			}
		}
	}
}
static void parse_errors()
{
	for(int i=0;i<mismatch_error.size();i++)
	{ 
		if (mismatch_error[i].first.type == TAG_TYPE::NULL_TAG && mismatch_error[i].second.type == TAG_TYPE::NULL_TAG)
		{//if both tags are null remove it
			mismatch_error.erase(mismatch_error.begin() + i);
			i--;
		}
		else if (mismatch_error[i].first.type == TAG_TYPE::NULL_TAG)
		{//missing opening tag
			string err = "Tag \'</" + mismatch_error[i].second.tag_name + ">\' was not opened";
			error_list.push_back(xml_error(ERROR_TYPE::M_OPEN, ERROR_MAIN_TYPE::SYNTAX, true, mismatch_error[i].second.line, err));
			missing_opening.push_back(mismatch_error[i].second);
			mismatch_error.erase(mismatch_error.begin() + i);
			i--;
		}
		else if (mismatch_error[i].second.type == TAG_TYPE::NULL_TAG)
		{
			string err = "Tag \'<" + mismatch_error[i].first.tag_name + ">\' was not closed";
			error_list.push_back(xml_error(ERROR_TYPE::M_CLOSE, ERROR_MAIN_TYPE::SYNTAX, true, mismatch_error[i].first.line, err));
			missing_closing.push_back(mismatch_error[i].first);
			mismatch_error.erase(mismatch_error.begin() + i);
			i--;
		}
		else
		{
			string err = "and Line : "+to_string(mismatch_error[i].second.line)+" Tags \'<" + \
				mismatch_error[i].first.tag_name + "\'> and \'</" +mismatch_error[i].second.tag_name + "\'> doesnt match" ;
			error_list.push_back(xml_error(ERROR_TYPE::MISMATCH_TAG, ERROR_MAIN_TYPE::SYNTAX, true, mismatch_error[i].first.line, err));
		}
	}
}
//this is the main error detection funciton
bool find_errors(string file_path, int check_flag)
{
	//clear all data
	clear_vectorsData();
	//make sure the file ends in a new line
	ensure_newLine(file_path);
	//set global path
	input_file_path = file_path;
	//if the user want to check for mssing brackets do it
	if (check_flag)
	{
		if (find_bracket_error(file_path))
		{
			return false;
		}
	}
	//open the file
	ifstream file(file_path);
	//check if the file is open
	if (!file.is_open())
	{
		string err = "Problem openning file: " + file_path;
		error_list.push_back(xml_error(ERROR_TYPE::FILE_NOT_OPEN, ERROR_MAIN_TYPE::LOGICAL, false, 0, err));
		file.close();
		return false;
	}
	//parse the file and get mismatches
	parse_file(file);
	do
	{
		parse_misMatch();
	} while (remove_nulls());
	parse_errors();
	//close the file and return success 
	file.close();
	return 1;
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
bool operator>(const xml_error& a, const xml_error& b)
{
	if (a.line > b.line)
		return true;
	return false;
}
bool operator<(const xml_error& a, const xml_error& b)
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
static int get_priority(string _tag_name)
{
	if (_tag_name == "id" || _tag_name == "name" || _tag_name == "body" || _tag_name == "topic")return 0;
	if (_tag_name == "topics" || _tag_name == "follower")return 1;
	if (_tag_name == "post" || _tag_name == "followers")return 2;
	if (_tag_name == "posts")return 3;
	if (_tag_name == "user")return 4;
	if (_tag_name == "users")return 5;
}
static string vec_to_str(vector<string>& content)
{
	string res;
	for (string line : content)
	{
		res += line;
	}
	return res;
}
static vector<string> get_tags_vec(vector<string>& str)
{//this function returns all the tags in the vector of strings
	vector<string> res;
	for (int i = str.size() - 1; i >= 0; i--)
	{//bgeeb el tagss mn t7t l fo2
		vector<string> tags_inLine;
		tags_inLine = get_tags(str[i]);
		res.insert(res.begin(), tags_inLine.begin(), tags_inLine.end());
	}
	return res;
}
static int check_tagContent(string& open,string& close,vector<string>& content)
{//return 1 if the tag should be the opening tag,2 if it should be the closing tag,return 0 if needs more info
	vector<string> content_tags = get_tags_vec(content);
	int close_match = 0;
	int open_match = 0;
	int done_flag = 0;
	//vectors to hold the nesting values
	//TODO:use another header file for abstraction
	vector<string> topics_nest = { "topics","topic" };
	vector<string> follower_nest = { "follower","id" };
	vector<string> followers_nest = { "followers","follower" };
	vector<string> post_nest = { "post","topics","body" };
	vector<string> posts_nest = { "posts","post" };
	vector<string> user_nest = { "user","id","name","followers","posts" };
	vector<string> users_nest = { "users","user" };
	vector<vector<string>> nesting_lists = { topics_nest,follower_nest,followers_nest,post_nest,posts_nest,\
		user_nest,users_nest };
	if (content_tags.size())
	{//has tags in between
		//check the nesting
		for (string tag : content_tags)
		{
			string last_tag;
			if (tag[1] == '/')
				last_tag = tag.substr(2, tag.size() - 3);
			else
				last_tag = tag.substr(1, tag.length() - 2);

			for (int i = 0; i < nesting_lists.size(); i++)
			{
				//check the nesting with respect to the closing tag
				if (nesting_lists[i][0] == close)
				{
					for (int o = 1; o < nesting_lists[i].size(); o++)
					{
						if (nesting_lists[i][o] == last_tag)
						{
							close_match++;
							done_flag++;
							break;
						}
					}
				}
				//check the nesting with respect to the opening tag
				else if (nesting_lists[i][0] == open)
				{
					for (int o = 1; o < nesting_lists[i].size(); o++)
					{
						if (nesting_lists[i][o] == last_tag)
						{
							open_match++;
							done_flag++;
							break;
						}
					}
				}
				if (done_flag == 2)
					break;
			}
		}
		if (close_match > open_match)return 2;
		if (close_match < open_match)return 1;
		else return 0;
	}
	else
	{//no tags in between only text
		//when only text then the tag could be topic,name,id,body
		// first look if one of the tags is a nested tag
		bool found_open = false, found_close = false;
		for (int i=0;i<nesting_lists.size();i++)
		{
			if (nesting_lists[i][0] == open)
				found_open = true;
			else if (nesting_lists[i][0] == close)
				found_close = true;
		}
		if (found_close && !found_open)
			//the open tag is the correct one
			return 1;
		if (!found_close && found_close)
			//the close tag is the correct one
			return 2;

		//else
		//get all the content in one line string to search using regex
		string string_content = vec_to_str(content);
		regex id_serach("^[ \t]*\d+[ \t]*$");//only digits with whitespaces b4 and after
		if (regex_match(string_content, id_serach))
		{
			//the content is only numbers
			//we give higher prio to id
			if (open == "id")return 1;
			else if (close == "id")return 2;
			//now it cant be name or topic
			//so if it is not body its an error
			else if (open == "body")return 1;
			else if (close == "body")return 2;
			else return 0;
		}
		//its not id
		//if any numbers its body
		smatch s;
		if(regex_search(string_content,s, regex("[\d,\.\?!]")))
		{//its body or error
			if (open == "body")return 1;
			else if (close == "body")return 2;
			else return 0;
		}
		//now we have no idea what the tag is so return error
		return 0;
	}
	return true;
}
static void replace_closing(int i)
{
	int insert_at = string_file[mismatch_error[i].second.line].find("</" + mismatch_error[i].second.tag_name + ">");
	string_file[mismatch_error[i].second.line].replace(insert_at + 2, mismatch_error[i].second.tag_name.size(), \
		mismatch_error[i].first.tag_name);
}
static void replace_opening(int i)
{
	int insert_at = string_file[mismatch_error[i].first.line].find("<" + mismatch_error[i].first.tag_name + ">");
	string_file[mismatch_error[i].first.line].replace(insert_at + 1, mismatch_error[i].first.tag_name.size(), \
		mismatch_error[i].second.tag_name);
}
static void writeFile()
{
	ofstream file(input_file_path, ofstream::trunc | ofstream::out);
	for (int i = 0; i < string_file.size(); i++)
	{
		file << string_file[i] << endl;
	}
	file.close();
}
static void solve_missingBracket()
{
	int prev_line = missing_bracket[0].line;
	int last_in = 0;
	for (int i = 0; i < missing_bracket.size(); i++)
	{
		if (missing_bracket[i].line != prev_line)
		{
			prev_line = missing_bracket[i].line;
			last_in = 0;
		}
		//find the tag with the missing bracket in the full line
		int insert_at = string_file[prev_line].find(missing_bracket[i].tag_name, last_in);
		if (missing_bracket[i].type == TAG_TYPE::M_CLOSE_BRACKET)
		{
			string_file[prev_line].insert(string_file[prev_line].begin() + insert_at + \
				missing_bracket[i].tag_name.size(), '>');
			last_in += insert_at + missing_bracket[i].tag_name.size();
		}
		else
		{//missing open bracket
			//either found a space or tab or found a closing bracket
			if ((missing_bracket[i].tag_name[0] == '>') && (string_file[prev_line].find_last_of('/') != -1))
			{//if found a closing bracket and a fwd slash
				//look /
				insert_at = string_file[prev_line].find_last_of('/');
				//insert before the /
				string_file[prev_line].insert(string_file[prev_line].begin() + insert_at, '<');
				last_in = insert_at;
			}
			else
			{//replace the space or tab insert before the /
				//replace the last space or tab or insert before the /
				int start = string_file[prev_line].find(missing_bracket[i].tag_name);
				int end = start + missing_bracket[i].tag_name.size();
				for (insert_at = end; insert_at > start; insert_at--)
					if ((string_file[prev_line][insert_at] == ' ') || (string_file[prev_line][insert_at] == '\t') || \
						(string_file[prev_line][insert_at] == '/'))break;
				//replace after the last space or insert before the /
				if (string_file[prev_line][insert_at] == '/')
					string_file[prev_line].insert(string_file[prev_line].begin() + insert_at, '<');
				else
					string_file[prev_line][insert_at] = '<';
			}
			last_in += insert_at + missing_bracket[i].tag_name.size();
		}
	}
}
static void solve_missing_closing_prio_0(int index)
{
	int line = missing_closing[index].line;
	vector<string> tags = {};
	while (tags.size() == 0 && line < string_file.size())
	{
		tags = get_tags(string_file[line]);
		line++;
	}
	if (line >= string_file.size())
		string_file[string_file.size() - 1] += ("</" + missing_closing[index].tag_name + ">");
	else
	{
		line--;
		string_file[line].insert(string_file[line].find_first_of(tags[0]), "</" + missing_closing[index].tag_name + ">\n");
	}
}
static  void solve_missingClose()
{
	sort(missing_closing.begin(), missing_closing.end());
	for (int i = 0; i < missing_closing.size(); i++)
	{
		switch (get_priority(missing_closing[i].tag_name))
		{
		case 0:
			solve_missing_closing_prio_0(i);
			break;
		}
	}
}
static void solve_missingOpen()
{
	
}
static bool solve_mismatch()
{
	for (int i = mismatch_error.size() - 1; i >= 0; i--)
	{
		if ((!is_inTagList(mismatch_error[i].first.tag_name)) && (!is_inTagList(mismatch_error[i].second.tag_name)))
			return false;
		if ((!is_inTagList(mismatch_error[i].first.tag_name)) || (!is_inTagList(mismatch_error[i].second.tag_name)))
		{//if one of the tags in not in the tag list then change it to the other
			if (is_inTagList(mismatch_error[i].first.tag_name))
				//the closing tag is not in the list,replace the closing tag with the opening tag
				replace_closing(i);
			else//the opening tag is not in the list,replace the opening tag with the closing tag
				replace_opening(i);
			continue;
		}
		vector<string> content;
		if (mismatch_error[i].first.line == mismatch_error[i].second.line)
		{//if both tags are in the same line the content in between them
			int start_at = string_file[mismatch_error[i].first.line].find("<" + mismatch_error[i].first.tag_name + ">");
			int end_at = string_file[mismatch_error[i].second.line].find("</" + mismatch_error[i].second.tag_name + ">");
			content.push_back(string_file[mismatch_error[i].first.line].substr(\
				start_at + 2 + mismatch_error[i].first.tag_name.size(), \
				end_at - (start_at + mismatch_error[i].first.tag_name.size() + 2)));
		}
		else
		{
			//get the rest of the first line
			int str_at = string_file[mismatch_error[i].first.line].find("<" + mismatch_error[i].first.tag_name + ">");
			content.push_back(string_file[mismatch_error[i].first.line].substr(str_at + 2 +\
				mismatch_error[i].first.tag_name.size()));
			//get all lines in the middle
			for (int o = mismatch_error[i].first.line + 1; o < mismatch_error[i].second.line; o++)
				content.push_back(string_file[o]);
			//get the rest of the second line
			str_at = string_file[mismatch_error[i].second.line].find("</" + mismatch_error[i].second.tag_name + ">");
			content.push_back(string_file[mismatch_error[i].second.line].substr(0, str_at - 1));
		}
		//now we have the content between the tags we check to see which tag works best
		if (check_tagContent(mismatch_error[i].first.tag_name, mismatch_error[i].second.tag_name, content) == 1)
			//replace the closing tag with the opening tag
			replace_closing(i);
		else if (check_tagContent(mismatch_error[i].first.tag_name, mismatch_error[i].second.tag_name, content) == 2)
			//replace the opening tag with the closing tag
			replace_opening(i);
		else return false;
	}
	return true;
}
static void solve_misspelling()
{
	for (int i = 0; i < misspelled_tags.size(); i++)
	{
		int insert_at = string_file[misspelled_tags[i].first.line].find(misspelled_tags[i].first.tag_name);
		string_file[misspelled_tags[i].first.line].replace(insert_at, misspelled_tags[i].first.tag_name.size(), \
			misspelled_tags[i].second);
	}
}
bool solve_errors()
{
	if (misspelled_tags.size())
		solve_misspelling();

	if (missing_bracket.size())
		solve_missingBracket();

	writeFile();

	if (mismatch_error.size())
		if (solve_mismatch() == false)return false;

	writeFile();
	if (missing_closing.size())
		solve_missingClose();

	if (missing_opening.size())
		solve_missingOpen();

	writeFile();
	return true;
}
//comparision operators to enable the use of the built in sort funciton
bool operator>(const xml_tag& a, const xml_tag& b)
{
	if (get_priority(a.tag_name) > get_priority(b.tag_name))
		return true;
	return false;
}
bool operator<(const xml_tag& a, const xml_tag& b)
{
	if (get_priority(a.tag_name) < get_priority(b.tag_name))
		return true;
	return false;
}