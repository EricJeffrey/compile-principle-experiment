#pragma once
#include "exp1Header.h"

/*
词法分析二元组
负责设置二元组的值以及二元组的输出
*/
class Exp1WordTuple {
private:
    string word;
    int len;
    int typeCode;
public:
    Exp1WordTuple(const string &source, int tc);
    void printWord();
    ~Exp1WordTuple();
};