#include "msg.h"

void Msg::Error(QString msg)
{
    QMessageBox MsgError;
    MsgError.setText(msg);
    MsgError.setIcon(QMessageBox::Critical);
    MsgError.exec();
}
