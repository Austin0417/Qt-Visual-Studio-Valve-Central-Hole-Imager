/********************************************************************************
** Form generated from reading UI file 'ValveCentralHole.ui'
**
** Created by: Qt User Interface Compiler version 6.6.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_VALVECENTRALHOLE_H
#define UI_VALVECENTRALHOLE_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_ValveCentralHoleClass
{
public:
    QWidget *centralwidget;
    QTabWidget *tab_widget;
    QWidget *tab;
    QWidget *tab_2;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *ValveCentralHoleClass)
    {
        if (ValveCentralHoleClass->objectName().isEmpty())
            ValveCentralHoleClass->setObjectName("ValveCentralHoleClass");
        ValveCentralHoleClass->resize(1920, 1080);
        centralwidget = new QWidget(ValveCentralHoleClass);
        centralwidget->setObjectName("centralwidget");
        tab_widget = new QTabWidget(centralwidget);
        tab_widget->setObjectName("tab_widget");
        tab_widget->setGeometry(QRect(0, 0, 1911, 1051));
        tab = new QWidget();
        tab->setObjectName("tab");
        tab_widget->addTab(tab, QString());
        tab_2 = new QWidget();
        tab_2->setObjectName("tab_2");
        tab_widget->addTab(tab_2, QString());
        ValveCentralHoleClass->setCentralWidget(centralwidget);
        menubar = new QMenuBar(ValveCentralHoleClass);
        menubar->setObjectName("menubar");
        menubar->setGeometry(QRect(0, 0, 1920, 22));
        ValveCentralHoleClass->setMenuBar(menubar);
        statusbar = new QStatusBar(ValveCentralHoleClass);
        statusbar->setObjectName("statusbar");
        ValveCentralHoleClass->setStatusBar(statusbar);

        retranslateUi(ValveCentralHoleClass);

        QMetaObject::connectSlotsByName(ValveCentralHoleClass);
    } // setupUi

    void retranslateUi(QMainWindow *ValveCentralHoleClass)
    {
        ValveCentralHoleClass->setWindowTitle(QCoreApplication::translate("ValveCentralHoleClass", "MainWindow", nullptr));
        tab_widget->setTabText(tab_widget->indexOf(tab), QCoreApplication::translate("ValveCentralHoleClass", "Tab 1", nullptr));
        tab_widget->setTabText(tab_widget->indexOf(tab_2), QCoreApplication::translate("ValveCentralHoleClass", "Tab 2", nullptr));
    } // retranslateUi

};

namespace Ui {
    class ValveCentralHoleClass: public Ui_ValveCentralHoleClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_VALVECENTRALHOLE_H
