#include "json_window.h"
#include "ui_json_window.h"

jason_window::jason_window(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::jason_window)
{
    ui->setupUi(this);
    ui->textEdit->setReadOnly(true);
}

jason_window::~jason_window()
{
    delete ui;
}

void jason_window::on_pushButton_clicked()
{
    jason_window::close();
}
