/********************************************************************************
** Form generated from reading UI file 'MeasureWidget.ui'
**
** Created by: Qt User Interface Compiler version 6.6.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MEASUREWIDGET_H
#define UI_MEASUREWIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MeasureWidget
{
public:
    QLabel *current_calibration_factor;
    QPushButton *select_valve_image;
    QLabel *label_2;
    QLabel *label_4;
    QLabel *original_valve;
    QLabel *binarized_valve;
    QPushButton *preview_btn;
    QPushButton *measure_btn;
    QLabel *valve_display_label;
    QLabel *calculated_valve_area;
    QSpinBox *valve_threshold_input;
    QLabel *label;

    void setupUi(QWidget *MeasureWidget)
    {
        if (MeasureWidget->objectName().isEmpty())
            MeasureWidget->setObjectName("MeasureWidget");
        MeasureWidget->resize(1920, 1080);
        current_calibration_factor = new QLabel(MeasureWidget);
        current_calibration_factor->setObjectName("current_calibration_factor");
        current_calibration_factor->setGeometry(QRect(730, 0, 471, 61));
        current_calibration_factor->setStyleSheet(QString::fromUtf8("font-size: 35px; color: black;"));
        select_valve_image = new QPushButton(MeasureWidget);
        select_valve_image->setObjectName("select_valve_image");
        select_valve_image->setGeometry(QRect(890, 150, 161, 41));
        label_2 = new QLabel(MeasureWidget);
        label_2->setObjectName("label_2");
        label_2->setGeometry(QRect(390, 410, 91, 31));
        label_2->setStyleSheet(QString::fromUtf8("font-size: 25px; color: black;"));
        label_4 = new QLabel(MeasureWidget);
        label_4->setObjectName("label_4");
        label_4->setGeometry(QRect(1390, 420, 111, 31));
        label_4->setStyleSheet(QString::fromUtf8("font-size: 25px; color: black;"));
        original_valve = new QLabel(MeasureWidget);
        original_valve->setObjectName("original_valve");
        original_valve->setGeometry(QRect(20, 460, 930, 510));
        original_valve->setStyleSheet(QString::fromUtf8("background-color: gray;"));
        binarized_valve = new QLabel(MeasureWidget);
        binarized_valve->setObjectName("binarized_valve");
        binarized_valve->setGeometry(QRect(970, 460, 930, 510));
        binarized_valve->setStyleSheet(QString::fromUtf8("background-color: gray;"));
        preview_btn = new QPushButton(MeasureWidget);
        preview_btn->setObjectName("preview_btn");
        preview_btn->setGeometry(QRect(890, 200, 161, 41));
        measure_btn = new QPushButton(MeasureWidget);
        measure_btn->setObjectName("measure_btn");
        measure_btn->setGeometry(QRect(890, 250, 161, 41));
        valve_display_label = new QLabel(MeasureWidget);
        valve_display_label->setObjectName("valve_display_label");
        valve_display_label->setGeometry(QRect(840, 310, 251, 41));
        valve_display_label->setStyleSheet(QString::fromUtf8("font-size: 25px; color: black;"));
        calculated_valve_area = new QLabel(MeasureWidget);
        calculated_valve_area->setObjectName("calculated_valve_area");
        calculated_valve_area->setGeometry(QRect(850, 350, 231, 31));
        calculated_valve_area->setStyleSheet(QString::fromUtf8("font-size: 25px; color: green;"));
        valve_threshold_input = new QSpinBox(MeasureWidget);
        valve_threshold_input->setObjectName("valve_threshold_input");
        valve_threshold_input->setGeometry(QRect(1010, 90, 151, 41));
        label = new QLabel(MeasureWidget);
        label->setObjectName("label");
        label->setGeometry(QRect(730, 90, 261, 31));
        label->setStyleSheet(QString::fromUtf8("font-size: 35px; color: black;"));

        retranslateUi(MeasureWidget);

        QMetaObject::connectSlotsByName(MeasureWidget);
    } // setupUi

    void retranslateUi(QWidget *MeasureWidget)
    {
        MeasureWidget->setWindowTitle(QCoreApplication::translate("MeasureWidget", "Form", nullptr));
        current_calibration_factor->setText(QCoreApplication::translate("MeasureWidget", "Calibration Factor: ", nullptr));
        select_valve_image->setText(QCoreApplication::translate("MeasureWidget", "Select Valve Image", nullptr));
        label_2->setText(QCoreApplication::translate("MeasureWidget", "Original", nullptr));
        label_4->setText(QCoreApplication::translate("MeasureWidget", "Binarized", nullptr));
        original_valve->setText(QString());
        binarized_valve->setText(QString());
        preview_btn->setText(QCoreApplication::translate("MeasureWidget", "Preview", nullptr));
        measure_btn->setText(QCoreApplication::translate("MeasureWidget", "Measure", nullptr));
        valve_display_label->setText(QCoreApplication::translate("MeasureWidget", "Calculated Valve Area: ", nullptr));
        calculated_valve_area->setText(QString());
        label->setText(QCoreApplication::translate("MeasureWidget", "Threshold Value:", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MeasureWidget: public Ui_MeasureWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MEASUREWIDGET_H
