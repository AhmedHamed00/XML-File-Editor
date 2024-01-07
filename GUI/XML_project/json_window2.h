#ifndef JSON_WINDOW2_H
#define JSON_WINDOW2_H

#include <QDialog>

namespace Ui {
class json_window2;
}

class json_window2 : public QDialog
{
    Q_OBJECT

public:
    explicit json_window2(QWidget *parent = nullptr);
    ~json_window2();

private slots:
    void on_pushButton_clicked();

private:
    Ui::json_window2 *ui;
};

#endif // JSON_WINDOW2_H
