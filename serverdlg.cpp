#include "serverdlg.h"
#include "ui_serverdlg.h"

#include <QHostInfo>

ServerDlg::ServerDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ServerDlg)
{
    ui->setupUi(this);

    QHostInfo info;
    info = QHostInfo::fromName(QHostInfo::localHostName());
    foreach(QHostAddress address, info.addresses())
        if(address.protocol() == QAbstractSocket::IPv4Protocol)
            ui->ipLable->setText(address.toString());
}

ServerDlg::~ServerDlg()
{
    delete ui;
}

void ServerDlg::on_buttonBox_accepted()
{
    emit serverSet(ui->portSpinBox->value());
}
