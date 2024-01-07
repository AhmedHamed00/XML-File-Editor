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
	//read lines unitl u find the closing tag
	while (line_text.find("</"+tag_name+">") == -1 && file)
	{
		getline(file, line_text);
		tag_text += line_text;
	}
	return tag_text;
}
//this function takes a string to search for a tag in it 
//the function returns the tag text wrapped around the tag
//the funciton edits the search text to discard the searched part of the string
string get_tagContent(string tag_name, string& text)
{
	int start_at = text.find("<" + tag_name + ">")+2+tag_name.size();
	int end_at = text.find("</" + tag_name + ">");
	string tag_text = text.substr(start_at, end_at - start_at);
	text = text.substr(end_at + 3 + tag_name.size());
	return tag_text;
}

//return a trimmed string (no leading nor trailing white spaces)
string trim_str(string str)
{
	int start = str.find_first_not_of(" \t\n");
	int end = str.find_last_not_of(" \t\n");
	if (start == -1 || end == -1)return "";
	return str.substr(start, end - start + 1);
}

//network constructor
network::network(string& xml_path)
{
	number_of_users = 0;
	//open the xml file
	ifstream file(xml_path);
	//loop through the file and find all users
	while (file)
	{
		//extract user text
		string user_text = get_tagText("user",file);
		//add new user the network
		if (user_text != "")
		{
			users.push_back(user(user_text, number_of_users));
			//increase the number of user in the network
			number_of_users++;
		}
	}
	//close the xml file
	file.close();
	//TODO:sort users by id
}

//user constructor
user::user(string& user_xml,int _index)
{
	index = _index;
	//first search for id
	int start_new_at=0;

	//set id
	{
		//set id
		//extract the id
		//update user_xml to the remaining of the string after searching and finding id
		string id_text = get_tagContent("id", user_xml);
		//convert into int and set it
		id = stoi(id_text);
	}

	//set name
	{
		//set name
		//extract the name
		//update user_xml to the remaining of the string after searching and finding name
		string name_text = get_tagContent("name",user_xml);
		//trim the name and set
		name = trim_str(name_text);
	}

	//set posts
	{
		//search for every post and add it to the user list of posts
		//update user_xml to discard parsed part
		string posts_text = get_tagContent("posts", user_xml);
		//extract every post in the posts tag
		while (posts_text.find("<post>") != -1)
		{
			//search for a post tag and update the posts_text to discard the parsed part
			string post_text = get_tagContent("post", posts_text);
			//create new post and add to user's posts
			posts.push_back(post(post_text));
		}
	}

	//set followers
	{
		//search for every follower (get the followers tag)
		//discard parsed part
		string followers_text = get_tagContent("followers", user_xml);
		//extract every follower in the post tag
		while (followers_text.find("<follower>")!=-1)
		{
			//search for follower tag
			string follower_text = get_tagContent("follower", followers_text);
			string id_text = get_tagContent("id",follower_text);
			followers.push_back(stoi(id_text));
		}
	}
}

//post constructor
post::post(string& post_xml)
{
	{
		string body_text = get_tagContent("body", post_xml);
		body = trim_str(body_text);
	}
	//set topics
	{
		//parse topics
		string topics_text = get_tagContent("topics", post_xml);
		//extract every topic
		while (topics_text.find("<topic>")!=-1)
		{
			string topic_text = get_tagContent("topic", topics_text);
			topics.push_back(trim_str(topic_text));
		}
	}
}

//the first matching post in the user posts
post user::search_posts(string key)
{
	//loop on users posts
	for (post _post : posts)
	{
		//search for the key in the topics
		for (string _topic : _post.topics)
			if (_topic.find(key) != -1)return _post;
		//search for the key in the body of the post
		if (_post.body.find(key) != -1)return _post;
	}
	//return empty
	return post();
}

//find all matching posts in the user posts
vector<post> user::search_posts_all(string key)
{
	vector<post> res;
	for (post _post : posts)
	{
		if (_post.body.find(key) != -1)
		{
			res.push_back(_post);
			continue;
		}
		for (string _topic : _post.topics)
		{
			if (_topic.find(key) != -1)
			{
				res.push_back(_post);
				break;
			}
		}	
	}
	return res;
}

//find the first matching post in the network
pair<int,post> network::search_posts(string key)
{
	key = trim_str(key);
	if (key.empty())return { -1,post() };
	//loop on all users in network
	for (int i = 0; i < users.size(); i++)
	{
		//search each user for the post
		post _post = users[i].search_posts(key);
		//if the user has a post return it
		if (_post.body != "")return { users[i].id,_post};
	}
	//return empty post
	return { -1, post() };
}

//find all matching posts in the network
vector<pair<int, post>> network::search_posts_all(string key)
{
	key = trim_str(key);
	if (key.empty())return vector<pair<int, post>>();
	vector<pair<int, post>> search_res;
	for (int i = 0; i < users.size(); i++)
	{
		vector<post> _posts = users[i].search_posts_all(key);
		if (!_posts.empty())
			for(int j=0;j<_posts.size();j++)
			search_res.push_back({ users[i].id,_posts[j]});
	}
	return search_res;
}

