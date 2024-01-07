#include "json_window2.h"
#include "ui_json_window2.h"
extern QString Json_text;
json_window2::json_window2(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::json_window2)
{
    ui->setupUi(this);
    ui->textEdit2->setText(Json_text);
    ui->textEdit2->setReadOnly(true);
}

json_window2::~json_window2()
{
    delete ui;
}

void json_window2::on_pushButton_clicked()
{
    json_window2::close();
}

