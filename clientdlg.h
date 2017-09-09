#ifndef CLIENTDLG_H
#define CLIENTDLG_H

#include <QDialog>

namespace Ui {
class ClientDlg;
}

class ClientDlg : public QDialog
{
    Q_OBJECT

public:
    explicit ClientDlg(QWidget *parent = 0);
    ~ClientDlg();

signals:
    void clientSet(QString ip, quint16 port);

private slots:
    void on_buttonBox_accepted();

private:
    Ui::ClientDlg *ui;
};

#endif // CLIENTDLG_H