//find the graph index of a user with a specifi id
int network::get_index(int _id)
{
	for (int i = 0; i < users.size(); i++)
	{
		if (users[i].id == _id)
			return users[i].index;
	}
	return -1;
}

//returns the user with a specific id
user* network::get_user(int _id)
{
	int u_index = get_index(_id);
	return &(users[u_index]);
}

//network analysis constructor
networking_analysis::networking_analysis(string& xml_path)
{
	//create network
	my_network = network(xml_path);
	//set graph size
	network_graph = vector<vector<bool>>(my_network.number_of_users, vector<bool>(my_network.number_of_users,false));
	most_influential = user();
	//loop on all users
	for(int i=0;i<my_network.users.size();i++)
	{
		//if the users have more followers set him as the most infuential
		if (my_network.users[i].followers.size() > most_influential.followers.size())
			most_influential = my_network.users[i];
		//set graph value
		for (int j = 0; j < my_network.users[i].followers.size();j++)
			network_graph[my_network.users[i].index][my_network.get_index(my_network.users[i].followers[j])]=true;
	}
	//loop on the graph and count connections
	most_active = user();
	int max_connections = 0;
	for (int i = 0; i < my_network.users.size(); i++)
	{
		int user_connections = 0;
		//loop on all followers in the user
		for (int j = 0; j < my_network.users[i].followers.size(); j++)
			//if the user's follower also follows him back its a connection
			if (network_graph[my_network.users[i].index][my_network.get_index(my_network.users[i].followers[j])]\
				&& network_graph[my_network.get_index(my_network.users[i].followers[j])][my_network.users[i].index])
				user_connections++;
		if (user_connections > max_connections)
		{
			most_active = my_network.users[i];
			max_connections = user_connections;
		}
	}
}

vector<user*> networking_analysis::suggest_followers(int u_id)
{
	vector<user*> suggestion_list;
	//get user index from their id
	int u_index = my_network.get_index(u_id);
	//loop on all followers and add their followers to the suggestion list
	for (int i = 0; i < my_network.users[u_index].followers.size(); i++)
	{
		user follower = *my_network.get_user(my_network.users[u_index].followers[i]);
		for (int j = 0; j < follower.followers.size(); j++)
		{
			if (follower.followers[j] == u_id)continue;
			suggestion_list.push_back(my_network.get_user(follower.followers[j]));
		}
	}
	//remove duplicates
	vector<user*>::iterator ip;
	ip = std::unique(suggestion_list.begin(), suggestion_list.end());
	suggestion_list.resize(distance(suggestion_list.begin(), ip));
	return suggestion_list;
}

vector<user*> networking_analysis::get_mutualFollowers(int u1_id,int u2_id)
{
	vector<user*> mutual_followers;
	int u1_index = my_network.get_index(u1_id);
	int u2_index = my_network.get_index(u2_id);
	//get u1 followers
	for (int i = 0; i < my_network.users[u1_index].followers.size(); i++)
	{
		int follower_index = my_network.get_index(my_network.users[u1_index].followers[i]);
		if (i == u1_index || i == u2_index)continue;
		if (network_graph[u1_index][follower_index] && network_graph[u2_index][follower_index])
			mutual_followers.push_back(&my_network.users[i]);
	}
	return mutual_followers;
}

ostream& operator<< (ostream& out, const post& c)
{
	out << "Post Topics: ";
	for (string topic : c.topics)
		out << topic << ", ";
	out << endl<<"Post Body: ";
	out << c.body<<endl;
	return out;
}
ostream& operator << (ostream& out, const user& c)
{
	out << "USER: network index->" << c.index << endl << "ID: " << c.id << " || Name: " << c.name << endl;
	out << "Number of Followers: " << c.followers.size()<<endl;
	out << "Followers IDs: ";
	for (int _id : c.followers)
		out << _id << ", ";
	out << endl;
	out << "Number of Posts: " << c.posts.size() << endl;
	for (post _post : c.posts)
		out << _post << endl;
	out << endl<<endl;
	return out;
}
ostream& operator << (ostream& out, const networking_analysis& c)
{
	out << "NETWORK:" << endl;
	out << "Number of users: " << c.my_network.number_of_users << endl;
	out << "Most Active User Info:" << endl;
	out << c.most_active << endl;
	out << "Most Influential User Info:" << endl;
	out << c.most_influential << endl << endl;
	out << "Full Network Users:" << endl;
	for (user _user : c.my_network.users)
		out << _user;
	out << endl;
	out << "Network Graph:" << endl;
	for (int i = 0; i < c.my_network.number_of_users; i++)
		out << '\t' << i;
	out << endl;
	for (int i = 0; i < c.my_network.number_of_users; i++)
	{
		out << i << '\t';
		for (int j = 0; i < c.my_network.number_of_users; i++)
			out << c.network_graph[i][j] << '\t';
		out << endl;
	}
	out << endl;
	return out;
}