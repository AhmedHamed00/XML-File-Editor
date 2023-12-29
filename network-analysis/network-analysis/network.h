#pragma once
#include <string>
#include <vector>
#include <list>
using namespace std;
class post
{
public:
	string body;
	vector<string> topics;
	post(string post_xml);
};
class user
{
public:
	int id = -1;
	string name = "";
	vector<post> posts = {};
	vector<int> followers = {};
	user(string user_xml);
	post search_posts(string key);
	vector<post> search_posts_all(string key);
};
class network
{
public:
	int number_of_users=0;
	vector<user> users = {};
	network(string xml_path);
	post search_posts(string key);
	vector<pair<post,int>> search_posts_all(string key);
	void generate_xml(string path);
};
class networking_analysis
{
public:
	user most_active;
	user most_influential;
	vector<list<int>> graph;
	networking_analysis(network);
};