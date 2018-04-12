#define _CRT_SECURE_NO_WARNINGS
#include <cstdio>
#include <cstdlib>
#include <cstring>

struct productionRule {
    char leftPart;
    char *rightPart;
    int rPartLength;
};
struct grammer {
    productionRule *rules;
    char *symbols;
    char **firstSet, **followSet;
    int symbolNumber, ruleNumber;

    void init(int ruleNum) {
        ruleNumber = ruleNum;
        rules = (productionRule *)malloc((ruleNum + 2) * sizeof productionRule);
        firstSet = followSet = NULL;
        symbolNumber = 0;
    }
    //从标准输入输出读取文法
    void readRules() {

    }
    void getFirst() {

    }
    void getFollow() {

    }
};
