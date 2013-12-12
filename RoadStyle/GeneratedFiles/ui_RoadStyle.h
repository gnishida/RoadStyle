/********************************************************************************
** Form generated from reading UI file 'RoadStyle.ui'
**
** Created: Thu Dec 12 16:50:29 2013
**      by: Qt User Interface Compiler version 4.8.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_ROADSTYLE_H
#define UI_ROADSTYLE_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHeaderView>
#include <QtGui/QMainWindow>
#include <QtGui/QMenu>
#include <QtGui/QMenuBar>
#include <QtGui/QStatusBar>
#include <QtGui/QToolBar>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_RoadStyleClass
{
public:
    QAction *actionOpen;
    QAction *actionExit;
    QAction *actionAll_roads;
    QAction *actionHighways;
    QAction *actionAvenues;
    QAction *actionStreets;
    QAction *actionExtract_and_Save;
    QAction *actionProperty;
    QAction *actionNew;
    QWidget *centralWidget;
    QMenuBar *menuBar;
    QMenu *menuFile;
    QMenu *menuView;
    QMenu *menuWindow;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *RoadStyleClass)
    {
        if (RoadStyleClass->objectName().isEmpty())
            RoadStyleClass->setObjectName(QString::fromUtf8("RoadStyleClass"));
        RoadStyleClass->resize(962, 737);
        actionOpen = new QAction(RoadStyleClass);
        actionOpen->setObjectName(QString::fromUtf8("actionOpen"));
        actionExit = new QAction(RoadStyleClass);
        actionExit->setObjectName(QString::fromUtf8("actionExit"));
        actionAll_roads = new QAction(RoadStyleClass);
        actionAll_roads->setObjectName(QString::fromUtf8("actionAll_roads"));
        actionHighways = new QAction(RoadStyleClass);
        actionHighways->setObjectName(QString::fromUtf8("actionHighways"));
        actionHighways->setCheckable(true);
        actionHighways->setChecked(true);
        actionAvenues = new QAction(RoadStyleClass);
        actionAvenues->setObjectName(QString::fromUtf8("actionAvenues"));
        actionAvenues->setCheckable(true);
        actionAvenues->setChecked(true);
        actionStreets = new QAction(RoadStyleClass);
        actionStreets->setObjectName(QString::fromUtf8("actionStreets"));
        actionStreets->setCheckable(true);
        actionStreets->setChecked(true);
        actionExtract_and_Save = new QAction(RoadStyleClass);
        actionExtract_and_Save->setObjectName(QString::fromUtf8("actionExtract_and_Save"));
        actionProperty = new QAction(RoadStyleClass);
        actionProperty->setObjectName(QString::fromUtf8("actionProperty"));
        actionNew = new QAction(RoadStyleClass);
        actionNew->setObjectName(QString::fromUtf8("actionNew"));
        centralWidget = new QWidget(RoadStyleClass);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        RoadStyleClass->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(RoadStyleClass);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 962, 21));
        menuFile = new QMenu(menuBar);
        menuFile->setObjectName(QString::fromUtf8("menuFile"));
        menuView = new QMenu(menuBar);
        menuView->setObjectName(QString::fromUtf8("menuView"));
        menuWindow = new QMenu(menuBar);
        menuWindow->setObjectName(QString::fromUtf8("menuWindow"));
        RoadStyleClass->setMenuBar(menuBar);
        mainToolBar = new QToolBar(RoadStyleClass);
        mainToolBar->setObjectName(QString::fromUtf8("mainToolBar"));
        RoadStyleClass->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(RoadStyleClass);
        statusBar->setObjectName(QString::fromUtf8("statusBar"));
        RoadStyleClass->setStatusBar(statusBar);

        menuBar->addAction(menuFile->menuAction());
        menuBar->addAction(menuView->menuAction());
        menuBar->addAction(menuWindow->menuAction());
        menuFile->addAction(actionNew);
        menuFile->addAction(actionOpen);
        menuFile->addAction(actionExit);
        menuView->addAction(actionHighways);
        menuView->addAction(actionAvenues);
        menuView->addAction(actionStreets);
        menuWindow->addAction(actionProperty);

        retranslateUi(RoadStyleClass);

        QMetaObject::connectSlotsByName(RoadStyleClass);
    } // setupUi

    void retranslateUi(QMainWindow *RoadStyleClass)
    {
        RoadStyleClass->setWindowTitle(QApplication::translate("RoadStyleClass", "RoadStyle", 0, QApplication::UnicodeUTF8));
        actionOpen->setText(QApplication::translate("RoadStyleClass", "Open", 0, QApplication::UnicodeUTF8));
        actionExit->setText(QApplication::translate("RoadStyleClass", "Exit", 0, QApplication::UnicodeUTF8));
        actionAll_roads->setText(QApplication::translate("RoadStyleClass", "All roads", 0, QApplication::UnicodeUTF8));
        actionHighways->setText(QApplication::translate("RoadStyleClass", "Highways", 0, QApplication::UnicodeUTF8));
        actionAvenues->setText(QApplication::translate("RoadStyleClass", "Avenues", 0, QApplication::UnicodeUTF8));
        actionStreets->setText(QApplication::translate("RoadStyleClass", "Streets", 0, QApplication::UnicodeUTF8));
        actionExtract_and_Save->setText(QApplication::translate("RoadStyleClass", "Extract and Save", 0, QApplication::UnicodeUTF8));
        actionProperty->setText(QApplication::translate("RoadStyleClass", "Property", 0, QApplication::UnicodeUTF8));
        actionNew->setText(QApplication::translate("RoadStyleClass", "New", 0, QApplication::UnicodeUTF8));
        menuFile->setTitle(QApplication::translate("RoadStyleClass", "File", 0, QApplication::UnicodeUTF8));
        menuView->setTitle(QApplication::translate("RoadStyleClass", "View", 0, QApplication::UnicodeUTF8));
        menuWindow->setTitle(QApplication::translate("RoadStyleClass", "Window", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class RoadStyleClass: public Ui_RoadStyleClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_ROADSTYLE_H
