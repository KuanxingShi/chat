#include <QMessageBox>
#include <QDateTime>
#include <QScrollBar>
#include <QColorDialog>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include "widget1.h"
#include "toolbox1.h"


Widget1::Widget1(const QIcon icon, int ID, QString name, toolbox1 *w, QWidget *parent) :
    QWidget(parent)
{
    userID = ID;
    username = name;
    main_w = w;
    setWindowTitle(name);
    setWindowIcon(icon);
    
    //设置最大化最小化按钮无效
    setWindowFlags(windowFlags()& ~Qt::WindowMaximizeButtonHint & ~Qt::WindowMinimizeButtonHint);

    //setWindowFlags(windowFlags() | Qt::Tool);//设置不在任务栏出现


    //设置背景颜色
    setAutoFillBackground(true);
    QPalette palette;
    palette.setColor(QPalette::Background, QColor(Qt::darkCyan));
    setPalette(palette);


    init_widget();//初始化相关的控件
}

Widget1::~Widget1()
{

}

void Widget1::init_widget()//初始化相关的控件
{
    textBrowser = new QTextBrowser;

    //设置textBrowser背景颜色或图片
    textBrowser->setStyleSheet("background-image: url(:/images/2.jpg);");//背景设置为2.jpg
    //textBrowser->setStyleSheet("border-image: url(:/images/2.jpg);");//背景设置为2.jpg



    QHBoxLayout *layout1 = new QHBoxLayout;
    layout1->addWidget(textBrowser);

    fontComboBox = new QFontComboBox;
    fontComboBox->setToolTip(tr("字体"));

    comboBox = new QComboBox;
    comboBox->setToolTip(tr("字号"));
    comboBox->setEditable(true);
    comboBox->addItem(QString::number(8));
    comboBox->addItem(QString::number(9));
    comboBox->addItem(QString::number(10));
    comboBox->addItem(QString::number(11));
    comboBox->addItem(QString::number(12));
    comboBox->addItem(QString::number(13));
    comboBox->addItem(QString::number(14));
    comboBox->addItem(QString::number(15));
    comboBox->addItem(QString::number(16));
    comboBox->addItem(QString::number(17));
    comboBox->addItem(QString::number(18));
    comboBox->addItem(QString::number(19));
    comboBox->addItem(QString::number(20));
    comboBox->addItem(QString::number(21));
    comboBox->addItem(QString::number(22));
    comboBox->setCurrentIndex(4);

    toolButton_1 = new QToolButton;
    toolButton_1->setToolTip(tr("加粗"));
    toolButton_1->setText(tr("加粗"));
    toolButton_1->setCheckable(true);
    toolButton_1->setAutoRaise(true);
    toolButton_1->setIcon(QPixmap(":/images/bold.png"));

    toolButton_2 = new QToolButton;
    toolButton_2->setToolTip(tr("倾斜"));
    toolButton_2->setText(tr("倾斜"));
    toolButton_2->setCheckable(true);
    toolButton_2->setAutoRaise(true);
    toolButton_2->setIcon(QPixmap(":/images/italic.png"));

    toolButton_3 = new QToolButton;
    toolButton_3->setToolTip(tr("下划线"));
    toolButton_3->setText(tr("下划线"));
    toolButton_3->setCheckable(true);
    toolButton_3->setAutoRaise(true);
    toolButton_3->setIcon(QPixmap(":/images/under.png"));

    toolButton_4 = new QToolButton;
    toolButton_4->setToolTip(tr("颜色"));
    toolButton_4->setText(tr("颜色"));
    toolButton_4->setAutoRaise(true);
    toolButton_4->setIcon(QPixmap(":/images/color.png"));

    toolButton = new QToolButton;
    toolButton->setText(tr("显示主窗口"));
    toolButton->setAutoRaise(true);

    QHBoxLayout *layout2 = new QHBoxLayout;
    layout2->addWidget(fontComboBox);
    layout2->addWidget(comboBox);
    layout2->addWidget(toolButton_1);
    layout2->addWidget(toolButton_2);
    layout2->addWidget(toolButton_3);
    layout2->addWidget(toolButton_4);
    layout2->addWidget(toolButton);

    lineEdit = new QLineEdit;

    pushButton = new QPushButton;
    pushButton->setText(tr("发送"));

    QHBoxLayout *layout3 = new QHBoxLayout;
    layout3->addWidget(lineEdit);
    layout3->addWidget(pushButton);

    QVBoxLayout *mainlayout = new QVBoxLayout;

    mainlayout->addLayout(layout1);
    mainlayout->addLayout(layout2);
    mainlayout->addLayout(layout3);
    //mainlayout->setSizeConstraint(QLayout::SetFixedSize);//设置窗口大小不能改变
    setLayout(mainlayout);

    textBrowser->setCurrentFont(fontComboBox->font());//设置textBrowser的默认字体
    textBrowser->setFontPointSize(comboBox->currentText().toDouble());//设置textBrowser的默认字号
    lineEdit->setFocus();

    connect(fontComboBox, SIGNAL(currentFontChanged(const QFont &)), this, SLOT(on_fontComboBox_currentFontChanged(const QFont &)));
    connect(comboBox, SIGNAL(currentIndexChanged(const QString &)), this, SLOT(on_comboBox_currentIndexChanged(const QString &)));
    connect(toolButton_1, SIGNAL(clicked(bool)), this, SLOT(on_toolButton_1_clicked(bool)));
    connect(toolButton_2, SIGNAL(clicked(bool)), this, SLOT(on_toolButton_2_clicked(bool)));
    connect(toolButton_3, SIGNAL(clicked(bool)), this, SLOT(on_toolButton_3_clicked(bool)));
    connect(toolButton_4, SIGNAL(clicked()), this, SLOT(on_toolButton_4_clicked()));
    connect(toolButton, SIGNAL(clicked()), this, SLOT(on_toolButton_clicked()));
    connect(lineEdit, SIGNAL(returnPressed()), this, SLOT(on_lineEdit_returnPressed()));
    connect(pushButton, SIGNAL(clicked()), this, SLOT(on_pushButton_clicked()));

}

