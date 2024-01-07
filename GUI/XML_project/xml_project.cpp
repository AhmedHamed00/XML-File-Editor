#include "xml_project.h"
#include "./ui_xml_project.h"
#include <QFileDialog>
#include <QFile>
#include <QTextStream>
#include <QMessageBox>
#include <vector>
#include <stack>
#include "json_window2.h"
#include "XmlFile2Json.h"
#include "prettify.h"
#include "Formatting.h"
#include "decompressedfile.h"
#include "error_checking_window.h"
#include "error_handling.h"
#include "errors_list.h"
#include "xml_details.h"
#include "network.h"
typedef enum {
    error_checking,error_correction,
    formatting,original,no_error_detected,minifying,unsolvable_error
} operation;

std::stack <std::pair<QString,operation>> undo_stack; //redo and undo stacks takes pairs to define the current operation and the contents of file
std::stack <std::pair<QString,operation>> redo_stack;

std::pair <QString,operation> undo_redo; //the pair that we will keep pushing or popping from the stack


bool misspelled_tags_check=0; //flags set by checkboxes to be checked on
bool missing_brackets_check=0;


QString text; //text contained in XML file
QString Json_text; //text contained in Json file
QString Decompress_text; //decompressed file contents
QString xml_File; //path of xml file chosen by user
QString error_list_text; //the text of the error list window
std::string xml_file_String ;//path of xml file but in string not QString


XML_project::XML_project(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::XML_project)
{
    /*
     * initialize the buttons to be initially off
     */
    ui->setupUi(this);
    ui->error_check_button->setEnabled(false);
    ui->error_correct_button->setEnabled(false);
    ui->textEdit->setLineWrapMode(QTextEdit::NoWrap);
    ui->format_button->setEnabled(false);
    ui->jason_button->setEnabled(false);
    ui->details_button->setEnabled(false);
    ui->actionundo->setEnabled(false);
    ui->actionRedo->setEnabled(false);
    ui->Minify_button->setEnabled(false);
    ui->actionSave_to_another_location->setEnabled(false);


}

XML_project::~XML_project()
{
    delete ui;
}


void XML_project::on_actionOpen_XML_file_triggered()
{
    QString filter= "XML File (*.xml)" ; //filter inputs to xml only
    xml_File =QFileDialog::getOpenFileName(this,"Choose XML file",QDir::homePath(),filter);
    xml_file_String= xml_File.toStdString();
    QFile file(xml_File);
    ui->actionOpen_XML_file->setEnabled(false); //set buttons states according to what can be done on file now and what cant be done
    ui->actionSave_Text_as_XML_file->setEnabled(false);
    ui->textEdit->setReadOnly(true);
    ui->error_check_button->setEnabled(true);
     ui->actionSave_to_another_location->setEnabled(true);
    if(!file.open(QFile::ReadOnly | QFile::Text)) //if nothing is chosen by user display warning message
    {
        QMessageBox::warning(this,"Error","file not open");
        ui->actionOpen_XML_file->setEnabled(true);
        ui->actionSave_Text_as_XML_file->setEnabled(true);
        ui->textEdit->setReadOnly(false);
        ui->error_check_button->setEnabled(false);
    }
    QTextStream in(&file);
    text = in.readAll();
    ui->textEdit->setText(text); //set the text field to the contents of the file
    file.close();
    undo_redo.first=text; //add text of file to pair
    undo_redo.second=original; //add type of operation to pair
    undo_stack.push(undo_redo); //push it to stack
}


void XML_project::on_actionSave_Text_as_XML_file_triggered()
{
    QString filter= "XML File (*.xml)" ;
    xml_File =QFileDialog::getSaveFileName(this,"Save XML file",QDir::homePath(),filter); //chose where to save the file
    xml_file_String= xml_File.toStdString();
    QFile file(xml_File);
    ui->actionSave_Text_as_XML_file->setEnabled(false);
    ui->actionOpen_XML_file->setEnabled(false);
    ui->textEdit->setReadOnly(true);
    ui->error_check_button->setEnabled(true);
     ui->actionSave_to_another_location->setEnabled(true);
    if(!file.open(QFile::WriteOnly | QFile::Text)) //if no location chosen display warning message
    {
        QMessageBox::warning(this,"Error","No location was chosen");
        ui->actionSave_Text_as_XML_file->setEnabled(true);
        ui->actionOpen_XML_file->setEnabled(true);
        ui->textEdit->setReadOnly(false);
        ui->error_check_button->setEnabled(false);
    }
    QTextStream out(&file);
    text = ui->textEdit->toPlainText(); //take text from textfield and insert it in the File
    out << text;
    file.flush();
    file.close();
    undo_redo.first=text;   //push contents into the undo stack
    undo_redo.second=original;
    undo_stack.push(undo_redo);

}


