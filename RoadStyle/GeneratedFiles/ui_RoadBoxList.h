/********************************************************************************
** Form generated from reading UI file 'RoadBoxList.ui'
**
** Created: Sat Dec 14 17:05:16 2013
**      by: Qt User Interface Compiler version 4.8.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_ROADBOXLIST_H
#define UI_ROADBOXLIST_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDockWidget>
#include <QtGui/QHeaderView>
#include <QtGui/QPushButton>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_RoadBoxList
{
public:
    QWidget *dockWidgetContents;
    QPushButton *pushButton;

    void setupUi(QDockWidget *RoadBoxList)
    {
        if (RoadBoxList->objectName().isEmpty())
            RoadBoxList->setObjectName(QString::fromUtf8("RoadBoxList"));
        RoadBoxList->resize(270, 810);
        RoadBoxList->setMinimumSize(QSize(270, 810));
        RoadBoxList->setStyleSheet(QString::fromUtf8("background-color: rgb(181, 181, 181);"));
        dockWidgetContents = new QWidget();
        dockWidgetContents->setObjectName(QString::fromUtf8("dockWidgetContents"));
        pushButton = new QPushButton(dockWidgetContents);
        pushButton->setObjectName(QString::fromUtf8("pushButton"));
        pushButton->setGeometry(QRect(120, 140, 75, 23));
        RoadBoxList->setWidget(dockWidgetContents);

        retranslateUi(RoadBoxList);

        QMetaObject::connectSlotsByName(RoadBoxList);
    } // setupUi

    void retranslateUi(QDockWidget *RoadBoxList)
    {
        pushButton->setText(QApplication::translate("RoadBoxList", "PushButton", 0, QApplication::UnicodeUTF8));
        Q_UNUSED(RoadBoxList);
    } // retranslateUi

};

namespace Ui {
    class RoadBoxList: public Ui_RoadBoxList {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_ROADBOXLIST_H
