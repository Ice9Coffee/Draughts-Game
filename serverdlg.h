#ifndef SERVERDLG_H
#define SERVERDLG_H

#include <QDialog>

namespace Ui {
class ServerDlg;
}

class ServerDlg : public QDialog
{
    Q_OBJECT

public:
    explicit ServerDlg(QWidget *parent = 0);
    ~ServerDlg();

signals:
    void serverSet(quint16 port);

private slots:
    void on_buttonBox_accepted();

private:
    Ui::ServerDlg *ui;
};

#endif // SERVERDLG_H
