/********************************************************************************
** Form generated from reading UI file 'errors_list.ui'
**
** Created by: Qt User Interface Compiler version 6.6.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_ERRORS_LIST_H
#define UI_ERRORS_LIST_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_errors_list
{
public:
    QVBoxLayout *verticalLayout;
    QTextEdit *textEdit;
    QHBoxLayout *horizontalLayout;
    QSpacerItem *horizontalSpacer;
    QPushButton *pushButton;

    void setupUi(QDialog *errors_list)
    {
        if (errors_list->objectName().isEmpty())
            errors_list->setObjectName("errors_list");
        errors_list->resize(383, 313);
        verticalLayout = new QVBoxLayout(errors_list);
        verticalLayout->setObjectName("verticalLayout");
        textEdit = new QTextEdit(errors_list);
        textEdit->setObjectName("textEdit");
        textEdit->setMinimumSize(QSize(550, 0));

        verticalLayout->addWidget(textEdit);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName("horizontalLayout");
        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        pushButton = new QPushButton(errors_list);
        pushButton->setObjectName("pushButton");

        horizontalLayout->addWidget(pushButton);


        verticalLayout->addLayout(horizontalLayout);


        retranslateUi(errors_list);

        QMetaObject::connectSlotsByName(errors_list);
    } // setupUi

    void retranslateUi(QDialog *errors_list)
    {
        errors_list->setWindowTitle(QCoreApplication::translate("errors_list", "Errors List", nullptr));
        pushButton->setText(QCoreApplication::translate("errors_list", "Ok", nullptr));
    } // retranslateUi

};

namespace Ui {
    class errors_list: public Ui_errors_list {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_ERRORS_LIST_H
