/********************************************************************************
** Form generated from reading UI file 'CalibrateWidget.ui'
**
** Created by: Qt User Interface Compiler version 6.6.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CALIBRATEWIDGET_H
#define UI_CALIBRATEWIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDoubleSpinBox>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_CalibrateWidget
{
public:
    QLabel *label;
    QDoubleSpinBox *diameter_input;
    QLabel *label_2;
    QSpinBox *threshold_input;
    QPushButton *select_image_file;
    QLabel *original_image;
    QLabel *label_4;
    QLabel *binarized_image;
    QLabel *label_6;
    QPushButton *preview_btn;
    QPushButton *calibrate_btn;

    void setupUi(QWidget *CalibrateWidget)
    {
        if (CalibrateWidget->objectName().isEmpty())
            CalibrateWidget->setObjectName("CalibrateWidget");
        CalibrateWidget->resize(1920, 1080);
        label = new QLabel(CalibrateWidget);
        label->setObjectName("label");
        label->setGeometry(QRect(660, 0, 281, 61));
        label->setStyleSheet(QString::fromUtf8("font-size: 35px; color: black;"));
        diameter_input = new QDoubleSpinBox(CalibrateWidget);
        diameter_input->setObjectName("diameter_input");
        diameter_input->setGeometry(QRect(940, 10, 151, 41));
        label_2 = new QLabel(CalibrateWidget);
        label_2->setObjectName("label_2");
        label_2->setGeometry(QRect(660, 100, 261, 61));
        label_2->setStyleSheet(QString::fromUtf8("font-size: 35px; color: black;"));
        threshold_input = new QSpinBox(CalibrateWidget);
        threshold_input->setObjectName("threshold_input");
        threshold_input->setGeometry(QRect(940, 110, 151, 41));
        select_image_file = new QPushButton(CalibrateWidget);
        select_image_file->setObjectName("select_image_file");
        select_image_file->setGeometry(QRect(870, 160, 151, 31));
        original_image = new QLabel(CalibrateWidget);
        original_image->setObjectName("original_image");
        original_image->setGeometry(QRect(10, 330, 930, 510));
        original_image->setStyleSheet(QString::fromUtf8("background: gray;"));
        label_4 = new QLabel(CalibrateWidget);
        label_4->setObjectName("label_4");
        label_4->setGeometry(QRect(420, 270, 91, 41));
        label_4->setStyleSheet(QString::fromUtf8("font-size: 25px; color: black;"));
        binarized_image = new QLabel(CalibrateWidget);
        binarized_image->setObjectName("binarized_image");
        binarized_image->setGeometry(QRect(970, 330, 930, 510));
        binarized_image->setStyleSheet(QString::fromUtf8("background: gray;"));
        label_6 = new QLabel(CalibrateWidget);
        label_6->setObjectName("label_6");
        label_6->setGeometry(QRect(1480, 280, 101, 31));
        label_6->setStyleSheet(QString::fromUtf8("font-size: 25px; color: black;"));
        preview_btn = new QPushButton(CalibrateWidget);
        preview_btn->setObjectName("preview_btn");
        preview_btn->setGeometry(QRect(870, 200, 151, 31));
        calibrate_btn = new QPushButton(CalibrateWidget);
        calibrate_btn->setObjectName("calibrate_btn");
        calibrate_btn->setGeometry(QRect(870, 240, 151, 31));

        retranslateUi(CalibrateWidget);

        QMetaObject::connectSlotsByName(CalibrateWidget);
    } // setupUi

    void retranslateUi(QWidget *CalibrateWidget)
    {
        CalibrateWidget->setWindowTitle(QCoreApplication::translate("CalibrateWidget", "Form", nullptr));
        label->setText(QCoreApplication::translate("CalibrateWidget", "Gauge Diameter:", nullptr));
        label_2->setText(QCoreApplication::translate("CalibrateWidget", "Threshold Value:", nullptr));
        select_image_file->setText(QCoreApplication::translate("CalibrateWidget", "Select Image File", nullptr));
        original_image->setText(QString());
        label_4->setText(QCoreApplication::translate("CalibrateWidget", "Original", nullptr));
        binarized_image->setText(QString());
        label_6->setText(QCoreApplication::translate("CalibrateWidget", "Binarized", nullptr));
        preview_btn->setText(QCoreApplication::translate("CalibrateWidget", "Preview", nullptr));
        calibrate_btn->setText(QCoreApplication::translate("CalibrateWidget", "Calibrate", nullptr));
    } // retranslateUi

};

namespace Ui {
    class CalibrateWidget: public Ui_CalibrateWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CALIBRATEWIDGET_H
