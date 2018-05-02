
#include "experiment-class.h"
#include "experiment-header.h"
extern bool errorExit(const char *str, char x = 0, char a = 0);
/*
产生式结构
包括产生式左部，右部
*/
ProductionRule::ProductionRule(char lpart, char *rpart) {
    leftPart = lpart;
    rPartLength = 0;
    while (rpart[rPartLength] != 0) rPartLength++;
    rightPart = (char *)malloc(rPartLength + 1);
    memcpy(rightPart, rpart, rPartLength);
    rightPart[rPartLength] = 0;
}
char ProductionRule::getLeftPart() {
    return leftPart;
}
char* ProductionRule::getRightPart() {
    return rightPart;
}
int ProductionRule::getRPartLength() {
    return rPartLength;
}
ProductionRule::~ProductionRule() {
    
}

Grammar::Grammar() {
    freopen("exp-two-data.in.txt", "r", stdin);
    freopen("exp-two-data.out.txt", "w+", stdout);
    int t;
    scanf("%d", &t);
    while (t--) {
        puts("--------------------------------------------------");
        clear();
        readRules();
        generateFirst();
        generateFollow();
        generatePredictTable();
        printGrammar();
        int k;
        scanf("%d", &k);
        while (k--) {
            startParse();
        }
        puts("--------------------------------------------------\n\n");
    }
}
void Grammar::clear() {
    startSymbol = endSymbol_char;
    rules.clear();
    nonTerminalVec.clear();
    nonTerminal.clear();
    terminal.clear();
    followSet.clear();
    firstSet.clear();
    predictTable.clear();
    symbolNumber = 0;
    ruleNumber = 0;
    isll1 = true;
}
void Grammar::getTerminalAndNon() {
    for (int i = 0; i < (int)rules.size(); i++) {
        char lp = rules[i].getLeftPart();
        if (nonTerminal.find(lp) == nonTerminal.end()) {
            nonTerminal.insert(lp);
            nonTerminalVec.push_back(lp);
        }
    }
    for (int i = 0; i < (int)rules.size(); i++) {
        char *tmpstr = rules[i].getRightPart();
        for (int j = 0; j < rules[i].getRPartLength(); j++) {
            if (tmpstr[j] != 0 && nonTerminal.find(tmpstr[j]) == nonTerminal.end()) {
                terminal.insert(tmpstr[j]);
            }
        }
    }
}
void Grammar::readRules() {
    int t;
    scanf("%d", &t);
    char tmpc;
    char *tmpstr;
    while (t--) {
        tmpstr = (char *)malloc(ruleMaxLen);
        scanf(" %c %s", &tmpc, tmpstr);
        if (startSymbol == endSymbol_char)
            startSymbol = tmpc;
        rules.push_back(ProductionRule(tmpc, tmpstr));
    }
    getTerminalAndNon();
}
void Grammar::printGrammar() {
    //输出得到的所有产生式
    puts("production rule:");
    for (int i = 0; i < (int)rules.size(); i++) {
        printf("%c -> %s\n", rules[i].getLeftPart(), rules[i].getRightPart());
    }
    puts("");
    //输出非终结符
    puts("NonTerminal:");
    set<char>::iterator it;
    for (it = nonTerminal.begin(); it != nonTerminal.end(); it++) {
        printf("%c ", *it);
    }
    puts("\n");
    //输出终结符
    puts("Terminal:");
    for (it = terminal.begin(); it != terminal.end(); it++) {
        printf("%c ", *it);
    }
    puts("\n");
    //输出FIRST集
    puts("First:");
    for (int i = 0; i < (int)nonTerminalVec.size(); i++) {
        char tmp = nonTerminalVec[i];
        printf("FIRST[%c]: ", tmp);
        for (it = firstSet[tmp].begin(); it != firstSet[tmp].end(); it++) {
            printf(" %c", *it);
        }
        puts("");
    }
    puts("");
    //输出FOLLOW集
    puts("Follow:");
    for (int i = 0; i < (int)nonTerminalVec.size(); i++) {
        char tmp = nonTerminalVec[i];
        printf("FOLLOW[%c]: ", tmp);
        for (it = followSet[tmp].begin(); it != followSet[tmp].end(); it++) {
            printf(" %c", *it);
        }
        puts("");
    }
    puts("");

    //输出预测表
    //判断是否是LL(1)文法，若非LL(1)文法则直接输出错误
    if (!isll1) {
        printf("This grammar isn't an LL(1) grammar!");
    }
    else {
        puts("Prediction Table:");
        printf("    ");
        //输出第一行
        for (it = terminal.begin(); it != terminal.end(); it++) {
            if (*it != epsilon_char)
                printf("%20c", *it);
        }
        printf("%20c", endSymbol_char);
        puts("");
        //对每个非终结符
        //1.输出其字符
        //2.根据表项大小，输出每一个表项
        for (int i = 0; i < (int)nonTerminalVec.size(); i++) {
            char left = nonTerminalVec[i];
            printf("%-4c", left);
            for (it = terminal.begin(); it != terminal.end(); it++) {
                if (*it == epsilon_char)
                    continue;
                set<ProductionRule> tmpSet = predictTable[left][*it];
                if (tmpSet.size() == 0) {
                    for (int i = 0; i < 20; i++)
                        printf(" ");
                }
                else if (tmpSet.size() == 1) {
                    set<ProductionRule>::iterator pit = tmpSet.begin();
                    ProductionRule tmprule = *pit;
                    for (int i = 0; i < 17 - (tmprule.getRPartLength()); i++) printf(" ");
                    printf("%c->%s", tmprule.getLeftPart(), tmprule.getRightPart());
                }
                else {
                    printf("%20s", "not LL(1)");
                }

            }
            //输出结束符#的表项
            set<ProductionRule> tmpSet = predictTable[left][endSymbol_char];
            if (tmpSet.size() == 0) {
                for (int i = 0; i < 20; i++)
                    printf(" ");
            }
            else if (tmpSet.size() == 1) {
                set<ProductionRule>::iterator pit = tmpSet.begin();
                ProductionRule tmprule = *pit;
                for (int i = 0; i < 17 - (tmprule.getRPartLength()); i++) printf(" ");
                printf("%c->%s", tmprule.getLeftPart(), tmprule.getRightPart());
            }
            else {
                printf("%20s", "not LL(1)");
            }
            puts("");
        }
    }
}
void Grammar::generateFirst() {
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
            char X = tmprule.getLeftPart();
            char Y = tmprule.getRightPart()[0];
            int ylen = tmprule.getRPartLength();
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
                        char nxty = tmprule.getRightPart()[j + 1];
                        firstSet[X].insert(firstSet[nxty].begin(), firstSet[nxty].end());
                    }
                    j++;
                    cury = tmprule.getRightPart()[j];
                }
            }
            else {
                errorExit("终结符与非终结符未正确识别");
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
void Grammar::generateFollow() {
    followSet[startSymbol].insert(endSymbol_char);
    int cursz = 0, lasz = -1;
    while (cursz != lasz) {
        lasz = cursz;
        for (int i = 0; i < (int)rules.size(); i++) {
            ProductionRule tmprule = rules[i];
            char A = tmprule.getLeftPart();
            int rlen = tmprule.getRPartLength();
            if (rlen == 1) {
                char B = tmprule.getRightPart()[0];
                if (nonTerminal.find(B) != nonTerminal.end()) {
                    followSet[B].insert(followSet[A].begin(), followSet[A].end());
                }
            }
            else {
                //对于产生式A->αBβ
                for (int i = 0; i < rlen; i++) {
                    char B = tmprule.getRightPart()[i];
                    //若B是非终结符
                    if (nonTerminal.find(B) != nonTerminal.end()) {
                        int k = i + 1;
                        bool betaHasEpsilon = true;
                        //确定FIRST[β]是否包含epsilon
                        //若包含，则将FOLLOW[A]加至FOLLOW[B]中
                        for (; k < rlen; k++) {
                            char beta = tmprule.getRightPart()[k];
                            followSet[B].insert(firstSet[beta].begin(), firstSet[beta].end());
                            if (firstSet[beta].find(epsilon_char) == firstSet[beta].end()) {
                                betaHasEpsilon = false;
                                break;
                            }
                        }
                        if (betaHasEpsilon) {
                            followSet[B].insert(followSet[A].begin(), followSet[A].end());
                        }
                    }
                }
            }
        }
        cursz = 0;
        map<char, set<char>>::iterator it;
        for (it = followSet.begin(); it != followSet.end(); it++) {
            cursz += it->second.size();
        }
    }
    for (int i = 0; i < (int)nonTerminalVec.size(); i++) {
        followSet[nonTerminalVec[i]].erase(epsilon_char);
    }
}
void Grammar::generatePredictTable() {
    //遍历每个产生式
    for (int i = 0; i < (int)rules.size(); i++) {
        ProductionRule tmprule = rules[i];

        char A = tmprule.getLeftPart();
        bool firstHasEpsilon = true;
        //1.对于产生式右部的每一个符号sym
        //2.对于sym的FIRST集中的每个符号it
        //3.将列为it，行为A的预测表项加入该产生式
        //4.重复操作2,3
        //5.若该FIRST集中包epsilon，则回到1，考察下一个符号
        //6.否则退出考察下一个产生式
        for (int j = 0; j < tmprule.getRPartLength(); j++) {
            char sym = tmprule.getRightPart()[j];
            bool hasEpsilon = false;
            set<char>::iterator it;
            for (it = firstSet[sym].begin(); it != firstSet[sym].end(); it++) {
                if (*it == epsilon_char) {
                    hasEpsilon = true;
                    continue;
                }
                predictTable[A][*it].insert(tmprule);
            }
            if (!hasEpsilon) {
                firstHasEpsilon = false;
                break;
            }
        }
        if (firstHasEpsilon) {
            set<char>::iterator it;
            for (it = followSet[A].begin(); it != followSet[A].end(); it++) {
                predictTable[A][*it].insert(tmprule);
            }
        }
    }
    //判断是不是LL(1)文法
    isll1 = true;
    set<char>::iterator it;
    for (int i = 0; i < (int)nonTerminalVec.size(); i++) {
        char left = nonTerminalVec[i];
        for (it = terminal.begin(); it != terminal.end(); it++) {
            if (*it == epsilon_char)
                continue;
            set<ProductionRule> tmpSet = predictTable[left][*it];
            if (tmpSet.size() > 1) {
                isll1 = false;
                break;
            }
            if (!isll1) break;
        }
    }
}
bool Grammar::startParse() {
    if (!isll1)
        return false;
    stack<char> s;
	vector<char> tmpvec;
    s.push(endSymbol_char);
	tmpvec.push_back(endSymbol_char);
    s.push(nonTerminalVec[0]);
	tmpvec.push_back(nonTerminalVec[0]);
    bool flag = true;
    char a;
    scanf(" %c", &a);

    while (flag) {
		for (int i = 0; i < (int)tmpvec.size(); i++)
			printf("%c", tmpvec[i]);
		for (int i = 0; i < 20 - tmpvec.size(); i++) 
			printf(" ");
		printf("%c\n", a);
        char X = s.top();
        if (terminal.find(X) != terminal.end()) {
            if (X != a) {
                return errorExit(parse_error_str, X, a);
            }
            s.pop();
			tmpvec.pop_back();
            scanf(" %c", &a);
        }
        else if (X == endSymbol_char) {
            if (X == a) {
                flag = false;
                s.pop();
				tmpvec.pop_back();
            }
            else {
                return errorExit(parse_error_str, X, a);
            }
        }
        else if (predictTable[X][a].size() == 1) {
            s.pop();
			tmpvec.pop_back();
            ProductionRule tmprule = *(predictTable[X][a].begin());
            if (tmprule.getRPartLength() == 1 && tmprule.getRightPart()[0] == epsilon_char)
                continue;
            for (int i = tmprule.getRPartLength() - 1; i >= 0; i--) {
				char tmpc = tmprule.getRightPart()[i];
                s.push(tmpc);
				tmpvec.push_back(tmpc);
            }
        }
        else {
            return errorExit(parse_error_str, X, a);
        }
    }
    puts(parse_succeed);
	return true;
}
