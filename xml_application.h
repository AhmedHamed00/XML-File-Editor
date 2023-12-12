#ifndef XML_APPLICATION_H
#define XML_APPLICATION_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui {
class XML_Application;
}
QT_END_NAMESPACE

class XML_Application : public QMainWindow
{
    Q_OBJECT

public:
    XML_Application(QWidget *parent = nullptr);
    ~XML_Application();

private slots:
    void on_actionOpen_XML_file_triggered();

    void on_actionAdd_XML_by_Text_triggered();

    void on_actionQuit_triggered();

    void on_pushButton_clicked();

private:
    Ui::XML_Application *ui;
};
#endif // XML_APPLICATION_H
