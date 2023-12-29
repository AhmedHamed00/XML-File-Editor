#include "error_checking_window.h"
#include "ui_error_checking_window.h"
extern bool misspelled_tags_check;
extern bool missing_brackets_check;

error_checking_window::error_checking_window(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::error_checking_window)
{
    ui->setupUi(this);
    error_checking_window::setFixedSize(300,100);
}

error_checking_window::~error_checking_window()
{
    delete ui;
}

void error_checking_window::on_checkBox_stateChanged(int arg1)
{
    if (ui->checkBox->isChecked())
    {
        misspelled_tags_check=1;
    }
    else
    {
        misspelled_tags_check=0;
    }

}


void error_checking_window::on_checkBox_2_stateChanged(int arg1)
{

    if (ui->checkBox_2->isChecked())
    {
        missing_brackets_check=1;
    }
    else
    {
        missing_brackets_check=0;
    }

}


void error_checking_window::on_pushButton_clicked()
{
    error_checking_window::close();
}

