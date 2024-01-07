#include "decompressedfile.h"
#include "ui_decompressedfile.h"
extern QString Decompress_text;
Decompressedfile::Decompressedfile(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::Decompressedfile)
{
    ui->setupUi(this);
    ui->textEdit3->setText(Decompress_text); //show the decompressed xml in a new window
    ui->textEdit3->setReadOnly(true);
}

Decompressedfile::~Decompressedfile()
{
    delete ui;
}

void Decompressedfile::on_pushButton_clicked()
{
   Decompressedfile::close();
}

