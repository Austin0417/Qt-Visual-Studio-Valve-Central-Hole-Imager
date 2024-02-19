#include "MessageBoxHelper.h"

MessageBoxHelper::MessageBoxHelper() {}


void MessageBoxHelper::ShowErrorDialog(const QString& error_msg) {
    QMessageBox* messageBox = new QMessageBox(nullptr);
    messageBox->setIcon(QMessageBox::Critical);
    messageBox->setText(error_msg);
    QPushButton* ok_btn = messageBox->addButton("OK", QMessageBox::AcceptRole);

    messageBox->show();
}
