#include "xml_application.h"
#include "./ui_xml_application.h"
#include <QFileDialog>
#include <QFile>
#include <QTextStream>
#include <QMessageBox>
#include <vector>
QString text;
XML_Application::XML_Application(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::XML_Application)
{
    ui->setupUi(this);
}

XML_Application::~XML_Application()
{
    delete ui;
}

void XML_Application::on_actionOpen_XML_file_triggered()
{

    QString filter= "XML File (*.xml)" ;
    QString xml_File =QFileDialog::getOpenFileName(this,"Choose XML file",QDir::homePath(),filter);
    QFile file(xml_File);
    if(!file.open(QFile::ReadOnly | QFile::Text))
    {
        QMessageBox::warning(this,"Error","file not open");
    }
    QTextStream in(&file);
    text = in.readAll();
    ui->textEdit->setText(text);
    file.close();
    ui->textEdit->setReadOnly(true);
}



void XML_Application::on_actionAdd_XML_by_Text_triggered()
{
    QString filter= "XML File (*.xml)" ;
    QString xml_File =QFileDialog::getSaveFileName(this,"Save XML file",QDir::homePath(),filter);
    QFile file(xml_File);
    if(!file.open(QFile::WriteOnly | QFile::Text))
    {
        QMessageBox::warning(this,"Error","file not open");
    }
    QTextStream out(&file);
     text = ui->textEdit->toPlainText(); //take text from textfield
    out << text;
     ui->textEdit->setReadOnly(true);
    file.flush();
    file.close();
}


void XML_Application::on_actionQuit_triggered()
{
    ui->textEdit->clear();
    ui->textEdit->setReadOnly(false);
    ui->textEdit->setTextColor(Qt::black);
}


void XML_Application::on_pushButton_clicked()
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
/*
 *   by text replacement (not used)
 *
 *  text.replace("","");
 *  text.replace("***","<------- ERROR");
 *  ui->textEdit->setPlainText(text);
 *
 */

/*
    try to compress

    text.replace("\n","");
    text.replace(" ","");
    ui->textEdit->setText(text);
*/

}

