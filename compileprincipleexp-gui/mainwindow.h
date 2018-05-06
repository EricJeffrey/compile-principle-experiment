#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <string>
#include <QTextEdit>
#include <QPushButton>

class MainWindow : public QMainWindow {
public:
    MainWindow(QWidget *parent = Q_NULLPTR);
    std::string getInputText();
    void setOutputText();
    void showExp1Window();
    void showExp2Window();
    void showExp3Window();
    void doExp1();
    void doExp2();
    ~MainWindow();
private:
    QWidget *exp1Window, *exp2Window, *exp3Window;
    QTextEdit *exp1InputTE, *exp1OutputTE;
    QPushButton *exp1GoButton;
    QTextEdit *exp2InputTE, *exp2OutputTE;
    QPushButton *exp2GoButton;
    QTextEdit *exp3InputTE, *exp3OutputTE;
    QPushButton *exp3GoButton;
};

#endif // MAINWINDOW_H
