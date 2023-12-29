#include "network.h"
#include <fstream>
#include <regex>
//a function to extract the text in a specific tag from a file
string get_tagText(string tag_name,ifstream& file)
{
	string line_text = "";
	string tag_text = "";
	//find the opening tag of the required tag name
	while (line_text.find("<"+tag_name+">") == -1 && file)
		getline(file, line_text);
	//read one more line
	getline(file, line_text);
	//read lines unitl u find the closing tag
	while (line_text.find("</"+tag_name+">") == -1)
	{
		tag_text += line_text;
		getline(file, line_text);
	}
	return tag_text;
}

//return the inside content of the parent tag
string get_tagContent(string tag_text)
{
	int start_at, end_at;
	start_at = tag_text.find_first_of('>');
	end_at = tag_text.find_last_of('<');
	string content = tag_text.substr(start_at+1,end_at-start_at-1);
	return content;
}

//return a trimmed string (no leading nor trailing white spaces)
string trim_str(string str)
{
	int start = str.find_first_not_of(" \t\n");
	int end = str.find_last_not_of(" \t\n");
	return str.substr(start, end - start + 1);
}
void parseFollowers()
{

}
network::network(string xml_path)
{
	//open the xml file
	ifstream file(xml_path);
	//loop through the file and find all users
	while (file)
	{
		//extract user text
		string user_text = get_tagText("user",file);
		//add new user the network
		users.push_back(user(user_text));
		//increase the number of user in the network
		number_of_users++;
	}
	//close the xml file
	file.close();
	//TODO:sort users by id
}
user::user(string user_xml)
{
	//regex used for searching for each tag
	regex id_search("<id>.*<\/id>");
	regex name_search("<name>.*<\/name>");
	regex posts_search("<posts>.*<\/posts>");
	regex followers_search("<followers>.*<\/followers>");
	smatch match_res;
	//first search for id
	if (regex_search(user_xml, match_res, id_search))
	{
		//set id
		string id_text = get_tagContent(static_cast<string>(match_res[0]));
		id = stoi(id_text);
		user_xml = static_cast<string>(match_res.suffix());
	}
	if (regex_search(user_xml, match_res, name_search))
	{
		//set name
		string name_text = get_tagContent(static_cast<string>(match_res[0]));
		name = trim_str(name_text);
		user_xml = static_cast<string>(match_res.suffix());
	}
	if (regex_search(user_xml, match_res, posts_search))
	{
		//search for every post and add it to the user list of posts
		string posts_text = get_tagContent(static_cast<string>(match_res[0]));
		vector<string> string_posts;
		regex post_search("<post>.*<\/post>");
		smatch m;
		while (regex_search(posts_text, m, post_search))
		{
			string post_text = get_tagContent(static_cast<string>(m[0]));
			posts.push_back(post(post_text));
			posts_text = static_cast<string>(m.suffix());
		}
		user_xml = static_cast<string>(match_res.suffix());
	}
	if (regex_search(user_xml, match_res, followers_search))
	{
		string followers_text = get_tagContent(static_cast<string>(match_res[0]));
		vector<string> string_followers;
		regex follower_search("<follower>.*<\/follower>");
		smatch m;
		while (regex_search(followers_text, m, follower_search))
		{
			string follower_text = get_tagContent(static_cast<string>(m[0]));
			followers.push_back(stoi(follower_text));
			followers_text = static_cast<string>(m.suffix());
		}
	}
}
post::post(string post_xml)
{
	regex body_search("<body>.*<\/body>");
	regex topics_search("<topics>.*<\/topics>");
	smatch match_res;
	if (regex_search(post_xml, match_res, body_search))
	{
		string body_text = get_tagContent(static_cast<string>(match_res[0]));
		body = trim_str(body_text);
		post_xml = static_cast<string>(match_res.suffix());
	}
	if (regex_search(post_xml, match_res, topics_search))
	{
		string topics_text = get_tagContent(static_cast<string>(match_res[0]));
		vector<string> string_topics;
		regex topic_search("<topic>.*<\/topic>");
		smatch m;
		while (regex_search(topics_text, m, topic_search))
		{
			string topic_text = get_tagContent(static_cast<string>(m[0]));
			topics.push_back(trim_str(topic_text));
			topics_text = static_cast<string>(m.suffix());
		}
	}
}