#include "xml_project.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    XML_project w;
    w.show();
    return a.exec();
}
