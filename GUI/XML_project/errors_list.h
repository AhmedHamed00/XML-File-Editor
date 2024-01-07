#ifndef ERRORS_LIST_H
#define ERRORS_LIST_H

#include <QDialog>

namespace Ui {
class errors_list;
}

class errors_list : public QDialog
{
    Q_OBJECT

public:
    explicit errors_list(QWidget *parent = nullptr);
    ~errors_list();

private slots:
    void on_pushButton_clicked();

private:
    Ui::errors_list *ui;
};

#endif // ERRORS_LIST_H
