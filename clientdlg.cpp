#include "clientdlg.h"
#include "ui_clientdlg.h"

ClientDlg::ClientDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ClientDlg)
{
    ui->setupUi(this);
}

ClientDlg::~ClientDlg()
{
    delete ui;
}

void ClientDlg::on_buttonBox_accepted()
{
    QString ip;
    ip.append(QString::number(ui->ipSpinBox1->value())).append('.')
      .append(QString::number(ui->ipSpinBox2->value())).append('.')
      .append(QString::number(ui->ipSpinBox3->value())).append('.')
      .append(QString::number(ui->ipSpinBox4->value()));
    quint32 port = ui->portSpinBox->value();
    emit clientSet(ip, port);
}
