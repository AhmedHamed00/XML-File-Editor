#include "xml_project.h"
#include "./ui_xml_project.h"
#include <QFileDialog>
#include <QFile>
#include <QTextStream>
#include <QMessageBox>
#include <vector>
#include <stack>

#define ALL_OPERATIONS 5
typedef enum {
    error_checking,error_correction,
    formatting, compressing,decompressing,original
} operation;

std::stack <std::pair<QString,operation>> undo_stack;
std::stack <std::pair<QString,operation>> redo_stack;
std::pair <QString,operation> undo_redo;



QString text; //text contained in file
QString xml_File; //path of file chosen by user

XML_project::XML_project(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::XML_project)
{
    ui->setupUi(this);
    ui->error_check_button->setEnabled(false);
    ui->error_correct_button->setEnabled(false);
    ui->compress_button->setEnabled(false);
    ui->format_button->setEnabled(false);
    ui->decompress_button->setEnabled(false);
    ui->jason_button->setEnabled(false);
    ui->details_button->setEnabled(false);
    ui->graph_button->setEnabled(false);
    ui->actionundo->setEnabled(false);
    ui->actionRedo->setEnabled(false);
}

XML_project::~XML_project()
{
    delete ui;
}

void XML_project::on_actionOpen_XML_file_triggered()
{
    QString filter= "XML File (*.xml)" ;
    xml_File =QFileDialog::getOpenFileName(this,"Choose XML file",QDir::homePath(),filter);
    QFile file(xml_File);
    ui->actionOpen_XML_file->setEnabled(false);
    ui->actionSave_Text_as_XML_file->setEnabled(false);
    ui->textEdit->setReadOnly(true);
    ui->error_check_button->setEnabled(true);
    if(!file.open(QFile::ReadOnly | QFile::Text))
    {
        QMessageBox::warning(this,"Error","file not open");
        ui->actionOpen_XML_file->setEnabled(true);
        ui->actionSave_Text_as_XML_file->setEnabled(true);
        ui->textEdit->setReadOnly(false);
        //
        ui->error_check_button->setEnabled(false);
    }
    QTextStream in(&file);
    text = in.readAll();
    ui->textEdit->setText(text);
    file.close();
    undo_redo.first=text;
    undo_redo.second=original;
    undo_stack.push(undo_redo);

}


void XML_project::on_actionSave_Text_as_XML_file_triggered()
{
    QString filter= "XML File (*.xml)" ;
    xml_File =QFileDialog::getSaveFileName(this,"Save XML file",QDir::homePath(),filter);
    QFile file(xml_File);
    ui->actionSave_Text_as_XML_file->setEnabled(false);
    ui->actionOpen_XML_file->setEnabled(false);
    ui->textEdit->setReadOnly(true);
    ui->error_check_button->setEnabled(true);
    if(!file.open(QFile::WriteOnly | QFile::Text))
    {
        QMessageBox::warning(this,"Error","file not open");
        ui->actionSave_Text_as_XML_file->setEnabled(true);
        ui->actionOpen_XML_file->setEnabled(true);
        //
        ui->textEdit->setReadOnly(false);
        ui->error_check_button->setEnabled(false);
    }
    QTextStream out(&file);
    text = ui->textEdit->toPlainText(); //take text from textfield
    out << text;
    file.flush();
    file.close();
    undo_redo.first=text;
    undo_redo.second=original;
    undo_stack.push(undo_redo);

}


void XML_project::on_actionClear_triggered()
{
    ui->textEdit->clear();
    ui->textEdit->setReadOnly(false);
    ui->textEdit->setTextColor(Qt::black);
    ui->actionSave_Text_as_XML_file->setEnabled(true);
    ui->actionOpen_XML_file->setEnabled(true);
    while(!undo_stack.empty())
    {
        undo_stack.pop();
    }
    while(!redo_stack.empty())
    {
        redo_stack.pop();
    }
    ui->actionundo->setEnabled(false);
    ui->actionRedo->setEnabled(false);
    ui->error_check_button->setEnabled(false);
    ui->error_correct_button->setEnabled(false);
    ui->compress_button->setEnabled(false);
    ui->format_button->setEnabled(false);
    ui->decompress_button->setEnabled(false);
    ui->jason_button->setEnabled(false);
    ui->details_button->setEnabled(false);
    ui->graph_button->setEnabled(false);

}

void XML_project::color_errors()
{
    QTextCursor cursor = ui->textEdit->textCursor();
    std:: vector<int>error_lines={3,6,7};
    for(int j=0;j<error_lines.size();j++)
    {
        cursor.movePosition(QTextCursor::Start);
        for (int i=0 ;i<(error_lines[j]-1);i++)
        {
            cursor.movePosition(QTextCursor::Down);
        }
        cursor.movePosition(QTextCursor::EndOfLine);
        ui->textEdit->setTextCursor(cursor);
        ui->textEdit->setTextColor(Qt::red);
        ui->textEdit->insertPlainText("  <--------- ERROR");
    }
        ui->textEdit->setTextColor(Qt::black);
}

