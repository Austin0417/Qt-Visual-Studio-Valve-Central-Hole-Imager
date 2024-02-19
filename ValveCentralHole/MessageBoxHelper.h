#pragma once
#include <QMessageBox>



class MessageBoxHelper
{
public:
    MessageBoxHelper();
    void static ShowErrorDialog(const QString& error_msg);
};

