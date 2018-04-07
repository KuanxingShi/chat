#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QGroupBox>
#include <QVBoxLayout>
#include <QToolBox>
#include <QToolButton>
#include <QTcpSocket>
#include <QCloseEvent>

#define CLINETCOUNT 256  //最大客户端数量
#define MSGBODYBUF 1024

//定义消息结构
struct msg_t{
    unsigned char head[4];
    char body[MSGBODYBUF];
};

class Widget1;

class toolbox1 : public QToolBox
{
    Q_OBJECT
public:
    explicit toolbox1(QWidget *parent = 0);
    virtual ~toolbox1();
    void send_Msg(int d_userid, const char *msgBody);

signals:

public slots:
    bool eventFilter(QObject *watched, QEvent *event);

private:
    QString username[CLINETCOUNT];
    QToolButton *toolBtn[CLINETCOUNT];
    Widget1 *child[CLINETCOUNT];
    void init_toolBtn();
    void init_username();
    QTcpSocket *sockClient;
    void login_Msg();
    void recv_send_Msg(int o_userid, const char *msgBody);
    void userStatus_Msg(const char *msgBody);
    void system_Msg(int msgcode);
    int userid;
    QString passwd;
    QString hostip;
    int hostport;
    void closeEvent(QCloseEvent *event);

private slots:
    void sock_Error(QAbstractSocket::SocketError sockErr);
    void read_Msg();
    void socket_connected();

};

#endif // MAINWINDOW_H