void XML_project::on_actionClear_triggered() //reset entire app (can take a new XML file now)
{
    ui->textEdit->clear();
    ui->textEdit->setReadOnly(false);
    ui->textEdit->setTextColor(Qt::black); //reset the textEdit
    ui->actionSave_Text_as_XML_file->setEnabled(true); //enable entering new XML files
    ui->actionOpen_XML_file->setEnabled(true);
    ui->actionSave_to_another_location->setEnabled(false);
    while(!undo_stack.empty()) //empty my stacks
    {
        undo_stack.pop();
    }
    while(!redo_stack.empty())
    {
        redo_stack.pop();
    }
    /*
     * deInitialize all buttons
     */
    ui->actionundo->setEnabled(false);
    ui->actionRedo->setEnabled(false);
    ui->error_check_button->setEnabled(false);
    ui->error_correct_button->setEnabled(false);

    ui->format_button->setEnabled(false);

    ui->jason_button->setEnabled(false);
    ui->details_button->setEnabled(false);
    ui->Minify_button->setEnabled(false);

}

void XML_project::color_errors() //function only to color the errors in the XML file
{
    QTextCursor cursor = ui->textEdit->textCursor();
    for(int j=0;j<error_list.size();j++)
    {
        // QTextCursor cursor2 = ui->textEdit->textCursor();
        // cursor2.movePosition(QTextCursor::Start);
        // cursor2.movePosition(QTextCursor::Down, QTextCursor::MoveAnchor, error_list[j].line);
        // cursor2.movePosition(QTextCursor::EndOfLine);
        // ui->textEdit->setTextCursor(cursor2);
        cursor.movePosition(QTextCursor::Start);
        for (int i=0 ;i<(error_list[j].line);i++)
        {
            cursor.movePosition(QTextCursor::Down);
        }
        cursor.movePosition(QTextCursor::EndOfLine);
        ui->textEdit->setTextCursor(cursor);
        ui->textEdit->setTextColor(Qt::red);
        ui->textEdit->insertPlainText(QString::fromStdString(error_list[j].msg));
    }
        ui->textEdit->setTextColor(Qt::black);
}
void XML_project::write_on_file() //new for undo , redo
{
    QFile file(xml_File);
    if(!file.open(QFile::WriteOnly | QFile::Text)) //if no location chosen display warning message
    {

    }
    QTextStream out(&file);
    out << text;
    file.flush();
    file.close();
}
void XML_project::write_on_textEdit_from_file() //take text of xml and type it on text edit
{
    QFile file2(xml_File);

    if(!file2.open(QFile::ReadOnly | QFile::Text))
    {

    }
    else
    {
        QTextStream in(&file2);
        text = in.readAll();
        ui->textEdit->setText(text);
    }
}
void XML_project::on_error_check_button_clicked()
{
    std::uint8_t success=0;
    error_checking_window error_wind;
    error_wind.setModal(true);
    error_wind.exec();
    find_errors(xml_file_String,success, missing_brackets_check,misspelled_tags_check);
    if(error_list.size()==0)
    {
         QMessageBox::information(this,"","No Errors were found");
        ui->error_check_button->setEnabled(false);
        ui->error_correct_button->setEnabled(false);
        ui->actionundo->setEnabled(true);
        ui->jason_button->setEnabled(true);
        ui->format_button->setEnabled(true);
        ui->details_button->setEnabled(true);
        ui->Minify_button->setEnabled(true);
        undo_redo.first=text;
        undo_redo.second=no_error_detected;
        undo_stack.push(undo_redo);
        do_error_action();
    }
    else
    {
        string str;
        stringstream s;
        for(auto a:error_list)
        {
            s << a << std::endl <<std::endl;
        }
        str = s.str();
        error_list_text=QString::fromStdString(str);
        errors_list error_window;
        error_window.setModal(true);
        error_window.exec();
        if(success==0 && missing_brackets_check)
        {
            QMessageBox::warning(this,"Error","Missing Brackets were Found please correct the file and try again");
        }
        color_errors();
        ui->error_check_button->setEnabled(false);
        ui->error_correct_button->setEnabled(true);
        ui->actionundo->setEnabled(true);
        undo_redo.first=text;
        undo_redo.second=error_checking;
        undo_stack.push(undo_redo);
        do_error_action();
    }

}


