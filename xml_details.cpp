#include "xml_details.h"
#include "ui_xml_details.h"
#include "network.h"
#include "graph_vis.h"
#include <QFile>
#include <QMessageBox>
#include <string>
extern std::string xml_file_String;

networking_analysis users_graph(xml_file_String);

xml_details::xml_details(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::xml_details)
{
    ui->setupUi(this);
    ui->textEdit->setReadOnly(true);
    ui->lineEdit_influencer->setReadOnly(true);
    ui->lineEdit_active->setReadOnly(true);
    users_graph = networking_analysis(xml_file_String);
    ui->comboBox_mutual_1->addItem("Select a User");
    ui->comboBox_mutual_2->addItem("Select a User");
    ui->comboBox_suggestions->addItem("Select a User");
    ui->lineEdit_active->setText(QString::fromStdString(users_graph.most_active.name));
    ui->lineEdit_influencer->setText(QString::fromStdString(users_graph.most_influential.name));
    for(int i=0;i<users_graph.my_network.users.size();i++)
    {
        ui->comboBox_mutual_1->addItem(QString::number(users_graph.my_network.users[i].id));
        ui->comboBox_mutual_2->addItem(QString::number(users_graph.my_network.users[i].id));
        ui->comboBox_suggestions->addItem(QString::number(users_graph.my_network.users[i].id));
    }
}

xml_details::~xml_details()
{
    delete ui;
}

void xml_details::on_mutual_button_pressed()
{
    if(ui->comboBox_mutual_1->currentText()=="Select a User" ||ui->comboBox_mutual_2->currentText()=="Select a User" )
    {
         QMessageBox::warning(this,"Error","Please select a user first");
    }
    else
    {
        QString show_mutual ="Mutual followers between user "+ui->comboBox_mutual_1->currentText()+" & user "+ui->comboBox_mutual_2->currentText() +" : \n\n";
        vector <user*> mutual_friends;
        mutual_friends=users_graph.get_mutualFollowers(stoi(((ui->comboBox_mutual_1->currentText()).toStdString())),stoi(((ui->comboBox_mutual_2->currentText()).toStdString())));
        for(int i=0;i<mutual_friends.size();i++)
        {
            show_mutual+=QString::fromStdString((*mutual_friends[i]).name) + ", id: "+QString::number((*mutual_friends[i]).id)+"\n";
        }
        if(mutual_friends.size()==0)
        {
            show_mutual+=" none";
        }
        ui->textEdit->setText(show_mutual);
    }
}


void xml_details::on_suggest_button_clicked()
{
    if(ui->comboBox_suggestions->currentText()=="Select a User" )
    {
        QMessageBox::warning(this,"Error","Please select a user first");
    }
    else
    {
        QString show_suggested="Suggested followers for user "+ui->comboBox_suggestions->currentText()+" : \n\n";
        vector <user*> suggested_friends;
        suggested_friends=users_graph.suggest_followers(stoi(((ui->comboBox_suggestions->currentText().toStdString()))));
        for(int i=0;i<suggested_friends.size();i++)
        {
            show_suggested+=QString::fromStdString((*suggested_friends[i]).name) + ", id: "+QString::number((*suggested_friends[i]).id)+"\n";
        }
        if(suggested_friends.size()==0)
        {
            show_suggested+=" none";
        }
        ui->textEdit->setText(show_suggested);

    }
}


void xml_details::on_search_nutton_clicked()
{
    if(ui->lineEdit_search->text()=="")
    {

    }
    else
    {

        if(ui->checkBox->isChecked())
        {
            QString line_Edit_text=ui->lineEdit_search->text();
            pair<int,post> found_post=users_graph.my_network.search_posts(line_Edit_text.toStdString());
            if(found_post.second.body=="")
            {
                QMessageBox::warning(this,"Error","No matching post was found");
                ui->lineEdit_search->clear();
            }
            else
            {
                QString current_post="Post By User: "+QString::number(found_post.first) +"\n" ;
                current_post+= "Topics: ";
                for(int i=0;i<found_post.second.topics.size();i++)
                {
                    current_post+=QString::fromStdString(found_post.second.topics[i]);
                    if(i<found_post.second.topics.size()-1)current_post+=", ";
                }
                current_post+="\n";
                current_post+=QString::fromStdString(found_post.second.body);
                ui->textEdit->setText(current_post);
                ui->lineEdit_search->clear();
            }


        }
        else
        {
            QString line_Edit_text=ui->lineEdit_search->text();
            vector<pair<int,post>> found_posts =users_graph.my_network.search_posts_all(line_Edit_text.toStdString());
            if(found_posts.empty())
            {
                QMessageBox::warning(this,"Error","No matching post was found");
                ui->lineEdit_search->clear();
            }
            else
            {
                QString current_post="Found "+QString::number(found_posts.size())+" posts\n\n\n";
                for(auto found_post : found_posts)
                {
                    current_post+="Post By User: "+QString::number(found_post.first) +"\n" ;
                    current_post+= "Topics: ";
                    for(int i=0;i<found_post.second.topics.size();i++)
                    {
                        current_post+=QString::fromStdString(found_post.second.topics[i]);
                        if(i<found_post.second.topics.size()-1)current_post+=", ";
                    }
                    current_post+="\n";
                    current_post+=QString::fromStdString(found_post.second.body)+"\n\n";
                }
                ui->textEdit->setText(current_post);
                ui->lineEdit_search->clear();
            }
        }

    }
}


void xml_details::on_pushButton_clicked()
{
        QString graph_details;
         graph_details="digraph test {";
         for(int i=0;i<users_graph.my_network.users.size();i++)
         {
           graph_details+="{" +QString::number(users_graph.my_network.users[i].id) + "}";
             if (users_graph.my_network.users[i].followers.size())
           {
              graph_details+="->";
              graph_details+="{";
              for(int j=0;j<users_graph.my_network.users[i].followers.size();j++)
              {
                  graph_details+= QString::number(users_graph.my_network.users[i].followers[j]);
                  if(j!=users_graph.my_network.users[i].followers.size()-1)
                  {
                      graph_details += ",";
                  }

                  else if( j==users_graph.my_network.users[i].followers.size()-1)
                  {
                      graph_details+="}";
                  }
              }
           }
         }
         graph_details+="}";

        QFile file("graph.dot");

        if(!file.open(QFile::WriteOnly | QFile::Text))
        {

        }
        else
        {
            QTextStream out(&file);
            out << graph_details;
            file.flush();
            file.close();
            system("dot -Tpng -O graph.dot");
            Graph_vis new_graph;
            new_graph.setModal(true);
            new_graph.exec();

        }
}

