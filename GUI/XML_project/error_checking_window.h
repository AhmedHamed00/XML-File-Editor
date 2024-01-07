#ifndef ERROR_CHECKING_WINDOW_H
#define ERROR_CHECKING_WINDOW_H

#include <QDialog>

namespace Ui {
class error_checking_window;
}

class error_checking_window : public QDialog
{
    Q_OBJECT

public:
    explicit error_checking_window(QWidget *parent = nullptr);
    ~error_checking_window();

private slots:
    void on_checkBox_stateChanged(int arg1);

    void on_checkBox_2_stateChanged(int arg1);

    void on_pushButton_clicked();

private:
    Ui::error_checking_window *ui;
};

#endif // ERROR_CHECKING_WINDOW_H
