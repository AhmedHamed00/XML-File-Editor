#ifndef JSON_H
#define JSON_H

#include <QDialog>

namespace Ui {
class Json;
}

class Json : public QDialog
{
    Q_OBJECT

public:
    explicit Json(QWidget *parent = nullptr);
    ~Json();

private slots:
    void on_pushButton_clicked();

private:
    Ui::Json *ui;
};

#endif // JSON_H
