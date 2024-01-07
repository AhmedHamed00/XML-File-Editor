/********************************************************************************
** Form generated from reading UI file 'xml_details.ui'
**
** Created by: Qt User Interface Compiler version 6.6.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_XML_DETAILS_H
#define UI_XML_DETAILS_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_xml_details
{
public:
    QHBoxLayout *horizontalLayout_3;
    QVBoxLayout *verticalLayout_2;
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout_7;
    QVBoxLayout *verticalLayout_3;
    QHBoxLayout *horizontalLayout_2;
    QLabel *label_influencer;
    QLineEdit *lineEdit_influencer;
    QHBoxLayout *horizontalLayout_4;
    QLabel *label_active;
    QLineEdit *lineEdit_active;
    QPushButton *pushButton;
    QHBoxLayout *horizontalLayout;
    QLabel *label_search;
    QLineEdit *lineEdit_search;
    QCheckBox *checkBox;
    QPushButton *search_nutton;
    QHBoxLayout *horizontalLayout_6;
    QPushButton *mutual_button;
    QComboBox *comboBox_mutual_2;
    QComboBox *comboBox_mutual_1;
    QHBoxLayout *horizontalLayout_8;
    QPushButton *suggest_button;
    QComboBox *comboBox_suggestions;
    QHBoxLayout *horizontalLayout_5;
    QTextEdit *textEdit;

    void setupUi(QDialog *xml_details)
    {
        if (xml_details->objectName().isEmpty())
            xml_details->setObjectName("xml_details");
        xml_details->resize(690, 521);
        horizontalLayout_3 = new QHBoxLayout(xml_details);
        horizontalLayout_3->setObjectName("horizontalLayout_3");
        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setObjectName("verticalLayout_2");
        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName("verticalLayout");
        horizontalLayout_7 = new QHBoxLayout();
        horizontalLayout_7->setObjectName("horizontalLayout_7");
        verticalLayout_3 = new QVBoxLayout();
        verticalLayout_3->setObjectName("verticalLayout_3");
        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName("horizontalLayout_2");
        label_influencer = new QLabel(xml_details);
        label_influencer->setObjectName("label_influencer");
        label_influencer->setMinimumSize(QSize(120, 0));

        horizontalLayout_2->addWidget(label_influencer);

        lineEdit_influencer = new QLineEdit(xml_details);
        lineEdit_influencer->setObjectName("lineEdit_influencer");
        lineEdit_influencer->setMinimumSize(QSize(150, 0));

        horizontalLayout_2->addWidget(lineEdit_influencer);


        verticalLayout_3->addLayout(horizontalLayout_2);

        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setObjectName("horizontalLayout_4");
        label_active = new QLabel(xml_details);
        label_active->setObjectName("label_active");
        label_active->setMinimumSize(QSize(120, 0));

        horizontalLayout_4->addWidget(label_active);

        lineEdit_active = new QLineEdit(xml_details);
        lineEdit_active->setObjectName("lineEdit_active");
        lineEdit_active->setMinimumSize(QSize(150, 0));

        horizontalLayout_4->addWidget(lineEdit_active);


        verticalLayout_3->addLayout(horizontalLayout_4);


        horizontalLayout_7->addLayout(verticalLayout_3);

        pushButton = new QPushButton(xml_details);
        pushButton->setObjectName("pushButton");
        QSizePolicy sizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(pushButton->sizePolicy().hasHeightForWidth());
        pushButton->setSizePolicy(sizePolicy);
        pushButton->setMinimumSize(QSize(150, 0));

        horizontalLayout_7->addWidget(pushButton);


        verticalLayout->addLayout(horizontalLayout_7);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName("horizontalLayout");
        label_search = new QLabel(xml_details);
        label_search->setObjectName("label_search");
        label_search->setMinimumSize(QSize(120, 0));

        horizontalLayout->addWidget(label_search);

        lineEdit_search = new QLineEdit(xml_details);
        lineEdit_search->setObjectName("lineEdit_search");
        lineEdit_search->setMinimumSize(QSize(150, 0));

        horizontalLayout->addWidget(lineEdit_search);

        checkBox = new QCheckBox(xml_details);
        checkBox->setObjectName("checkBox");

        horizontalLayout->addWidget(checkBox);

        search_nutton = new QPushButton(xml_details);
        search_nutton->setObjectName("search_nutton");
        search_nutton->setMinimumSize(QSize(150, 0));

        horizontalLayout->addWidget(search_nutton);


        verticalLayout->addLayout(horizontalLayout);

        horizontalLayout_6 = new QHBoxLayout();
        horizontalLayout_6->setObjectName("horizontalLayout_6");
        mutual_button = new QPushButton(xml_details);
        mutual_button->setObjectName("mutual_button");
        QSizePolicy sizePolicy1(QSizePolicy::Minimum, QSizePolicy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(mutual_button->sizePolicy().hasHeightForWidth());
        mutual_button->setSizePolicy(sizePolicy1);
        mutual_button->setMinimumSize(QSize(100, 0));

        horizontalLayout_6->addWidget(mutual_button);

        comboBox_mutual_2 = new QComboBox(xml_details);
        comboBox_mutual_2->setObjectName("comboBox_mutual_2");

        horizontalLayout_6->addWidget(comboBox_mutual_2);

        comboBox_mutual_1 = new QComboBox(xml_details);
        comboBox_mutual_1->setObjectName("comboBox_mutual_1");

        horizontalLayout_6->addWidget(comboBox_mutual_1);


        verticalLayout->addLayout(horizontalLayout_6);

        horizontalLayout_8 = new QHBoxLayout();
        horizontalLayout_8->setObjectName("horizontalLayout_8");
        suggest_button = new QPushButton(xml_details);
        suggest_button->setObjectName("suggest_button");
        sizePolicy1.setHeightForWidth(suggest_button->sizePolicy().hasHeightForWidth());
        suggest_button->setSizePolicy(sizePolicy1);
        suggest_button->setMinimumSize(QSize(100, 0));

        horizontalLayout_8->addWidget(suggest_button);

        comboBox_suggestions = new QComboBox(xml_details);
        comboBox_suggestions->setObjectName("comboBox_suggestions");

        horizontalLayout_8->addWidget(comboBox_suggestions);


        verticalLayout->addLayout(horizontalLayout_8);

        horizontalLayout_5 = new QHBoxLayout();
        horizontalLayout_5->setObjectName("horizontalLayout_5");

        verticalLayout->addLayout(horizontalLayout_5);

        textEdit = new QTextEdit(xml_details);
        textEdit->setObjectName("textEdit");

        verticalLayout->addWidget(textEdit);


        verticalLayout_2->addLayout(verticalLayout);


        horizontalLayout_3->addLayout(verticalLayout_2);


        retranslateUi(xml_details);

        QMetaObject::connectSlotsByName(xml_details);
    } // setupUi

    void retranslateUi(QDialog *xml_details)
    {
        xml_details->setWindowTitle(QCoreApplication::translate("xml_details", "Network Analysis", nullptr));
        label_influencer->setText(QCoreApplication::translate("xml_details", "Most Influencer User", nullptr));
        label_active->setText(QCoreApplication::translate("xml_details", "Most Active User", nullptr));
        pushButton->setText(QCoreApplication::translate("xml_details", "Show Graph", nullptr));
        label_search->setText(QCoreApplication::translate("xml_details", "Search for posts", nullptr));
        checkBox->setText(QCoreApplication::translate("xml_details", "Find first Only", nullptr));
        search_nutton->setText(QCoreApplication::translate("xml_details", "Search", nullptr));
        mutual_button->setText(QCoreApplication::translate("xml_details", "Show Mutual Followers", nullptr));
        suggest_button->setText(QCoreApplication::translate("xml_details", "Suggest users to follow", nullptr));
    } // retranslateUi

};

namespace Ui {
    class xml_details: public Ui_xml_details {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_XML_DETAILS_H
