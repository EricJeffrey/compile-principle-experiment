#pragma once
#include "exp2Header.h"
#include "Exp2ProductionRule.h"

/*
定义文法
文法由非终结符，终结符，产生式组成
产生式中的ε使用$代替
*/
class Exp2Grammar {
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
    vector<Exp2ProductionRule> rules;
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
    map<char, map<char, set<Exp2ProductionRule>>> predictTable;
    /*
    终结符与非终结符的个数以及产生式的个数
    */
    int symbolNumber, ruleNumber;
    /*
    是否是LL(1)文法
    */
    bool isll1 = true;
    /*
    输入输出流
    */
    istringstream scin;
    ostringstream scout;
public:
    /*
    构造文法

    读取文法
    构造FIRST集
    构造FOLLOW集
    构造预测分析表
    分析
    */
    Exp2Grammar(string &data);

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
    bool startParse(const string &str);
    string getOutputStr();
    /*
    析构
    */
    inline void printSpace(int num = 0){
        for (int i = 0; i < num && i >= 0; i++)
            scout << ' ';
    }
    ~Exp2Grammar();
};