void XML_project::on_error_correct_button_clicked()
{
    while(!redo_stack.empty())
    {
        redo_stack.pop();
    }
    ui->actionRedo->setEnabled(false);
    ui->actionundo->setEnabled(true);
    bool error_correction_status=solve_errors();
    if(error_correction_status==0)
    {
        QMessageBox::warning(this,"Error","Unsolvable Errors were Found please correct the file and try again");
        ui->error_correct_button->setEnabled(false);
        undo_redo.first=text;
        undo_redo.second=unsolvable_error;
        undo_stack.push(undo_redo);
    }
    else
    {
        write_on_textEdit_from_file();
        undo_redo.first=text;
        undo_redo.second=error_correction;
        undo_stack.push(undo_redo);
        ui->error_correct_button->setEnabled(false);
        ui->jason_button->setEnabled(true);
        ui->format_button->setEnabled(true);
        ui->details_button->setEnabled(true);
        ui->Minify_button->setEnabled(true);
    }
}


void XML_project::on_jason_button_clicked()
{
    int flag=1;
    QString filter= "Json File (*.json)" ;
    QString Json_path;
    Json_path =QFileDialog::getSaveFileName(this,"Choose Json file save location",QDir::homePath(),filter);
    QFile file(Json_path);

    if(!file.open(QFile::WriteOnly | QFile::Text))
    {
        flag=0;
        QMessageBox::warning(this,"Error","No location was chosen");
    }
    else
    {

    }
    file.close();

    std::string Json_file_String = Json_path.toStdString();

    xmlFileToJson(xml_file_String,Json_file_String);

    QFile file2(Json_path); //take text of json file to display it on TextEdit

    if(!file2.open(QFile::ReadOnly | QFile::Text))
    {

    }
    else
    {
        QTextStream in(&file2);
        Json_text= in.readAll();
    }

    if(flag==1)
    {
        json_window2 json_file;
        json_file.setModal(true);
        json_file.exec();
    }
    file2.close();

}


void XML_project::on_format_button_clicked()
{
    while(!redo_stack.empty())
    {
        redo_stack.pop();
    }
    ui->actionRedo->setEnabled(false);
    ui->actionundo->setEnabled(true);
    ui->format_button->setEnabled(true);
    ui->Minify_button->setEnabled(true);
    separateTags(xml_file_String, "prettify1.xml");
    prettifyXML("prettify1.xml", xml_file_String);
    write_on_textEdit_from_file();
    undo_redo.first=text;
    undo_redo.second=formatting;
    undo_stack.push(undo_redo);
    do_error_action();
}





void XML_project::on_details_button_clicked()
{

    xml_details network;
    network.setModal(true);
    network.exec();
}


void XML_project::on_actionundo_triggered()
{
    if(undo_stack.top().second==error_checking)
    {

        ui->error_check_button->setEnabled(false);
        ui->error_correct_button->setEnabled(false);
    }
    else if(undo_stack.top().second==error_correction)
    {
        ui->error_check_button->setEnabled(false);
        ui->error_correct_button->setEnabled(true);
        ui->format_button->setEnabled(false);
        ui->details_button->setEnabled(false);
        ui->jason_button->setEnabled(false);
        ui->Minify_button->setEnabled(false);
    }
    else if(undo_stack.top().second==unsolvable_error)
    {
        ui->error_correct_button->setEnabled(true);
    }
    else if(undo_stack.top().second==formatting)
    {
        ui->format_button->setEnabled(true);
        ui->Minify_button->setEnabled(true);
    }
    else if(undo_stack.top().second==no_error_detected)
    {
        ui->error_check_button->setEnabled(true);
        ui->format_button->setEnabled(false);
        ui->details_button->setEnabled(false);
        ui->jason_button->setEnabled(false);
        ui->Minify_button->setEnabled(false);
    }
    else if(undo_stack.top().second==minifying)
    {
        ui->Minify_button->setEnabled(true);
        ui->format_button->setEnabled(true);
    }
    redo_stack.push(undo_stack.top());
    undo_stack.pop();
    text=undo_stack.top().first;
    ui->actionRedo->setEnabled(true);
    ui->textEdit->setText(text);
    write_on_file();
    undo_error_correction();
    if(undo_stack.top().second==error_checking)
    {
        color_errors();
    }
    else if(undo_stack.top().second==original)
    {
        ui->error_check_button->setEnabled(true);
    }


    if(undo_stack.size()==1)
    {
        ui->actionundo->setEnabled(false);
    }
}


