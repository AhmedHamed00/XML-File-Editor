#pragma once
#include <string>
#include <vector>
#include <list>
using namespace std;
class post
{
public:
	string body = "";
	vector<string> topics = {};
	post(string& post_xml);
	post() {};
	friend ostream& operator<< (ostream& out, const post& c);
};
class user
{
public:
	int id = -1;
	int index = -1;
	string name = "";
	vector<post> posts = {};
	vector<int> followers = {};
	user(string& user_xml,int _index);
	post search_posts(string key);
	vector<post> search_posts_all(string key);
	user() {};
	friend ostream& operator<< (ostream& out, const user& c);
};
class network
{
public:
	int number_of_users=0;
	vector<user> users = {};
	network(string& xml_path);
	pair<int, post> search_posts(string key);
	vector<pair<int,vector<post>>> search_posts_all(string key);
	int get_index(int _id);
	user* get_user(int _id);
	network() {};
};
class networking_analysis
{
public:
	user most_active;
	user most_influential;
	network my_network;
	vector<vector<bool>> network_graph;
	networking_analysis(string& xml_path);
	vector<user*> suggest_followers(int u_id);
	vector<user*> get_mutualFollowers(int u1_id, int u2_id);
	friend ostream& operator<< (ostream& out, const networking_analysis& c);
};