#include "errors_list.h"
#include "ui_errors_list.h"
extern QString error_list_text;
errors_list::errors_list(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::errors_list)
{
    ui->setupUi(this);
    ui->textEdit->setTextColor(Qt::red); //window to show error in color red
    ui->textEdit->setReadOnly(true);
    ui->textEdit->setText(error_list_text);
}

errors_list::~errors_list()
{
    delete ui;
}

void errors_list::on_pushButton_clicked()
{
    errors_list::close();
}

