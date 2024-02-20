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
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDoubleSpinBox>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSlider>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_CalibrateWidget
{
public:
    QLabel *label;
    QDoubleSpinBox *diameter_input;
    QLabel *label_2;
    QPushButton *select_image_file;
    QLabel *original_image;
    QLabel *label_4;
    QLabel *binarized_image;
    QLabel *label_6;
    QPushButton *preview_btn;
    QPushButton *calibrate_btn;
    QLabel *calibration_factor_label;
    QComboBox *diameter_unit_selection;
    QSlider *threshold_value_slider;
    QLabel *threshold_value_display;

    void setupUi(QWidget *CalibrateWidget)
    {
        if (CalibrateWidget->objectName().isEmpty())
            CalibrateWidget->setObjectName("CalibrateWidget");
        CalibrateWidget->resize(1920, 1080);
        CalibrateWidget->setStyleSheet(QString::fromUtf8(""));
        label = new QLabel(CalibrateWidget);
        label->setObjectName("label");
        label->setGeometry(QRect(660, 0, 281, 61));
        label->setStyleSheet(QString::fromUtf8("font-size: 35px; color: black;"));
        diameter_input = new QDoubleSpinBox(CalibrateWidget);
        diameter_input->setObjectName("diameter_input");
        diameter_input->setGeometry(QRect(940, 10, 151, 41));
        diameter_input->setStyleSheet(QString::fromUtf8("background: white;"));
        label_2 = new QLabel(CalibrateWidget);
        label_2->setObjectName("label_2");
        label_2->setGeometry(QRect(660, 100, 261, 61));
        label_2->setStyleSheet(QString::fromUtf8("font-size: 35px; color: black;"));
        select_image_file = new QPushButton(CalibrateWidget);
        select_image_file->setObjectName("select_image_file");
        select_image_file->setGeometry(QRect(870, 160, 151, 31));
        select_image_file->setStyleSheet(QString::fromUtf8("background: white;"));
        original_image = new QLabel(CalibrateWidget);
        original_image->setObjectName("original_image");
        original_image->setGeometry(QRect(10, 330, 930, 510));
        original_image->setStyleSheet(QString::fromUtf8("background: brown;"));
        label_4 = new QLabel(CalibrateWidget);
        label_4->setObjectName("label_4");
        label_4->setGeometry(QRect(420, 270, 91, 41));
        label_4->setStyleSheet(QString::fromUtf8("font-size: 25px; color: black;"));
        binarized_image = new QLabel(CalibrateWidget);
        binarized_image->setObjectName("binarized_image");
        binarized_image->setGeometry(QRect(970, 330, 930, 510));
        binarized_image->setStyleSheet(QString::fromUtf8("background: brown;"));
        label_6 = new QLabel(CalibrateWidget);
        label_6->setObjectName("label_6");
        label_6->setGeometry(QRect(1430, 280, 101, 31));
        label_6->setStyleSheet(QString::fromUtf8("font-size: 25px; color: black;"));
        preview_btn = new QPushButton(CalibrateWidget);
        preview_btn->setObjectName("preview_btn");
        preview_btn->setGeometry(QRect(870, 200, 151, 31));
        preview_btn->setStyleSheet(QString::fromUtf8("background: white;"));
        calibrate_btn = new QPushButton(CalibrateWidget);
        calibrate_btn->setObjectName("calibrate_btn");
        calibrate_btn->setGeometry(QRect(870, 240, 151, 31));
        calibrate_btn->setStyleSheet(QString::fromUtf8("background: white;"));
        calibration_factor_label = new QLabel(CalibrateWidget);
        calibration_factor_label->setObjectName("calibration_factor_label");
        calibration_factor_label->setGeometry(QRect(710, 840, 881, 81));
        calibration_factor_label->setStyleSheet(QString::fromUtf8("font-size: 35px; color: lightgreen; font-weight: bold;"));
        diameter_unit_selection = new QComboBox(CalibrateWidget);
        diameter_unit_selection->setObjectName("diameter_unit_selection");
        diameter_unit_selection->setGeometry(QRect(1090, 10, 91, 41));
        diameter_unit_selection->setStyleSheet(QString::fromUtf8("background: white;"));
        threshold_value_slider = new QSlider(CalibrateWidget);
        threshold_value_slider->setObjectName("threshold_value_slider");
        threshold_value_slider->setGeometry(QRect(930, 120, 160, 22));
        threshold_value_slider->setOrientation(Qt::Horizontal);
        threshold_value_display = new QLabel(CalibrateWidget);
        threshold_value_display->setObjectName("threshold_value_display");
        threshold_value_display->setGeometry(QRect(1110, 110, 71, 31));
        threshold_value_display->setStyleSheet(QString::fromUtf8("font-size: 30px;"));

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
        label_6->setText(QCoreApplication::translate("CalibrateWidget", "Binary", nullptr));
        preview_btn->setText(QCoreApplication::translate("CalibrateWidget", "Preview", nullptr));
        calibrate_btn->setText(QCoreApplication::translate("CalibrateWidget", "Calibrate", nullptr));
        calibration_factor_label->setText(QCoreApplication::translate("CalibrateWidget", "Calibration Factor: ", nullptr));
        threshold_value_display->setText(QCoreApplication::translate("CalibrateWidget", "127", nullptr));
    } // retranslateUi

};

namespace Ui {
    class CalibrateWidget: public Ui_CalibrateWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CALIBRATEWIDGET_H
