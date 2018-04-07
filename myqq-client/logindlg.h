#ifndef LOGINDLG_H
#define LOGINDLG_H

#include <QDialog>
#include <QPushButton>
#include <QLabel>
#include <QLineEdit>

class loginDlg : public QDialog
{
    Q_OBJECT
public:
    explicit loginDlg(QWidget *parent = 0);

signals:

public slots:

private:

    QLabel *lable0;
    QLabel *lable1;
    QLabel *lable2;
    QLabel *lable3;
    QLineEdit *lineEditUserID;
    QLineEdit *lineEditPasswd;
    QLineEdit *lineEditHostIP;
    QLineEdit *lineEditHostport;
    QPushButton *loginBtn;
    QPushButton *logoutBtn;

private slots:
    void loginBtnOnclick();
    void logoutBtnOnclick();

public:
    int userid;
    QString passwd;
    QString hostip;
    int hostport;
    bool islogin;

};

#endif // LOGINDLG_H
