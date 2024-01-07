/********************************************************************************
** Form generated from reading UI file 'json_window2.ui'
**
** Created by: Qt User Interface Compiler version 6.6.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_JSON_WINDOW2_H
#define UI_JSON_WINDOW2_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_json_window2
{
public:
    QVBoxLayout *verticalLayout_2;
    QVBoxLayout *verticalLayout;
    QTextEdit *textEdit2;
    QHBoxLayout *horizontalLayout;
    QSpacerItem *horizontalSpacer;
    QPushButton *pushButton;

    void setupUi(QDialog *json_window2)
    {
        if (json_window2->objectName().isEmpty())
            json_window2->setObjectName("json_window2");
        json_window2->resize(481, 394);
        json_window2->setMinimumSize(QSize(400, 200));
        verticalLayout_2 = new QVBoxLayout(json_window2);
        verticalLayout_2->setObjectName("verticalLayout_2");
        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName("verticalLayout");
        textEdit2 = new QTextEdit(json_window2);
        textEdit2->setObjectName("textEdit2");

        verticalLayout->addWidget(textEdit2);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName("horizontalLayout");
        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        pushButton = new QPushButton(json_window2);
        pushButton->setObjectName("pushButton");

        horizontalLayout->addWidget(pushButton);


        verticalLayout->addLayout(horizontalLayout);


        verticalLayout_2->addLayout(verticalLayout);


        retranslateUi(json_window2);

        QMetaObject::connectSlotsByName(json_window2);
    } // setupUi

    void retranslateUi(QDialog *json_window2)
    {
        json_window2->setWindowTitle(QCoreApplication::translate("json_window2", "Json file", nullptr));
        pushButton->setText(QCoreApplication::translate("json_window2", "Ok", nullptr));
    } // retranslateUi

};

namespace Ui {
    class json_window2: public Ui_json_window2 {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_JSON_WINDOW2_H
