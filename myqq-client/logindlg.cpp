#include <QGridLayout>
#include <QLayout>
#include <QMessageBox>
#include "logindlg.h"

loginDlg::loginDlg(QWidget *parent) :
    QDialog(parent)
{
    lable0 = new QLabel;
    lable0->setText(tr("用户ID"));
    lineEditUserID = new QLineEdit;

    lable1 = new QLabel;
    lable1->setText(tr("密码"));
    lineEditPasswd = new QLineEdit;
    lineEditPasswd->setEchoMode(QLineEdit::Password);//设置为密码框

    lable2 = new QLabel;
    lable2->setText(tr("服务器IP"));
    lineEditHostIP = new QLineEdit;

    lable3 = new QLabel;
    lable3->setText(tr("服务器端口号"));
    lineEditHostport = new QLineEdit;


    loginBtn = new QPushButton;
    loginBtn->setText(tr("登录"));
    logoutBtn = new QPushButton;
    logoutBtn->setText(tr("取消"));


    QGridLayout *layout1 = new QGridLayout(this);
    layout1->addWidget(lable0, 0, 0);
    layout1->addWidget(lineEditUserID, 0, 1);

    layout1->addWidget(lable1, 1, 0);
    layout1->addWidget(lineEditPasswd, 1, 1);

    layout1->addWidget(lable2, 2, 0);
    layout1->addWidget(lineEditHostIP, 2, 1);

    layout1->addWidget(lable3, 3, 0);
    layout1->addWidget(lineEditHostport, 3, 1);

    layout1->addWidget(loginBtn, 4, 0);
    layout1->addWidget(logoutBtn, 4, 1);

    layout1->setColumnStretch(0, 1);
    layout1->setColumnStretch(1, 1);

    layout1->setMargin(15);
    layout1->setSpacing(10);
    layout1->setSizeConstraint(QLayout::SetFixedSize);

    connect(loginBtn,SIGNAL(clicked()), this, SLOT(loginBtnOnclick()));
    connect(logoutBtn,SIGNAL(clicked()), this, SLOT(logoutBtnOnclick()));

    islogin = false;
    setWindowIcon(QPixmap(":/images/3.png"));//设置窗口图标
    lineEditUserID->setFocus();//设置lineEditUserID控件具有输入焦点

    //设置窗口没有边框
    setWindowFlags(Qt::FramelessWindowHint);


    //设置对话框背景颜色或图片
    setAutoFillBackground(true);
    QPalette palette;
    //palette.setColor(QPalette::Background, QColor(Qt::blue));
    palette.setBrush(QPalette::Background, QBrush(QPixmap(":/images/1.jpg")));
    setPalette(palette);
}

void loginDlg::loginBtnOnclick()
{  
    if (lineEditUserID->text().isEmpty())//如果lineEditUserID控件内容为空，提示用户错误
    {
        QMessageBox::information(this, tr("错误"), tr("用户ID不能为空"));
        lineEditUserID->setFocus();
    }
    else
    {
        bool ok;
        userid = lineEditUserID->text().toInt(&ok);
        if (!ok)//如果lineEditUserID控件内容不是数字，提示用户错误
        {
            QMessageBox::information(this, tr("错误"), tr("用户ID必须是从0到255的数字"));
            lineEditUserID->setFocus();
        }else
        {
            islogin = true;
            passwd = lineEditPasswd->text();
            hostip = lineEditHostIP->text();
            hostport = lineEditHostport->text().toInt();
            close();
        }
    }
}

void loginDlg::logoutBtnOnclick()
{
    close();
}


