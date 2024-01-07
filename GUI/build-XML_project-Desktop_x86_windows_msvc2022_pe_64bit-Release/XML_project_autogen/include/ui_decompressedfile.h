/********************************************************************************
** Form generated from reading UI file 'decompressedfile.ui'
**
** Created by: Qt User Interface Compiler version 6.6.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DECOMPRESSEDFILE_H
#define UI_DECOMPRESSEDFILE_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_Decompressedfile
{
public:
    QVBoxLayout *verticalLayout_3;
    QVBoxLayout *verticalLayout_2;
    QTextEdit *textEdit3;
    QHBoxLayout *horizontalLayout;
    QSpacerItem *horizontalSpacer;
    QPushButton *pushButton;

    void setupUi(QDialog *Decompressedfile)
    {
        if (Decompressedfile->objectName().isEmpty())
            Decompressedfile->setObjectName("Decompressedfile");
        Decompressedfile->resize(454, 374);
        Decompressedfile->setMinimumSize(QSize(400, 200));
        verticalLayout_3 = new QVBoxLayout(Decompressedfile);
        verticalLayout_3->setObjectName("verticalLayout_3");
        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setObjectName("verticalLayout_2");
        textEdit3 = new QTextEdit(Decompressedfile);
        textEdit3->setObjectName("textEdit3");

        verticalLayout_2->addWidget(textEdit3);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName("horizontalLayout");
        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        pushButton = new QPushButton(Decompressedfile);
        pushButton->setObjectName("pushButton");

        horizontalLayout->addWidget(pushButton);


        verticalLayout_2->addLayout(horizontalLayout);


        verticalLayout_3->addLayout(verticalLayout_2);


        retranslateUi(Decompressedfile);

        QMetaObject::connectSlotsByName(Decompressedfile);
    } // setupUi

    void retranslateUi(QDialog *Decompressedfile)
    {
        Decompressedfile->setWindowTitle(QCoreApplication::translate("Decompressedfile", "Decompressed File", nullptr));
        pushButton->setText(QCoreApplication::translate("Decompressedfile", "Ok", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Decompressedfile: public Ui_Decompressedfile {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DECOMPRESSEDFILE_H
