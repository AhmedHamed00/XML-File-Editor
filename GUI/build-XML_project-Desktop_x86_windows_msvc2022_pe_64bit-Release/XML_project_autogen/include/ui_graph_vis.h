/********************************************************************************
** Form generated from reading UI file 'graph_vis.ui'
**
** Created by: Qt User Interface Compiler version 6.6.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_GRAPH_VIS_H
#define UI_GRAPH_VIS_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>

QT_BEGIN_NAMESPACE

class Ui_Graph_vis
{
public:
    QHBoxLayout *horizontalLayout;
    QLabel *label;

    void setupUi(QDialog *Graph_vis)
    {
        if (Graph_vis->objectName().isEmpty())
            Graph_vis->setObjectName("Graph_vis");
        Graph_vis->resize(311, 285);
        Graph_vis->setBaseSize(QSize(100, 100));
        horizontalLayout = new QHBoxLayout(Graph_vis);
        horizontalLayout->setObjectName("horizontalLayout");
        label = new QLabel(Graph_vis);
        label->setObjectName("label");
        label->setScaledContents(false);
        label->setAlignment(Qt::AlignCenter);

        horizontalLayout->addWidget(label);


        retranslateUi(Graph_vis);

        QMetaObject::connectSlotsByName(Graph_vis);
    } // setupUi

    void retranslateUi(QDialog *Graph_vis)
    {
        Graph_vis->setWindowTitle(QCoreApplication::translate("Graph_vis", "Graph Visualization", nullptr));
        label->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class Graph_vis: public Ui_Graph_vis {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_GRAPH_VIS_H
