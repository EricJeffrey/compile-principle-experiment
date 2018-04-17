#define _CRT_SECURE_NO_WARNINGS
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <set>
#include <vector>
#include <map>

#define epsilon $
#define epsilon_char '$'
#define endSymbol #
#define endSymbol_char '#'

using namespace std;

void error(const char *s) {
    printf("%s", s);
    exit(-1);
}

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

/*
定义文法
文法由非终结符，终结符，产生式组成
产生式中的ε使用$代替
*/
struct Grammar {
    int ruleMaxLen = 50;
    vector<ProductionRule> rules;
    set<char> terminal, nonTerminal;
    map<char, set<char>> firstSet, followSet;
    int symbolNumber, ruleNumber;

    Grammar() {
        readRules();
        generateFirst();
    }
    /*
    确定终结符与非终结符
    将每个产生式的左部作为一个非终结符加入相应的集合中
    确定非终结符后，将每个非非终结符加入终结符集合中
    */
    void getTerminalAndNon() {
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
    若产生式中有ε，请使用$代替
    默认输入的文法为LL(1)文法
    */
    void readRules() {
        char tmpc;
        char *tmpstr;
        tmpstr = (char *)malloc(ruleMaxLen);
        while (scanf(" %c %s", &tmpc, tmpstr) != EOF) {
            rules.push_back(ProductionRule(tmpc, tmpstr));
        }
        getTerminalAndNon();
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
            printf("%c\n", *it);
        }
        puts("Terminal:");
        for (it = terminal.begin(); it != terminal.end(); it++) {
            printf("%c\n", *it);
        }
        puts("FIRST:");
        map<char, set<char>>::iterator fit;
        for (fit = firstSet.begin(); fit != firstSet.end(); fit++) {
            if (terminal.find(fit->first) != terminal.end())
                continue;
            printf("FIRST[%c]: ", fit->first);
            for (it = fit->second.begin(); it != fit->second.end(); it++) {
                printf(" %c", *it);
            }
            puts("");
        }
    }
    /*
    构造first集
    */
    void generateFirst() {
        set<char>::iterator it;
        //终结符：FIRST(X) = X
        for (it = terminal.begin(); it != terminal.end(); it++) {
            firstSet[*it].insert(*it);
        }
        int cursz = 0;
        int lasz = -1;
        while (cursz != lasz) {
            lasz = cursz;
            //对于每一个产生式进行判断
            for (int i = 0; i < (int)rules.size(); i++) {
                ProductionRule tmprule = rules[i];
                char X = tmprule.leftPart;
                char Y = tmprule.rightPart[0];
                int ylen = tmprule.rPartLength;
                //X->a... : FIRST[X] += a
                if (terminal.find(Y) != terminal.end()) {
                    firstSet[X].insert(Y);
                    continue;
                }
                //X->Y... : FIRST[X] += FIRST[Y]
                else if (nonTerminal.find(Y) != nonTerminal.end()) {
                    firstSet[X].insert(firstSet[Y].begin(), firstSet[Y].end());
                    //若epsilon属于FIRST[Y1...Yi-1]，则将FIRST[Yi]加入FIRST[X]中
                    int j = 0;
                    char cury = Y;
                    while (j < ylen && firstSet[cury].find(epsilon_char) != firstSet[cury].end()) {
                        //Y0...Yn -> epsilon，将epsilon加入FIRST[X]
                        if (j == ylen - 1) {
                            firstSet[X].insert(epsilon_char);
                        }
                        else {
                            char nxty = tmprule.rightPart[j + 1];
                            firstSet[X].insert(firstSet[nxty].begin(), firstSet[nxty].end());
                        }
                        j++;
                    }
                }
                else {
                    error("终结符与非终结符未正确识别");
                }
            }
            //统计first集的大小
            cursz = 0;
            map<char, set<char>>::iterator it;
            for (it = firstSet.begin(); it != firstSet.end(); it++) {
                cursz += it->second.size();
            }
        }
    }
    /*
    构造follow集
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
