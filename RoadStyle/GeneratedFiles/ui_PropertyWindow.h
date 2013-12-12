/********************************************************************************
** Form generated from reading UI file 'PropertyWindow.ui'
**
** Created: Wed Dec 11 19:25:08 2013
**      by: Qt User Interface Compiler version 4.8.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_PROPERTYWINDOW_H
#define UI_PROPERTYWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDockWidget>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_PropertyWindow
{
public:
    QWidget *dockWidgetContents;
    QLabel *labelOneWay;
    QLabel *label_2;
    QLabel *label;
    QLabel *labelNumLanes;
    QLabel *label_3;
    QLabel *labelType;

    void setupUi(QDockWidget *dockWidget)
    {
        if (dockWidget->objectName().isEmpty())
            dockWidget->setObjectName(QString::fromUtf8("dockWidget"));
        dockWidget->resize(221, 191);
        dockWidgetContents = new QWidget();
        dockWidgetContents->setObjectName(QString::fromUtf8("dockWidgetContents"));
        labelOneWay = new QLabel(dockWidgetContents);
        labelOneWay->setObjectName(QString::fromUtf8("labelOneWay"));
        labelOneWay->setGeometry(QRect(120, 80, 51, 16));
        labelOneWay->setFrameShape(QFrame::Box);
        labelOneWay->setFrameShadow(QFrame::Sunken);
        label_2 = new QLabel(dockWidgetContents);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setGeometry(QRect(30, 50, 81, 16));
        label = new QLabel(dockWidgetContents);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(30, 20, 31, 16));
        labelNumLanes = new QLabel(dockWidgetContents);
        labelNumLanes->setObjectName(QString::fromUtf8("labelNumLanes"));
        labelNumLanes->setGeometry(QRect(120, 50, 51, 16));
        labelNumLanes->setFrameShape(QFrame::Box);
        labelNumLanes->setFrameShadow(QFrame::Sunken);
        label_3 = new QLabel(dockWidgetContents);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setGeometry(QRect(30, 80, 51, 16));
        labelType = new QLabel(dockWidgetContents);
        labelType->setObjectName(QString::fromUtf8("labelType"));
        labelType->setGeometry(QRect(120, 20, 51, 16));
        labelType->setFrameShape(QFrame::Box);
        labelType->setFrameShadow(QFrame::Sunken);
        dockWidget->setWidget(dockWidgetContents);

        retranslateUi(dockWidget);

        QMetaObject::connectSlotsByName(dockWidget);
    } // setupUi

    void retranslateUi(QDockWidget *dockWidget)
    {
        labelOneWay->setText(QString());
        label_2->setText(QApplication::translate("PropertyWindow", "Num of Lanes", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("PropertyWindow", "Type", 0, QApplication::UnicodeUTF8));
        labelNumLanes->setText(QString());
        label_3->setText(QApplication::translate("PropertyWindow", "One Way", 0, QApplication::UnicodeUTF8));
        labelType->setText(QString());
        Q_UNUSED(dockWidget);
    } // retranslateUi

};

namespace Ui {
    class PropertyWindow: public Ui_PropertyWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_PROPERTYWINDOW_H
