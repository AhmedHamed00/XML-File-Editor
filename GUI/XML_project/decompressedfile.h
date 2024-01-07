#ifndef DECOMPRESSEDFILE_H
#define DECOMPRESSEDFILE_H

#include <QDialog>

namespace Ui {
class Decompressedfile;
}

class Decompressedfile : public QDialog
{
    Q_OBJECT

public:
    explicit Decompressedfile(QWidget *parent = nullptr);
    ~Decompressedfile();

private slots:
    void on_pushButton_clicked();

private:
    Ui::Decompressedfile *ui;
};

#endif // DECOMPRESSEDFILE_H
