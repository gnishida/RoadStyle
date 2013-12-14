/********************************************************************************
** Form generated from reading UI file 'PropertyWindow.ui'
**
** Created: Fri Dec 13 10:36:16 2013
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
#include <QtGui/QGroupBox>
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
    QPushButton *pushButtonSimplify;
    QLineEdit *lineEditSimplifyThreshold;
    QLabel *label_4;
    QPushButton *pushButtonModeSketch;
    QPushButton *pushButtonModeView;
    QGroupBox *groupBox;
    QLabel *label;
    QLineEdit *lineEditType;
    QLabel *label_2;
    QLineEdit *lineEditLanes;
    QLabel *label_3;
    QLineEdit *lineEditOneWay;
    QPushButton *pushButtonModeSelect;
    QPushButton *pushButtonClear;
    QPushButton *pushButtonMakeRoad;

    void setupUi(QDockWidget *PropertyWindow)
    {
        if (PropertyWindow->objectName().isEmpty())
            PropertyWindow->setObjectName(QString::fromUtf8("PropertyWindow"));
        PropertyWindow->resize(192, 563);
        PropertyWindow->setMinimumSize(QSize(192, 240));
        PropertyWindow->setStyleSheet(QString::fromUtf8("background-color: rgb(181, 181, 181);"));
        dockWidgetContents = new QWidget();
        dockWidgetContents->setObjectName(QString::fromUtf8("dockWidgetContents"));
        pushButtonSimplify = new QPushButton(dockWidgetContents);
        pushButtonSimplify->setObjectName(QString::fromUtf8("pushButtonSimplify"));
        pushButtonSimplify->setGeometry(QRect(20, 480, 151, 31));
        lineEditSimplifyThreshold = new QLineEdit(dockWidgetContents);
        lineEditSimplifyThreshold->setObjectName(QString::fromUtf8("lineEditSimplifyThreshold"));
        lineEditSimplifyThreshold->setGeometry(QRect(100, 450, 71, 20));
        label_4 = new QLabel(dockWidgetContents);
        label_4->setObjectName(QString::fromUtf8("label_4"));
        label_4->setGeometry(QRect(20, 450, 46, 21));
        pushButtonModeSketch = new QPushButton(dockWidgetContents);
        pushButtonModeSketch->setObjectName(QString::fromUtf8("pushButtonModeSketch"));
        pushButtonModeSketch->setGeometry(QRect(70, 10, 51, 51));
        pushButtonModeView = new QPushButton(dockWidgetContents);
        pushButtonModeView->setObjectName(QString::fromUtf8("pushButtonModeView"));
        pushButtonModeView->setGeometry(QRect(10, 10, 51, 51));
        groupBox = new QGroupBox(dockWidgetContents);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        groupBox->setGeometry(QRect(10, 220, 171, 111));
        label = new QLabel(groupBox);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(10, 20, 31, 16));
        lineEditType = new QLineEdit(groupBox);
        lineEditType->setObjectName(QString::fromUtf8("lineEditType"));
        lineEditType->setGeometry(QRect(80, 20, 71, 20));
        label_2 = new QLabel(groupBox);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setGeometry(QRect(10, 50, 51, 16));
        lineEditLanes = new QLineEdit(groupBox);
        lineEditLanes->setObjectName(QString::fromUtf8("lineEditLanes"));
        lineEditLanes->setGeometry(QRect(80, 50, 71, 20));
        label_3 = new QLabel(groupBox);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setGeometry(QRect(10, 80, 51, 16));
        lineEditOneWay = new QLineEdit(groupBox);
        lineEditOneWay->setObjectName(QString::fromUtf8("lineEditOneWay"));
        lineEditOneWay->setGeometry(QRect(80, 80, 71, 20));
        pushButtonModeSelect = new QPushButton(dockWidgetContents);
        pushButtonModeSelect->setObjectName(QString::fromUtf8("pushButtonModeSelect"));
        pushButtonModeSelect->setGeometry(QRect(130, 10, 51, 51));
        pushButtonClear = new QPushButton(dockWidgetContents);
        pushButtonClear->setObjectName(QString::fromUtf8("pushButtonClear"));
        pushButtonClear->setGeometry(QRect(20, 110, 151, 31));
        pushButtonMakeRoad = new QPushButton(dockWidgetContents);
        pushButtonMakeRoad->setObjectName(QString::fromUtf8("pushButtonMakeRoad"));
        pushButtonMakeRoad->setGeometry(QRect(20, 150, 151, 31));
        PropertyWindow->setWidget(dockWidgetContents);

        retranslateUi(PropertyWindow);

        QMetaObject::connectSlotsByName(PropertyWindow);
    } // setupUi

    void retranslateUi(QDockWidget *PropertyWindow)
    {
        pushButtonSimplify->setText(QApplication::translate("PropertyWindow", "Simplify", 0, QApplication::UnicodeUTF8));
        label_4->setText(QApplication::translate("PropertyWindow", "Threshold", 0, QApplication::UnicodeUTF8));
        pushButtonModeSketch->setText(QApplication::translate("PropertyWindow", "Sketch", 0, QApplication::UnicodeUTF8));
        pushButtonModeView->setText(QApplication::translate("PropertyWindow", "View", 0, QApplication::UnicodeUTF8));
        groupBox->setTitle(QApplication::translate("PropertyWindow", "Edge", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("PropertyWindow", "Type", 0, QApplication::UnicodeUTF8));
        label_2->setText(QApplication::translate("PropertyWindow", "Lanes", 0, QApplication::UnicodeUTF8));
        label_3->setText(QApplication::translate("PropertyWindow", "One Way", 0, QApplication::UnicodeUTF8));
        pushButtonModeSelect->setText(QApplication::translate("PropertyWindow", "Select", 0, QApplication::UnicodeUTF8));
        pushButtonClear->setText(QApplication::translate("PropertyWindow", "Clear", 0, QApplication::UnicodeUTF8));
        pushButtonMakeRoad->setText(QApplication::translate("PropertyWindow", "Make Roads", 0, QApplication::UnicodeUTF8));
        Q_UNUSED(PropertyWindow);
    } // retranslateUi

};

namespace Ui {
    class PropertyWindow: public Ui_PropertyWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_PROPERTYWINDOW_H
