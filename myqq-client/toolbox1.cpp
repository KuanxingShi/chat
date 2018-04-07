#include <QMessageBox>
#include <QHostAddress>
#include "toolbox1.h"
#include "logindlg.h"
#include "widget1.h"

//主窗口的信号过滤函数，watched代表是哪个控件触发了信号，event代表触发了具体什么信号
bool toolbox1::eventFilter(QObject *watched, QEvent *event)
{
    if(event->type() == QEvent::MouseButtonPress)//如果是鼠标按键信号
    {
        int i = watched->objectName().toInt();
        child[i]->hide();
        child[i]->showNormal();//将toolbtn下对应的child显示到屏幕
    }
    return QToolBox::eventFilter(watched, event);//其他信号交给父类进行默认处理
}

void toolbox1::init_username()
{
    for(int i = 0;i<CLINETCOUNT;i++ )//将username[]数组中所有的成员初始化
    {
        username[i] = tr("用户") + QString::number(i);
    }
}

void toolbox1::init_toolBtn()//初始化所有的toolBtn
{
    QString imagename;
    for(int i = 0;i<CLINETCOUNT;i++ )
    {
        toolBtn[i] = new QToolButton();
        toolBtn[i]->setObjectName(QString::number(i));//为每个toolBtn设置ObjectName属性
        toolBtn[i]->installEventFilter(this);//toolBtn的点击事件由toolbox1来处理
        toolBtn[i]->setText(username[i]);//设置toolBtn标题
        imagename.sprintf(":/images/%d.png", i);
        toolBtn[i]->setIcon(QPixmap(imagename));////设置toolBtn图像
        toolBtn[i]->setIconSize(QPixmap(imagename).size());//设置toolBtn大小和图像一致
        toolBtn[i]->setAutoRaise(true);//设置toolBtn自动浮起界面风格
        toolBtn[i]->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);//设置toolBtn文字在图像旁边
        toolBtn[i]->setToolTip(tr("未知"));//设置toolBtn 鼠标提示为“未知”
        child[i] = new Widget1(toolBtn[i]->icon(), i, toolBtn[i]->text(), this);
    }

    QGroupBox *groupBox[8];
    QVBoxLayout *layout[8];
    static int index = 0;
    for(int i=0;i<8;i++)
    {
        groupBox[i] = new QGroupBox;
        layout[i] = new QVBoxLayout(groupBox[i]);
        layout[i]->setMargin(10);//设置布局中各窗体的显示间距
        layout[i]->setAlignment(Qt::AlignHCenter);//设置布局中各窗体的位置
        for(int j=0;j<32;j++)
        {
            layout[i]->addWidget(toolBtn[index++]);
        }
        layout[i]->addStretch();//插入一个占位符
    }

    addItem((QWidget *)groupBox[0], tr("好友"));
    addItem((QWidget *)groupBox[1], tr("陌生人"));
    addItem((QWidget *)groupBox[2], tr("家人"));
    addItem((QWidget *)groupBox[3], tr("同学"));
    addItem((QWidget *)groupBox[4], tr("同事"));
    addItem((QWidget *)groupBox[5], tr("网友"));
    addItem((QWidget *)groupBox[6], tr("驴友"));
    addItem((QWidget *)groupBox[7], tr("社区"));
}


toolbox1::toolbox1(QWidget *parent) :
    QToolBox(parent)
{
    setWindowIcon(QPixmap(":/images/3.png"));
    init_username();//--------------设置用户名------------------
    init_toolBtn();//--------------初始化主窗口控件------------------

    //-------------弹出登录对话框------------------------------
    loginDlg login;
    login.exec();

    if(login.islogin)//登录对话框中用户点击登录按钮
    {
        if((login.userid < 0) ||(login.userid >= CLINETCOUNT))//如果用户输入的userid小于0，大于255，提示错误，程序退出
        {
            QMessageBox::information(this, tr("错误"), tr("无效用户ID号"));
            exit(0);
        }
        setWindowTitle(username[login.userid]);//根据登录对话框中用户输入的ID号设置主窗口标题.

        userid = login.userid;
        passwd = login.passwd;
        hostip = login.hostip;
        hostport = login.hostport;

        //根据登录对话框中用户输入的服务器IP和端口号连接到服务器
        sockClient = new QTcpSocket(this);
        connect(sockClient, SIGNAL(error(QAbstractSocket::SocketError)), this,
                SLOT(sock_Error(QAbstractSocket::SocketError)));//设置socket出错时的槽函数
        connect(sockClient, SIGNAL(readyRead()), this, SLOT(read_Msg()));//设置socket读消息的槽函数
        connect(sockClient, SIGNAL(connected()), this, SLOT(socket_connected()));//设置socket完成连接消息的槽函数
        QHostAddress hostAddr(hostip);
        sockClient->connectToHost(hostAddr, hostport);
    }
    else
    {
        exit(0);
    }
}

toolbox1::~toolbox1()
{

}

