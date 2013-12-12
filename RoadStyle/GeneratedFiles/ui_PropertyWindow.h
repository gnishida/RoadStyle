/********************************************************************************
** Form generated from reading UI file 'PropertyWindow.ui'
**
** Created: Thu Dec 12 10:59:46 2013
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
#include <QtGui/QLineEdit>
#include <QtGui/QPushButton>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_PropertyWindow
{
public:
    QWidget *dockWidgetContents;
    QLabel *label_2;
    QLabel *label;
    QLabel *label_3;
    QLineEdit *lineEditType;
    QLineEdit *lineEditLanes;
    QLineEdit *lineEditOneWay;
    QPushButton *pushButtonSimplify;
    QLineEdit *lineEditSimplifyThreshold;
    QLabel *label_4;

    void setupUi(QDockWidget *PropertyWindow)
    {
        if (PropertyWindow->objectName().isEmpty())
            PropertyWindow->setObjectName(QString::fromUtf8("PropertyWindow"));
        PropertyWindow->resize(150, 411);
        PropertyWindow->setMinimumSize(QSize(150, 240));
        PropertyWindow->setStyleSheet(QString::fromUtf8("background-color: rgb(181, 181, 181);"));
        dockWidgetContents = new QWidget();
        dockWidgetContents->setObjectName(QString::fromUtf8("dockWidgetContents"));
        label_2 = new QLabel(dockWidgetContents);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setGeometry(QRect(10, 40, 51, 16));
        label = new QLabel(dockWidgetContents);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(10, 10, 31, 16));
        label_3 = new QLabel(dockWidgetContents);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setGeometry(QRect(10, 70, 51, 16));
        lineEditType = new QLineEdit(dockWidgetContents);
        lineEditType->setObjectName(QString::fromUtf8("lineEditType"));
        lineEditType->setGeometry(QRect(80, 10, 61, 20));
        lineEditLanes = new QLineEdit(dockWidgetContents);
        lineEditLanes->setObjectName(QString::fromUtf8("lineEditLanes"));
        lineEditLanes->setGeometry(QRect(80, 40, 61, 20));
        lineEditOneWay = new QLineEdit(dockWidgetContents);
        lineEditOneWay->setObjectName(QString::fromUtf8("lineEditOneWay"));
        lineEditOneWay->setGeometry(QRect(80, 70, 61, 20));
        pushButtonSimplify = new QPushButton(dockWidgetContents);
        pushButtonSimplify->setObjectName(QString::fromUtf8("pushButtonSimplify"));
        pushButtonSimplify->setGeometry(QRect(10, 150, 131, 31));
        lineEditSimplifyThreshold = new QLineEdit(dockWidgetContents);
        lineEditSimplifyThreshold->setObjectName(QString::fromUtf8("lineEditSimplifyThreshold"));
        lineEditSimplifyThreshold->setGeometry(QRect(70, 120, 71, 20));
        label_4 = new QLabel(dockWidgetContents);
        label_4->setObjectName(QString::fromUtf8("label_4"));
        label_4->setGeometry(QRect(10, 120, 46, 21));
        PropertyWindow->setWidget(dockWidgetContents);

        retranslateUi(PropertyWindow);

        QMetaObject::connectSlotsByName(PropertyWindow);
    } // setupUi

    void retranslateUi(QDockWidget *PropertyWindow)
    {
        label_2->setText(QApplication::translate("PropertyWindow", "Lanes", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("PropertyWindow", "Type", 0, QApplication::UnicodeUTF8));
        label_3->setText(QApplication::translate("PropertyWindow", "One Way", 0, QApplication::UnicodeUTF8));
        pushButtonSimplify->setText(QApplication::translate("PropertyWindow", "Simplify", 0, QApplication::UnicodeUTF8));
        label_4->setText(QApplication::translate("PropertyWindow", "Threshold", 0, QApplication::UnicodeUTF8));
        Q_UNUSED(PropertyWindow);
    } // retranslateUi

};

namespace Ui {
    class PropertyWindow: public Ui_PropertyWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_PROPERTYWINDOW_H
