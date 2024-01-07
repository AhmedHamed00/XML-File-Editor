#ifndef XML_DETAILS_H
#define XML_DETAILS_H

#include <QDialog>

namespace Ui {
class xml_details;
}

class xml_details : public QDialog
{
    Q_OBJECT

public:
    explicit xml_details(QWidget *parent = nullptr);
    ~xml_details();

private slots:
    void on_mutual_button_pressed();

    void on_suggest_button_clicked();

    void on_search_nutton_clicked();

    void on_pushButton_clicked();

private:
    Ui::xml_details *ui;
};

#endif // XML_DETAILS_H
