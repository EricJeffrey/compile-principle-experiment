#define _CRT_SECURE_NO_WARNINGS
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <set>
#include <vector>

using namespace std;

/*
产生式结构
包括产生式左部，右部
*/
struct ProductionRule {
    char leftPart;
    char *rightPart;
    int rPartLength;

    ProductionRule(char lpart, char *rpart) {
        leftPart = lpart;
        rPartLength = 0;
        while (rpart[rPartLength] != 0) rPartLength++;
        rightPart = (char *) malloc(rPartLength + 1);
        memcpy(rightPart, rpart, rPartLength);
        rightPart[rPartLength] = 0;
    }
};

//TODO 输入文法，确定终结符与非终结符
struct Grammar {
    int ruleMaxLen = 20;
    vector<ProductionRule> rules;
    set<char> terminal, nonTerminal;
    vector<set<char>> firstSet, followSet;
    int symbolNumber, ruleNumber;

    Grammar() {
        readRules();
    }
    /*
    将每个产生式的左部作为一个非终结符加入相应的集合中
    确定非终结符后，将每个非非终结符加入终结符集合中
    */
    void init() {
        for (int i = 0; i < (int)rules.size(); i++) {
            nonTerminal.insert(rules[i].leftPart);
        }
        for (int i = 0; i < (int)rules.size(); i++) {
            char *tmpstr = rules[i].rightPart;
            for (int j = 0; j < rules[i].rPartLength; j++) {
                if (tmpstr[j] != 0 && nonTerminal.find(tmpstr[j]) == nonTerminal.end()) {
                    terminal.insert(tmpstr[j]);
                }
            }
        }
    }
    /*
    从标准输入输出读取文法的产生式

    输入文法格式：left right
    若产生式中有ε，请使用epsilon代替
    默认输入的文法为LL(1)文法
    */
    void readRules() {
        char tmpc;
        char *tmpstr;
        tmpstr = (char *)malloc(ruleMaxLen);
        while (scanf(" %c %s", &tmpc, tmpstr) != EOF) {
            rules.push_back(ProductionRule(tmpc, tmpstr));
        }
        init();
    }
    /*
    输出产生式、终结符以及非终结符
    */
    void printGrammar() {
        puts("production rule:");
        for (int i = 0; i < (int)rules.size(); i++) {
            printf("%c -> %s\n", rules[i].leftPart, rules[i].rightPart);
        }
        puts("NonTerminal:");
        set<char>::iterator it;
        for (it = nonTerminal.begin(); it != nonTerminal.end(); it++) {
            printf("%c %d\n", *it, *it);
        }
        puts("Terminal:");
        for (it = terminal.begin(); it != terminal.end(); it++) {
            printf("%c %d\n", *it, *it);
        }
    }
    /*
    产生first集
    */
    void generateFirst() {

    }
    /*
    产生follow集
    */
    void generateFollow() {

    }
};

int main() {
    freopen("exp-two-data.in.txt", "r", stdin);
    freopen("exp-two-data.out.txt", "w+", stdout);
    Grammar().printGrammar();
    return 0;
}
