#include "json.h"
#include "ui_json.h"

Json::Json(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::Json)
{
    ui->setupUi(this);
}

Json::~Json()
{
    delete ui;
}

void Json::on_pushButton_clicked()
{
    Json::close();
}

