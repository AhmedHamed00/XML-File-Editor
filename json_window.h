#ifndef JSON_WINDOW_H
#define JSON_WINDOW_H

#include <QMainWindow>

namespace Ui {
class jason_window;
}

class jason_window : public QMainWindow
{
    Q_OBJECT

public:
    explicit jason_window(QWidget *parent = nullptr);
    ~jason_window();

private slots:
    void on_pushButton_clicked();

private:
    Ui::jason_window *ui;
};

#endif // JSON_WINDOW_H
