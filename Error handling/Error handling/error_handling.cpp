#include "error_handling.h"
using namespace std;

xml_base_tag id("id", false, true);
xml_base_tag name("name", false, true);
xml_base_tag topic("topic", false, false);
xml_base_tag body("body", false, true);
xml_base_tag topics("topics", false, true, vector<xml_base_tag>{topic});
xml_base_tag follower("follower", false, false, vector<xml_base_tag>{id});
xml_base_tag followers("followers", true, true, vector<xml_base_tag>{follower});
xml_base_tag post("post", false, false, vector<xml_base_tag>{topics, body});
xml_base_tag posts("posts", true, false, vector<xml_base_tag>{post});
xml_base_tag user("user", false, false, vector<xml_base_tag>{id, name, posts, followers});
xml_base_tag users("users", false, true, vector<xml_base_tag>{user});
xml_base_tag unknown("", false, true);
vector<xml_base_tag> users_vect{ users };
static bool find_bracket_error(ifstream& file, vector<xml_error>& error_list)
{
	string parsed;
	smatch m;
	int line = 0;
	while (file)
	{
		getline(file, parsed);
		regex search("(<\/?[a-z]+)(?![a-z]*>)|(([ \t])(\/?[a-z]+>))");
		if (regex_search(parsed, m, search))
		{
			error_list.push_back(xml_error(ERROR_TYPE::M_BRACKET, ERROR_MAIN_TYPE::SYNTAX, line, true));
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

static xml_tag get_tag(string tag_name, int line)
{//creates and return a tag object
	if (tag_name == "id")
	{
		xml_tag tag(&id, line); return tag;
	}
	else if (tag_name == "name")
	{
		xml_tag tag(&name, line); return tag;
	}
	else if (tag_name == "topic")
	{
		xml_tag tag(&topic, line); return tag;
	}
	else if (tag_name == "body")
	{
		xml_tag tag(&body, line); return tag;
	}
	else if (tag_name == "topics")
	{
		xml_tag tag(&topics, line); return tag;
	}
	else if (tag_name == "follower")
	{
		xml_tag tag(&follower, line); return tag;
	}
	else if (tag_name == "post")
	{
		xml_tag tag(&post, line); return tag;
	}
	else if (tag_name == "posts")
	{
		xml_tag tag(&posts, line); return tag;
	}
	else if (tag_name == "user")
	{
		xml_tag tag(&users, line); return tag;
	}
	else if (tag_name == "users")
	{
		xml_tag tag(&users, line); return tag;
	}
	else
	{
		xml_tag tag(&unknown, line); return tag;
	}
}

vector<xml_error> find_errors(string file_path, uint8_t& success, int check_flag)
{
	ifstream file(file_path);
	vector<xml_error> error_list;
	if (check_flag)
	{
		if (find_bracket_error(file, error_list))
		{
			return error_list;
			success = 0;
		}
	}
	int line = 0;
	stack<xml_tag> tags;
	vector<string> string_tags;
	xml_tag current_tag;
	if (file)
	{//search for users tag
		//read tags form first line
		string_tags = get_tags(file);
		//if multiple tags fount at the first line
		if (string_tags.size() > 1)
		{//first line must be the users tag only
			error_list.push_back(xml_error(ERROR_TYPE::NO_ROOT_TAG, ERROR_MAIN_TYPE::LOGICAL, 0, false));
			return error_list;
		}
		//found one tag
		current_tag = get_tag(string_tags[1].substr(1, string_tags[1].size() - 2), line);
		if (string_tags[1][1] != '/')
		{//found openning tag
			//tag is either users or there is an error
			if (current_tag.get_type_name() == "users")
			{//found users type
				current_tag.error = ERROR_TAG::NO_ERROR;
				current_tag.in_order = true;
				tags.push(current_tag);
			}
			else if (current_tag.in_nestting_set_of(&users.nestting_list))
			{
				current_tag.error = ERROR_TAG::M_OPEN_OR_MISMATCH_OPEN;
				current_tag.in_order = false;
				current_tag.excp_type_list = &users_vect;
				tags.push(current_tag);
			}
			else
			{
				current_tag.error = ERROR_TAG::MISMATCH_OPEN_OR_UNKNOWN;
				current_tag.in_order = false;
				current_tag.excp_type_list = &users_vect;
				tags.push(current_tag);
			}
		}
		else
		{//found closing tag
			//error cant start with closing tag
			error_list.push_back(xml_error(ERROR_TYPE::NO_ROOT_TAG, ERROR_MAIN_TYPE::LOGICAL, 0, false));
			return error_list;
		}
	}
	else
	{//couldnt open file
		error_list.push_back(xml_error(ERROR_TYPE::FILE_NOT_OPEN, ERROR_MAIN_TYPE::LOGICAL, 0, false));
		return error_list;
	}
	line++;
	while (file)
	{
		//get tags from next line
		string_tags = get_tags(file);
		//loop through all tags in line
		for (string string_tag : string_tags)
		{
			//get tag from tag name
			if (string_tag[1] == '/')
			{//closing tag
				current_tag = get_tag(string_tag.substr(2, string_tag.size() - 3), line);
				current_tag.open = false;
			}
			else
			{//opening tag
				current_tag = get_tag(string_tag.substr(1, string_tag.size() - 2), line);
				current_tag.open = true;
			}

			if (current_tag.open)
			{//deal with openning tag and push on the stack
				if (current_tag.in_nestting_set_of(&tags.top().type->nestting_list))
					current_tag.in_order = true;
				else
					current_tag.in_order = false;

				if (current_tag.in_order)
				{//el order dah tmam 
				 //y2ma ely foo2 in order y2ma la
				 //yb2a top feh error aw la
					if (tags.top().in_order && tags.top().error == ERROR_TAG::NO_ERROR)
					{
						//lw top w current in order w no error in top yb2a el donia banby
						current_tag.error = ERROR_TAG::NO_ERROR;
						tags.push(current_tag);
					}
					else if (tags.top().in_order && tags.top().error != ERROR_TAG::NO_ERROR)
					{//top in order with error

					}
					else
					{//tag.open not in order + top error
						//el tag el current sa7 relative to el top bs el top asln kan feh mo4kla
						current_tag.error = ERROR_TAG::ROOT_ERROR;
						tags.push(current_tag);
						//now current tag is the new top
						//top is now in order with error
					}
				}
				else if (!current_tag.in_order)
				{//lw not in order it means 7aga mn 3
					//1-current tag 8lt
					//2-top tag 8lt
					//3-top w current 8lt
					if (tags.top().in_order && tags.top().error == ERROR_TAG::NO_ERROR)
					{

					}
					else if (tags.top().in_order && tags.top().error != ERROR_TAG::NO_ERROR)
					{

					}
					else
					{

					}
				}
			}
			else
			{//deal with closing tag and pop from the stack

			}
		}
		line++;
	}
	success = 1;
	return error_list;
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
	else if (c == ERROR_TYPE::M_SLASH)
		out << "Missing closing slash";
	else if (c == ERROR_TYPE::NESTING_ERROR)
		out << "Wrong nesting order";
	else if (c == ERROR_TYPE::UNKNOWN_ERROR)
		out << "Unknown Error found";
	else if (c == ERROR_TYPE::UNKNOWN_TAG)
		out << "Tag not in tag list";
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
