/********************************************************************************
** Form generated from reading UI file 'error_checking_window.ui'
**
** Created by: Qt User Interface Compiler version 6.6.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_ERROR_CHECKING_WINDOW_H
#define UI_ERROR_CHECKING_WINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_error_checking_window
{
public:
    QVBoxLayout *verticalLayout;
    QCheckBox *checkBox;
    QCheckBox *checkBox_2;
    QHBoxLayout *horizontalLayout_2;
    QSpacerItem *horizontalSpacer;
    QPushButton *pushButton;

    void setupUi(QDialog *error_checking_window)
    {
        if (error_checking_window->objectName().isEmpty())
            error_checking_window->setObjectName("error_checking_window");
        error_checking_window->resize(256, 213);
        verticalLayout = new QVBoxLayout(error_checking_window);
        verticalLayout->setObjectName("verticalLayout");
        checkBox = new QCheckBox(error_checking_window);
        checkBox->setObjectName("checkBox");

        verticalLayout->addWidget(checkBox);

        checkBox_2 = new QCheckBox(error_checking_window);
        checkBox_2->setObjectName("checkBox_2");

        verticalLayout->addWidget(checkBox_2);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName("horizontalLayout_2");
        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer);

        pushButton = new QPushButton(error_checking_window);
        pushButton->setObjectName("pushButton");

        horizontalLayout_2->addWidget(pushButton);


        verticalLayout->addLayout(horizontalLayout_2);


        retranslateUi(error_checking_window);

        QMetaObject::connectSlotsByName(error_checking_window);
    } // setupUi

    void retranslateUi(QDialog *error_checking_window)
    {
        error_checking_window->setWindowTitle(QCoreApplication::translate("error_checking_window", "Extra Error Checking Options", nullptr));
        checkBox->setText(QCoreApplication::translate("error_checking_window", "Check on Misspelled Tags", nullptr));
        checkBox_2->setText(QCoreApplication::translate("error_checking_window", "Check on Missing Brackets", nullptr));
        pushButton->setText(QCoreApplication::translate("error_checking_window", "Ok", nullptr));
    } // retranslateUi

};

namespace Ui {
    class error_checking_window: public Ui_error_checking_window {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_ERROR_CHECKING_WINDOW_H
