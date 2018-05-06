#pragma once
#include "exp1Header.h"
#include "Exp1WordTuple.h"

/*
词法分析器
包括
*/
class Exp1WordAnalyzer {
public:
    /*
    构造分析器

    读入源代码
    预处理后输出结果
    分析词法
    输出分析后结果
    */
    Exp1WordAnalyzer();
    /*
    预处理，删除多余的空格，换行，回车，制表符，注释

    spaceGot：已经遇到过一个“空格类字符”，接下来的所有“空格类”字符都会被忽略
    lineCommentGot：已经遇到了“//”，接下来的字符都会被忽略，直到遇到回车字符
    blockCommentGot：已经遇到了“/*”，接下来的字符都会被忽略，直到遇到“*#(去掉#)/”
    stringGot：应遇到了“"”，接下来的字符都会被忽略，直到遇到下一个“"”

    当前处理字符位于
    1.字符串中时，忽略空格，注释的影响
    2.注释中时，忽略字符串，空格，另一种注释的影响

    input: 待处理的串，串最大长度由全局变量maxn指定
    output: 结果串存放位置，串最大长度由全局变量maxn指定
    */
    void preprocess(const string &input, string &output);
    /*
    是否是字母
    */
    bool isLetter(char ch);
    /*
    是否是数字
    */
    bool isDigit(char ch);
    /*
    词法分析，将识别出的单词放入一个二元组数组中

    input: 待处理代码串
    output: 分析结果存放位置
    return: 分析后得到二元组数目
    */
    int wordParse(const string &input, vector<Exp1WordTuple> &output);
    ~Exp1WordAnalyzer();
};