void XML_project::on_error_check_button_clicked()
{

    color_errors();
    ui->error_check_button->setEnabled(false);
    ui->error_correct_button->setEnabled(true);
    ui->actionundo->setEnabled(true);
    undo_redo.first=text;
    undo_redo.second=error_checking;
    undo_stack.push(undo_redo);

}


void XML_project::on_error_correct_button_clicked()
{

    if(!redo_stack.empty())
    {
        undo_stack.pop();
        while(!redo_stack.empty())
        {
            undo_stack.push(redo_stack.top());
            redo_stack.pop();
        }
    }
    ui->actionRedo->setEnabled(false);
    ui->actionundo->setEnabled(true);
    text="error correcting";
    undo_redo.first=text;
    undo_redo.second=error_correction;
    undo_stack.push(undo_redo);
    ui->error_correct_button->setEnabled(false);
    ui->textEdit->setPlainText(text);
    ui->jason_button->setEnabled(true);
    ui->compress_button->setEnabled(true);
    ui->graph_button->setEnabled(true);
    ui->format_button->setEnabled(true);


}


void XML_project::on_jason_button_clicked()
{

}


void XML_project::on_format_button_clicked()
{
    if(!redo_stack.empty())
    {
        undo_stack.pop();
        while(!redo_stack.empty())
        {
            undo_stack.push(redo_stack.top());
            redo_stack.pop();
        }
    }
    ui->actionRedo->setEnabled(false);
    ui->actionundo->setEnabled(true);
    ui->format_button->setEnabled(false);
    text="formatting";
    undo_redo.first=text;
    undo_redo.second=formatting;
    undo_stack.push(undo_redo);
    ui->textEdit->setPlainText(text);
}


void XML_project::on_compress_button_clicked()
{
    if(!redo_stack.empty())
    {
        undo_stack.pop();
        while(!redo_stack.empty())
        {
            undo_stack.push(redo_stack.top());
            redo_stack.pop();
        }
    }
    ui->compress_button->setEnabled(false);
    ui->decompress_button->setEnabled(true);
    ui->actionRedo->setEnabled(false);
    ui->actionundo->setEnabled(true);

    text="compressing";
    undo_redo.first=text;
    undo_redo.second=compressing;
    undo_stack.push(undo_redo);
    ui->textEdit->setPlainText(text);

}


void XML_project::on_decompress_button_clicked()
{
    if(!redo_stack.empty())
    {
        undo_stack.pop();
        while(!redo_stack.empty())
        {
            undo_stack.push(redo_stack.top());
            redo_stack.pop();
        }
    }
    ui->compress_button->setEnabled(true);
    ui->decompress_button->setEnabled(false);
    ui->actionRedo->setEnabled(false);
    ui->actionundo->setEnabled(true);

    text="decompressing";
    undo_redo.first=text;
    undo_redo.second=decompressing;
    undo_stack.push(undo_redo);
    ui->textEdit->setPlainText(text);

}


void XML_project::on_graph_button_clicked()
{

}


void XML_project::on_details_button_clicked()
{

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
        ui->decompress_button->setEnabled(false);
        ui->compress_button->setEnabled(false);
        ui->format_button->setEnabled(false);
        ui->graph_button->setEnabled(false);
        ui->details_button->setEnabled(false);
    }
    else if(undo_stack.top().second==compressing)
    {
        ui->decompress_button->setEnabled(false);
        ui->compress_button->setEnabled(true);
    }
    else if(undo_stack.top().second==decompressing)
    {
        ui->decompress_button->setEnabled(true);
        ui->compress_button->setEnabled(false);
    }
    else if(undo_stack.top().second==formatting)
    {
        ui->format_button->setEnabled(true);
    }

    redo_stack.push(undo_stack.top());
    undo_stack.pop();
    text=undo_stack.top().first;
    ui->actionRedo->setEnabled(true);
    ui->textEdit->setText(text);
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
        ui->decompress_button->setEnabled(true);
        ui->compress_button->setEnabled(true);
        ui->format_button->setEnabled(true);
        ui->graph_button->setEnabled(true);
        ui->details_button->setEnabled(true);
    }

    else if(redo_stack.top().second==compressing)
    {
        ui->decompress_button->setEnabled(true);
        ui->compress_button->setEnabled(false);
    }
    else if(redo_stack.top().second==decompressing)
    {
        ui->decompress_button->setEnabled(false);
        ui->compress_button->setEnabled(true);
    }
    else if(redo_stack.top().second==formatting)
    {
        ui->format_button->setEnabled(false);
    }

    undo_stack.push(redo_stack.top());
    redo_stack.pop();
    text=undo_stack.top().first;
    ui->actionundo->setEnabled(true);
    ui->textEdit->setText(text);
    if(undo_stack.top().second==error_checking)
    {
        color_errors();

    }
    if(redo_stack.empty()==true)
    {
        ui->actionRedo->setEnabled(false);
    }
}

