#pragma once
#include "experiment-header.h"

//实验一

/*
词法分析二元组
负责设置二元组的值以及二元组的输出
*/
class WordTuple {
private:
    char *word;
    int len;
    int typeCode;
public:
    void setWrod(char *source, int sz, int tc);
    void printWord();
    ~WordTuple();
};

/*
词法分析器
包括
*/
class WordAnalyzer {
public:
    /*
    构造分析器

    读入源代码
    预处理后输出结果
    分析词法
    输出分析后结果
    */
    WordAnalyzer();
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
    void preprocess(char *input, char *output);
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
    int wordParse(char *input, WordTuple *output);
    ~WordAnalyzer();
};

//实验二

/*
产生式结构
包括产生式左部，右部
*/
class ProductionRule {
private:
    char leftPart;
    char *rightPart;
    int rPartLength;
public:
    ProductionRule(char lpart, char *rpart);
    bool operator < (ProductionRule t) const {
        if (leftPart < t.leftPart) return true;
        else if (leftPart == t.leftPart) {
            if (rPartLength < t.rPartLength) return true;
            else if (rPartLength == t.rPartLength) {
                int tmp = strcmp(rightPart, t.rightPart);
                if (tmp == -1) return true;
                else return false;
            }
            else return false;
        }
        else return false;
    }
    char getLeftPart();
    char *getRightPart();
    int getRPartLength();
    ~ProductionRule();
};

/*
定义文法
文法由非终结符，终结符，产生式组成
产生式中的ε使用$代替
*/
class Grammar {
private:
    /*
    规定产生式的长度最大值
    */
    int ruleMaxLen = 50;
    /*
    记录开始符
    */
    char startSymbol;
    /*
    所有产生式
    */
    vector<ProductionRule> rules;
    /*
    按照产生式输入顺序记录所有非终结符
    */
    vector<char> nonTerminalVec;
    /*
    终结符与非终结符集合
    */
    set<char> terminal, nonTerminal;
    /*
    FIRST集和FOLLOW集
    */
    map<char, set<char>> firstSet, followSet;
    /*
    预测分析表
    */
    map<char, map<char, set<ProductionRule>>> predictTable;
    /*
    终结符与非终结符的个数以及产生式的个数
    */
    int symbolNumber, ruleNumber;
    /*
    是否是LL(1)文法
    */
    bool isll1 = true;
public:
    /*
    构造文法

    读取文法
    构造FIRST集
    构造FOLLOW集
    构造预测分析表
    分析
    */
    Grammar();

    /*
    清空所有集合
    */
    void clear();
    /*
    确定终结符与非终结符
    将每个产生式的左部作为一个非终结符加入相应的集合中
    确定非终结符后，将每个非非终结符加入终结符集合中
    */
    void getTerminalAndNon();
    /*
    从标准输入输出读取文法的产生式

    输入文法格式：left right
    若产生式中有ε，请使用$代替
    默认输入的文法为LL(1)文法
    */
    void readRules();
    /*
    输出文法

    输出产生式、终结符以及非终结符
    */
    void printGrammar();
    /*
    构造first集
    */
    void generateFirst();
    /*
    构造follow集
    */
    void generateFollow();
    /*
    构造预测分析表
    */
    void generatePredictTable();
    /*
    开始分析
    */
    void startParse();
    /*
    析构
    */
};