void Widget1::on_lineEdit_returnPressed()
{
    if (pushButton->isEnabled())//如果pushButton没有变灰，那么就可以调用on_pushButton_clicked()函数
        on_pushButton_clicked();
}

void Widget1::on_pushButton_clicked()
{
    if( lineEdit->text().isEmpty())//如果lineEdit控件内容为空，提示用户不能发送空消息
    {
        QMessageBox::information(this, tr("注意"), tr("不能发送空消息"));
    }
    else
    {
        add_msg(tr("我的消息"), lineEdit->text());//将要发送的消息内容加入textBrowser控件中
        main_w->send_Msg(userID, lineEdit->text().toStdString().data());//调用主窗口的send_Msg方法，向服务器提交send消息
        lineEdit->clear();//发送完成后，将lineEdit控件内容清空
    }
    lineEdit->setFocus();
}

void Widget1::add_msg(QString delivername, QString msg)//向textBrowser添加消息
{
    //得到当前时间，并把时间格式化为"yyyy-MM-dd hh:ss:ss"形式的字符串
	QString sTime = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:ss:ss");
    textBrowser->append("[" + delivername + "]" + sTime);
    textBrowser->append(msg);
    
    //当消息textBrowser中消息过多出现滚动条时，自动滚动到最下方
    textBrowser->verticalScrollBar()->setValue(textBrowser->verticalScrollBar()->maximum());
}

void Widget1::on_fontComboBox_currentFontChanged(const QFont &f)//修改textBrowser字体
{
    textBrowser->setCurrentFont(f);
    textBrowser->setFontPointSize(comboBox->currentText().toDouble());

    if(toolButton_1->isChecked())
    {
        textBrowser->setFontWeight(QFont::Bold);
    }
    else
    {
        textBrowser->setFontWeight(QFont::Normal);
    }

    if(toolButton_2->isChecked())
    {
        textBrowser->setFontItalic(true);
    }
    else
    {
        textBrowser->setFontItalic(false);
    }

    if(toolButton_3->isChecked())
    {
        textBrowser->setFontUnderline(true);
    }
    else
    {
        textBrowser->setFontUnderline(false);
    }

    textBrowser->setTextColor(color);
    lineEdit->setFocus();

}

void Widget1::on_comboBox_currentIndexChanged(const QString &arg1)//修改textBrowser字号
{
    textBrowser->setFontPointSize(arg1.toDouble());
    lineEdit->setFocus();
}

void Widget1::on_toolButton_1_clicked(bool checked)//修改textBrowser字体是否加粗
{
    if(checked)
    {
        textBrowser->setFontWeight(QFont::Bold);
    }
    else
    {
        textBrowser->setFontWeight(QFont::Normal);
    }
    lineEdit->setFocus();
}

void Widget1::on_toolButton_2_clicked(bool checked)//修改textBrowser字体是否斜体
{
    textBrowser->setFontItalic(checked);
    lineEdit->setFocus();
}

void Widget1::on_toolButton_3_clicked(bool checked)//修改textBrowser字体是否下划线
{
    textBrowser->setFontUnderline(checked);
    lineEdit->setFocus();
}

void Widget1::on_toolButton_4_clicked()//修改textBrowser字体颜色
{
    color = QColorDialog::getColor(color,this);
    if(color.isValid())
    {
        textBrowser->setTextColor(color);
        lineEdit->setFocus();
    }
}

void Widget1::on_toolButton_clicked()
{
    main_w->hide();
    main_w->showNormal();
}

