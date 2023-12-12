#include "xml_application.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    XML_Application w;
    w.show();
    return a.exec();
}
