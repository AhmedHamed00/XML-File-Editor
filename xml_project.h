#ifndef XML_PROJECT_H
#define XML_PROJECT_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui {
class XML_project;
}
QT_END_NAMESPACE

class XML_project : public QMainWindow
{
    Q_OBJECT

public:
    XML_project(QWidget *parent = nullptr);
    ~XML_project();

private slots:
    void color_errors();
    void on_actionOpen_XML_file_triggered();

    void on_actionSave_Text_as_XML_file_triggered();

    void on_actionClear_triggered();

    void on_error_check_button_clicked();

    void on_error_correct_button_clicked();

    void on_jason_button_clicked();

    void on_format_button_clicked();

    void on_compress_button_clicked();

    void on_decompress_button_clicked();

    void on_graph_button_clicked();

    void on_details_button_clicked();

    void on_actionundo_triggered();

    void on_actionRedo_triggered();

private:
    Ui::XML_project *ui;
};
#endif // XML_PROJECT_H
