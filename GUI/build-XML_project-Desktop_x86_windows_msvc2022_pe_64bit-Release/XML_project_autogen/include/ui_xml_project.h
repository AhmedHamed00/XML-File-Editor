/********************************************************************************
** Form generated from reading UI file 'xml_project.ui'
**
** Created by: Qt User Interface Compiler version 6.6.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_XML_PROJECT_H
#define UI_XML_PROJECT_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_XML_project
{
public:
    QAction *actionOpen_XML_file;
    QAction *actionSave_Text_as_XML_file;
    QAction *actionClear;
    QAction *actionundo;
    QAction *actionRedo;
    QAction *actionCompress;
    QAction *actionDecompress;
    QAction *actionSave_to_another_location;
    QWidget *centralwidget;
    QHBoxLayout *horizontalLayout_2;
    QHBoxLayout *horizontalLayout;
    QTextEdit *textEdit;
    QVBoxLayout *verticalLayout_4;
    QPushButton *details_button;
    QSpacerItem *verticalSpacer;
    QPushButton *error_check_button;
    QPushButton *error_correct_button;
    QPushButton *Minify_button;
    QPushButton *format_button;
    QPushButton *jason_button;
    QVBoxLayout *verticalLayout;
    QStatusBar *statusbar;
    QToolBar *toolBar;

    void setupUi(QMainWindow *XML_project)
    {
        if (XML_project->objectName().isEmpty())
            XML_project->setObjectName("XML_project");
        XML_project->resize(617, 442);
        actionOpen_XML_file = new QAction(XML_project);
        actionOpen_XML_file->setObjectName("actionOpen_XML_file");
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/images/Open_icon-icons.com_73704.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionOpen_XML_file->setIcon(icon);
        actionSave_Text_as_XML_file = new QAction(XML_project);
        actionSave_Text_as_XML_file->setObjectName("actionSave_Text_as_XML_file");
        QIcon icon1;
        icon1.addFile(QString::fromUtf8(":/images/add.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionSave_Text_as_XML_file->setIcon(icon1);
        actionClear = new QAction(XML_project);
        actionClear->setObjectName("actionClear");
        QIcon icon2;
        icon2.addFile(QString::fromUtf8(":/images/close.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionClear->setIcon(icon2);
        actionundo = new QAction(XML_project);
        actionundo->setObjectName("actionundo");
        QIcon icon3;
        icon3.addFile(QString::fromUtf8(":/images/10558_green_undo_icon.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionundo->setIcon(icon3);
        actionRedo = new QAction(XML_project);
        actionRedo->setObjectName("actionRedo");
        QIcon icon4;
        icon4.addFile(QString::fromUtf8(":/images/10508_green_redo_icon.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionRedo->setIcon(icon4);
        actionCompress = new QAction(XML_project);
        actionCompress->setObjectName("actionCompress");
        QIcon icon5;
        icon5.addFile(QString::fromUtf8(":/images/compression.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionCompress->setIcon(icon5);
        actionCompress->setMenuRole(QAction::NoRole);
        actionDecompress = new QAction(XML_project);
        actionDecompress->setObjectName("actionDecompress");
        QIcon icon6;
        icon6.addFile(QString::fromUtf8(":/images/zip.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionDecompress->setIcon(icon6);
        actionDecompress->setMenuRole(QAction::NoRole);
        actionSave_to_another_location = new QAction(XML_project);
        actionSave_to_another_location->setObjectName("actionSave_to_another_location");
        QIcon icon7;
        icon7.addFile(QString::fromUtf8(":/images/save.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionSave_to_another_location->setIcon(icon7);
        actionSave_to_another_location->setMenuRole(QAction::NoRole);
        centralwidget = new QWidget(XML_project);
        centralwidget->setObjectName("centralwidget");
        horizontalLayout_2 = new QHBoxLayout(centralwidget);
        horizontalLayout_2->setObjectName("horizontalLayout_2");
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName("horizontalLayout");
        textEdit = new QTextEdit(centralwidget);
        textEdit->setObjectName("textEdit");
        textEdit->setMinimumSize(QSize(500, 300));

        horizontalLayout->addWidget(textEdit);

        verticalLayout_4 = new QVBoxLayout();
        verticalLayout_4->setObjectName("verticalLayout_4");
        details_button = new QPushButton(centralwidget);
        details_button->setObjectName("details_button");

        verticalLayout_4->addWidget(details_button);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_4->addItem(verticalSpacer);

        error_check_button = new QPushButton(centralwidget);
        error_check_button->setObjectName("error_check_button");

        verticalLayout_4->addWidget(error_check_button);

        error_correct_button = new QPushButton(centralwidget);
        error_correct_button->setObjectName("error_correct_button");

        verticalLayout_4->addWidget(error_correct_button);

        Minify_button = new QPushButton(centralwidget);
        Minify_button->setObjectName("Minify_button");

        verticalLayout_4->addWidget(Minify_button);

        format_button = new QPushButton(centralwidget);
        format_button->setObjectName("format_button");

        verticalLayout_4->addWidget(format_button);

        jason_button = new QPushButton(centralwidget);
        jason_button->setObjectName("jason_button");

        verticalLayout_4->addWidget(jason_button);


        horizontalLayout->addLayout(verticalLayout_4);

        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName("verticalLayout");

        horizontalLayout->addLayout(verticalLayout);


        horizontalLayout_2->addLayout(horizontalLayout);

        XML_project->setCentralWidget(centralwidget);
        statusbar = new QStatusBar(XML_project);
        statusbar->setObjectName("statusbar");
        XML_project->setStatusBar(statusbar);
        toolBar = new QToolBar(XML_project);
        toolBar->setObjectName("toolBar");
        XML_project->addToolBar(Qt::TopToolBarArea, toolBar);

        toolBar->addAction(actionOpen_XML_file);
        toolBar->addAction(actionSave_Text_as_XML_file);
        toolBar->addAction(actionSave_to_another_location);
        toolBar->addAction(actionCompress);
        toolBar->addAction(actionDecompress);
        toolBar->addAction(actionClear);
        toolBar->addAction(actionundo);
        toolBar->addAction(actionRedo);

        retranslateUi(XML_project);

        QMetaObject::connectSlotsByName(XML_project);
    } // setupUi

    void retranslateUi(QMainWindow *XML_project)
    {
        XML_project->setWindowTitle(QCoreApplication::translate("XML_project", "XML Editor", nullptr));
        actionOpen_XML_file->setText(QCoreApplication::translate("XML_project", "Open XML file", nullptr));
        actionSave_Text_as_XML_file->setText(QCoreApplication::translate("XML_project", "Save Text as XML file", nullptr));
        actionClear->setText(QCoreApplication::translate("XML_project", "Clear", nullptr));
        actionundo->setText(QCoreApplication::translate("XML_project", "Undo", nullptr));
        actionRedo->setText(QCoreApplication::translate("XML_project", "Redo", nullptr));
        actionCompress->setText(QCoreApplication::translate("XML_project", "Compress File", nullptr));
        actionDecompress->setText(QCoreApplication::translate("XML_project", "Decompress File", nullptr));
        actionSave_to_another_location->setText(QCoreApplication::translate("XML_project", "Save to another location", nullptr));
        details_button->setText(QCoreApplication::translate("XML_project", "Analyze", nullptr));
        error_check_button->setText(QCoreApplication::translate("XML_project", "Check Errors", nullptr));
        error_correct_button->setText(QCoreApplication::translate("XML_project", "Correct Errors", nullptr));
        Minify_button->setText(QCoreApplication::translate("XML_project", "Minify", nullptr));
        format_button->setText(QCoreApplication::translate("XML_project", "Format File", nullptr));
        jason_button->setText(QCoreApplication::translate("XML_project", "Show JSON", nullptr));
        toolBar->setWindowTitle(QCoreApplication::translate("XML_project", "toolBar", nullptr));
    } // retranslateUi

};

namespace Ui {
    class XML_project: public Ui_XML_project {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_XML_PROJECT_H