void XML_project::on_actionRedo_triggered()

{

    if(redo_stack.top().second==error_checking)
    {
       ui->error_correct_button->setEnabled(true);
       ui->error_check_button->setEnabled(false);
    }
    else if(redo_stack.top().second==error_correction)
    {

        ui->error_correct_button->setEnabled(false);
        ui->format_button->setEnabled(true);
        ui->details_button->setEnabled(true);
        ui->jason_button->setEnabled(true);
        ui->Minify_button->setEnabled(true);
    }
    else if(redo_stack.top().second==unsolvable_error)
    {
        ui->error_correct_button->setEnabled(false);
    }


    else if(redo_stack.top().second==formatting)
    {
        ui->format_button->setEnabled(true);
        ui->Minify_button->setEnabled(true);
    }
    else if(redo_stack.top().second==no_error_detected)
    {
        ui->error_check_button->setEnabled(false);
        ui->error_correct_button->setEnabled(false);
        ui->format_button->setEnabled(true);
        ui->details_button->setEnabled(true);
        ui->jason_button->setEnabled(true);
        ui->Minify_button->setEnabled(true);

    }
    else if(redo_stack.top().second==minifying)
    {
        ui->format_button->setEnabled(true);
        ui->Minify_button->setEnabled(true);
    }

    undo_stack.push(redo_stack.top());
    redo_stack.pop();
    text=undo_stack.top().first;
    ui->actionundo->setEnabled(true);
    ui->textEdit->setText(text);
    write_on_file();
    do_error_action();
    if(undo_stack.top().second==error_checking)
    {
        color_errors();

    }
    if(redo_stack.empty()==true)
    {
        ui->actionRedo->setEnabled(false);
    }

}




void XML_project::on_actionDecompress_triggered()
{
    QString decompress_path;
    std::string decompress_path_string;
    QString decompress_path_tree;
    std::string decompress_path_tree_string;
    QString decompress_path_tree_coded_output;
    std::string decompress_path_tree_coded_output_string;


    QString filter= "XML File (*.xml) ;; JSON File (*.json)" ; //filter inputs to xml , jsononly
    decompress_path =QFileDialog::getSaveFileName(this,"Choose Location to save Decompressed XML or JSON file",QDir::homePath(),filter);
    decompress_path_string= decompress_path.toStdString();
    QFile file(decompress_path);
    if(!file.open(QFile::WriteOnly | QFile::Text)) //if nothing is chosen by user display warning message
    {
        QMessageBox::warning(this,"Error","No location was chosen for Decompressed file");
    }
    else
    {
        file.close();

        QString filter_tree= "Text File (*.txt)" ;
        decompress_path_tree =QFileDialog::getOpenFileName(this,"Choose The Hoffman tree File ",QDir::homePath(),filter_tree);
        decompress_path_tree_string= decompress_path_tree.toStdString();
        QFile file2(decompress_path_tree);
        if(!file2.open(QFile::ReadOnly | QFile::Text)) //if nothing is chosen by user display warning message
        {
            QMessageBox::warning(this,"Error","No Hoffman tree file was chosen");
        }
        else
        {
            file2.close();
            QString filter_tree_output_coded= "Text File (*.txt)" ; //filter inputs to xml only
            decompress_path_tree_coded_output =QFileDialog::getOpenFileName(this,"Choose The Hoffman Tree coded output location ",QDir::homePath(),filter_tree_output_coded);
            decompress_path_tree_coded_output_string= decompress_path_tree_coded_output.toStdString();
            QFile file3(decompress_path_tree_coded_output);
            if(!file3.open(QFile::ReadOnly | QFile::Text)) //if nothing is chosen by user display warning message
            {
                QMessageBox::warning(this,"Error","No Hoffman tree coded output file was chosen");
            }
            else
            {
                file3.close();
                Decompress_Hoffman_Coding(decompress_path_string,decompress_path_tree_string,decompress_path_tree_coded_output_string);
                QFile file4(decompress_path); //take text of json file to display it on TextEdit

                if(!file4.open(QFile::ReadOnly | QFile::Text))
                {

                }
                else
                {
                    QTextStream in(&file4);
                    Decompress_text= in.readAll();
                    file4.close();
                    Decompressedfile decompress_window;
                    decompress_window.setModal(true);
                    decompress_window.exec();
                }



            }

        }

    }
}