void toolbox1::sock_Error(QAbstractSocket::SocketError sockErr)//socket出错时调用的槽函数
{
    switch (sockErr)
    {
    case QAbstractSocket::RemoteHostClosedError:
        break;
    default:
        QMessageBox::information(this, tr("错误"), sockClient->errorString());
    }
}

void toolbox1::read_Msg()//socket接收到来自server端的消息时调用的槽函数
{
    struct msg_t msg;
    while(sockClient->bytesAvailable() > 0)
    {
        //int iLen = sockClient->bytesAvailable();
        memset(&msg, 0, sizeof(msg));
        sockClient->read((char *)&msg, sizeof(msg));

        switch (msg.head[0]) {
        case 0://来自server的send消息
            recv_send_Msg(msg.head[1], msg.body);
            break;
        case 1://来自server的用户状态消息
            userStatus_Msg(msg.body);
            break;
        case 2://来自server的系统消息
            system_Msg(msg.head[1]);;
            break;
        default:
            ;//未知消息 --忽略
            break;
        }
    }
}

void toolbox1::login_Msg()//发送登录消息
{
    const char *pw = passwd.toStdString().data();
    if(sockClient->isOpen())
    {
        if (sockClient->state() == QAbstractSocket::ConnectedState)
        {
            struct msg_t msg;
            memset(&msg, 0, sizeof(msg));
            msg.head[0] = 1;//设置消息为登录消息
            msg.head[1] = userid;//设置登录用户ID
            msg.head[2] = 0; //暂时保留，填0
            msg.head[3] = 0; //暂时保留，填0
            strncpy(msg.body, pw, strlen(pw));//设置登录消息密码
            sockClient->write((const char *)&msg, strlen(msg.body) + sizeof(msg.head));
        }
    }
}

void toolbox1::recv_send_Msg(int o_userid, const char *msgBody)//来自server的send消息
{
    child[o_userid]->add_msg(username[o_userid], msgBody);
    child[o_userid]->show();
}

void toolbox1::userStatus_Msg(const char *msgBody)//来自server的deliver消息
{
    for(int i=0;i<CLINETCOUNT;i++)
    {
        if(msgBody[i] == '1')
        {
            toolBtn[i]->setToolTip(tr("在线"));//设置toolBtn 鼠标提示为“在线”
            child[i]->pushButton->setEnabled(true);//设置child窗口中pushButton控件状态为可用
        }
        else
        {
            toolBtn[i]->setToolTip(tr("离线"));//设置toolBtn 鼠标提示为“离线”
            child[i]->pushButton->setEnabled(false);//设置child窗口中pushButton控件状态为不可用
        }
    }
}

void toolbox1::send_Msg(int d_userid, const char *msgBody)//发往server的send消息
{
    if(sockClient->isOpen())
    {
        if (sockClient->state() == QAbstractSocket::ConnectedState)
        {
            struct msg_t msg;
            memset(&msg, 0, sizeof(msg));
            msg.head[0] = 0;//设置消息为send消息
            msg.head[1] = userid;//设置消息源用户ID
            msg.head[2] = d_userid;//设置消息目的用户ID
            msg.head[3] = 0; //暂时保留，填0
            strncpy(msg.body, msgBody, strlen(msgBody));//设置send消息内容
            sockClient->write((const char *)&msg, strlen(msg.body) + sizeof(msg.head));
        }
    }
}

void toolbox1::closeEvent(QCloseEvent *event)//窗口在close之前会自动调用closeEvent函数
{
    QMessageBox::StandardButton button = QMessageBox::question(this, tr("退出程序"),
                                                               QString(tr("是否退出?")),
                                                               QMessageBox::Yes | QMessageBox::No);

    if (button == QMessageBox::Yes)
    {
        event->accept();  //接受退出信号，程序退出
        if (sockClient->isOpen())
            sockClient->close();

        for(int i=0;i<CLINETCOUNT;i++)
        {
            delete child[i];
        }
    }
    else
    {
        event->ignore();  //忽略退出信号，程序继续运行
    }
}

void toolbox1::system_Msg(int msgcode)//来自server的系统消息
{
    switch (msgcode) {
    case 0:
        QMessageBox::information(this, tr("来自server的消息"), tr("无法识别的消息"));
        break;
    case 1:
        QMessageBox::information(this, tr("来自server的消息"), tr("无效userid"));
        break;
    case 2:
        QMessageBox::information(this, tr("来自server的消息"), tr("无效密码"));
        break;
    case 3:
        QMessageBox::information(this, tr("来自server的消息"), tr("userid已经登录"));
        break;
    case 4:
        QMessageBox::information(this, tr("来自server的消息"), tr("其他"));
        break;
    default:
        QMessageBox::information(this, tr("来自server的消息"), tr("未知消息"));
    }
}

void toolbox1::socket_connected()//socket连接到server端以后调用的槽函数
{
    login_Msg();//向server端发送登录消息
}


