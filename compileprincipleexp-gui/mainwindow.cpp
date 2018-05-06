#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "Exp1WordAnalyzer.h"
#include "Exp2Grammar.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include <string>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
{
    QWidget *window = new QWidget();
    setCentralWidget(window);

    //layout of the application
    QHBoxLayout *appLayout = new QHBoxLayout();
    window->setLayout(appLayout);

    //layout contain buttons choose different exp
    QVBoxLayout *expChooseLayout = new QVBoxLayout();
    appLayout->addItem(expChooseLayout);

    //button to choose exp1
    QPushButton *exp1ChooseButton = new QPushButton("实验一", window);
    expChooseLayout->addWidget(exp1ChooseButton);

    //button to choose exp2
    QPushButton *exp2ChooseButton = new QPushButton("实验二", window);
    expChooseLayout->addWidget(exp2ChooseButton);

    //button to choose exp3
    QPushButton *exp3ChooseButton = new QPushButton("实验三", window);
    expChooseLayout->addWidget(exp3ChooseButton);

    //experiment 1 window and content, default one
    exp1Window = new QWidget(window);
    appLayout->addWidget(exp1Window);
    QHBoxLayout *exp1Layout = new QHBoxLayout();
    exp1Window->setLayout(exp1Layout);
    exp1InputTE = new QTextEdit(exp1Window);
    exp1InputTE->setMaximumWidth(300);
    exp1InputTE->setText(QString("输入待分析的C语言源代码"));
    exp1OutputTE = new QTextEdit(exp1Window);
    exp1GoButton = new QPushButton("分析词法", exp1Window);
    exp1OutputTE->setReadOnly(true);
    exp1Layout->addWidget(exp1InputTE);
    exp1Layout->addWidget(exp1GoButton);
    exp1Layout->addWidget(exp1OutputTE);

    QObject::connect(exp1GoButton, &QPushButton::clicked, this, &MainWindow::doExp1);
    QObject::connect(exp1ChooseButton, &QPushButton::clicked, this, &MainWindow::showExp1Window);

    //experiment 2 window and content
    exp2Window = new QWidget(window);
    appLayout->addWidget(exp2Window);
    QHBoxLayout *exp2Layout = new QHBoxLayout();
    exp2Window->setLayout(exp2Layout);
    exp2InputTE = new QTextEdit(exp2Window);
    exp2InputTE->setMaximumWidth(300);
    exp2InputTE->setText("输入待分析的语法以及句子，输入格式为:\n 产生式个数\n产生式1左部 产生式1右部\n\
产生式2左部 产生式2右部\n...\n句子个数\n句子1\n句子2\n...");
    exp2OutputTE = new QTextEdit(exp2Window);
    exp2GoButton = new QPushButton("分析语法(LL(1))", exp2Window);
    exp2OutputTE->setReadOnly(true);
    exp2Layout->addWidget(exp2InputTE);
    exp2Layout->addWidget(exp2GoButton);
    exp2Layout->addWidget(exp2OutputTE);
    exp2Window->setVisible(false);

    QObject::connect(exp2GoButton, &QPushButton::clicked, this, &MainWindow::doExp2);
    QObject::connect(exp2ChooseButton, &QPushButton::clicked, this, &MainWindow::showExp2Window);

    //experiment 3 window and content
    exp3Window = new QWidget(window);
    appLayout->addWidget(exp3Window);

    exp3Window->setVisible(false);
    QObject::connect(exp3ChooseButton, &QPushButton::clicked, this, &MainWindow::showExp3Window);
}
void MainWindow::showExp1Window(){
    exp1Window->show();
    exp2Window->hide();
    exp3Window->hide();
}
void MainWindow::showExp2Window(){
    exp2Window->show();
    exp1Window->hide();
    exp3Window->hide();
}
void MainWindow::showExp3Window(){
    exp3Window->show();
    exp1Window->setVisible(false);
    exp2Window->setVisible(false);
}
void MainWindow::doExp1(){
    std::string data = exp1InputTE->toPlainText().toStdString();
    Exp1WordAnalyzer tmp(data);
    exp1OutputTE->setText(QString().fromStdString(tmp.getOutputStr()));
}
void MainWindow::doExp2(){
    std::string data = exp2InputTE->toPlainText().toStdString();
    Exp2Grammar tmp(data);
    exp2OutputTE->setText(QString().fromStdString(tmp.getOutputStr()));
}
MainWindow::~MainWindow()
{

}