void XML_project::on_actionCompress_triggered()
{
    QString compress_path;
    std::string compress_path_string;
    QString compress_path_tree;
    std::string compress_path_tree_string;
    QString compress_path_tree_coded_output;
    std::string compress_path_tree_coded_output_string;


    QString filter= "XML File (*.xml);; JSON File (*.json)" ; //filter inputs to xml only
    compress_path =QFileDialog::getOpenFileName(this,"Choose XML or JSON file to Compress",QDir::homePath(),filter);
    compress_path_string= compress_path.toStdString();
    QFile file(compress_path);
    if(!file.open(QFile::ReadOnly | QFile::Text)) //if nothing is chosen by user display warning message
    {
        QMessageBox::warning(this,"Error","No file was chosen for Compressing");
    }
    else
    {
        file.close();

        QString filter_tree= "Text File (*.txt)" ; //filter inputs to xml only
        compress_path_tree =QFileDialog::getSaveFileName(this,"Choose The Hoffman Tree location ",QDir::homePath(),filter_tree);
        compress_path_tree_string= compress_path_tree.toStdString();
        QFile file2(compress_path_tree);
        if(!file2.open(QFile::WriteOnly | QFile::Text)) //if nothing is chosen by user display warning message
        {
            QMessageBox::warning(this,"Error","No Hoffman tree file location was chosen");
        }
        else
        {
            file2.close();
            QString filter_tree_output_coded= "Text File (*.txt)" ; //filter inputs to xml only
            compress_path_tree_coded_output =QFileDialog::getSaveFileName(this,"Choose The Hoffman Tree coded output location ",QDir::homePath(),filter_tree_output_coded);
            compress_path_tree_coded_output_string= compress_path_tree_coded_output.toStdString();
            QFile file3(compress_path_tree_coded_output);
            if(!file3.open(QFile::WriteOnly | QFile::Text)) //if nothing is chosen by user display warning message
            {
                QMessageBox::warning(this,"Error","No Hoffman tree coded output file location was chosen");
            }
            else
            {
                file3.close();
                long long bytes_size;
                QString message;
                bytes_size=Compress_Using_Hoffman_Coding(compress_path_string,compress_path_tree_string,compress_path_tree_coded_output_string);
                message= "Size of file after compression is : " + QString::number(bytes_size) + " bytes";
                QMessageBox::information(this,"Compression Message", message);

            }

        }

    }


}


void XML_project::on_Minify_button_clicked()
{
    while(!redo_stack.empty())
    {
        redo_stack.pop();
    }
    ui->Minify_button->setEnabled(true);
    ui->format_button->setEnabled(true);
    _Minify(xml_file_String);
    write_on_textEdit_from_file();
    undo_redo.first=text;
    undo_redo.second=minifying;
    undo_stack.push(undo_redo);
    do_error_action();
}


void XML_project::on_actionSave_to_another_location_triggered()
{
    QString xml_File_new_location;
    QString filter= "XML File (*.xml)" ;
    xml_File_new_location =QFileDialog::getSaveFileName(this,"Save XML file",QDir::homePath(),filter); //chose where to save the file
    QFile file(xml_File_new_location);
    if(!file.open(QFile::WriteOnly | QFile::Text)) //if no location chosen display warning message
    {
        QMessageBox::warning(this,"Error","No location was chosen");
    }
    else
    {
        QTextStream out(&file);
        out << text;
        file.flush();
        file.close();
    }

}

