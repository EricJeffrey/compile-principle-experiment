#include "exp2Header.h"
#include "Exp2ProductionRule.h"
#include "Exp2Grammar.h"

extern bool errorExit(const string &str, ostringstream & scout, char x = 0, char a = 0);
/*
产生式结构
包括产生式左部，右部
*/
Exp2ProductionRule::Exp2ProductionRule(char lpart, string &rpart) {
    leftPart = lpart;
    rightPart = rpart;
    rPartLength = (int) rightPart.size();
}
char Exp2ProductionRule::getLeftPart() {
    return leftPart;
}
string Exp2ProductionRule::getRightPart() {
    return rightPart;
}
int Exp2ProductionRule::getRPartLength() {
    return rPartLength;
}
Exp2ProductionRule::~Exp2ProductionRule() {
    
}

Exp2Grammar::Exp2Grammar(string &data) {
    scin = istringstream(data);
    scout << "--------------------------------------------------" << endl;
    clear();
    readRules();
    generateFirst();
    generateFollow();
    generatePredictTable();
    printGrammar();
    int k;
    scin>> k;
    scin.get();
    while (k > 0 && k--) {
        string tmp;
        getline(scin, tmp);
        startParse(tmp);
    }
    scout << "--------------------------------------------------" << endl << endl;
}
void Exp2Grammar::clear() {
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
void Exp2Grammar::readRules() {
    int t;
    scin>> t;
    char tmpc;
    string tmpstr;
    while (t--) {
        scin>> tmpc >> tmpstr;
        if (startSymbol == endSymbol_char)
            startSymbol = tmpc;
        rules.push_back(Exp2ProductionRule(tmpc, tmpstr));
    }
    getTerminalAndNon();
}
void Exp2Grammar::getTerminalAndNon() {
    for (int i = 0; i < (int)rules.size(); i++) {
        char lp = rules[i].getLeftPart();
        if (nonTerminal.find(lp) == nonTerminal.end()) {
            nonTerminal.insert(lp);
            nonTerminalVec.push_back(lp);
        }
    }
    for (int i = 0; i < (int)rules.size(); i++) {
        string tmpstr = rules[i].getRightPart();
        for (int j = 0; j < rules[i].getRPartLength(); j++) {
            if (tmpstr[j] != 0 && nonTerminal.find(tmpstr[j]) == nonTerminal.end()) {
                terminal.insert(tmpstr[j]);
            }
        }
    }
}
void Exp2Grammar::generateFirst() {
    //终结符：FIRST(X) = X
    for (char ch : terminal) {
        firstSet[ch].insert(ch);
    }
    int cursz = 0;
    int lasz = -1;
    while (cursz != lasz) {
        lasz = cursz;
        //对于每一个产生式进行判断
        for (Exp2ProductionRule tmprule : rules) {
            char X = tmprule.getLeftPart(), Y = tmprule.getRightPart()[0];
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
                errorExit("终结符与非终结符未正确识别", scout);
            }
        }
        //统计first集的大小
        cursz = 0;
        for (auto tmp : firstSet) {
            cursz += (int) tmp.second.size();
        }
    }
}
void Exp2Grammar::generateFollow() {
    followSet[startSymbol].insert(endSymbol_char);
    int cursz = 0, lasz = -1;
    while (cursz != lasz) {
        lasz = cursz;
        for (Exp2ProductionRule tmprule : rules) {
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
        for (auto tmp : followSet) {
            cursz += (int) tmp.second.size();
        }
    }
    for (char ch : nonTerminalVec) {
        followSet[ch].erase(epsilon_char);
    }
}
void Exp2Grammar::generatePredictTable() {
    //遍历每个产生式
    for (Exp2ProductionRule tmprule : rules) {
        char A = tmprule.getLeftPart();
        bool firstHasEpsilon = true;
        //1.对于产生式右部的每一个符号sym
        //2.对于sym的FIRST集中的每个符号it
        //3.将列为it，行为A的预测表项加入该产生式
        //4.重复操作2,3
        //5.若该FIRST集中包epsilon，则回到1，考察下一个符号
        //6.否则退出考察下一个产生式
        for (char sym : tmprule.getRightPart()) {
            bool hasEpsilon = false;
            for (char ch : firstSet[sym]) {
                if (ch == epsilon_char) {
                    hasEpsilon = true;
                    continue;
                }
                predictTable[A][ch].insert(tmprule);
            }
            if (!hasEpsilon) {
                firstHasEpsilon = false;
                break;
            }
        }
        if (firstHasEpsilon) {
            for (char ch : followSet[A]) {
                predictTable[A][ch].insert(tmprule);
            }
        }
    }
    //判断是不是LL(1)文法
    isll1 = true;
    for (char left : nonTerminalVec) {
        for (char ch : terminal) {
            if (ch == epsilon_char)
                continue;
            if (predictTable[left][ch].size() > 1) {
                isll1 = false;
                break;
            }
            if (!isll1) 
                break;
        }
    }
}
void Exp2Grammar::printGrammar() {
    //输出得到的所有产生式
    scout << "production rule:";
    for (auto rule : rules) {
        scout << rule.getLeftPart() << " -> " << rule.getRightPart() << endl;
    }
    scout << endl << "NonTerminal:" << endl;
    //输出非终结符
    set<char>::iterator it;
    for (auto tmp : nonTerminal) {
        scout << tmp << ' ';
    }
    scout << endl << "Terminal:" << endl;
    //输出终结符
    for (auto tmp : terminal) {
        scout << tmp << ' ';
    }
    scout << endl << "First:" << endl;
    //输出FIRST集
    for (char ch : nonTerminalVec) {
        scout << "FIRST[" << ch << "]:";
        for (char tmpch : firstSet[ch]) {
            scout << ' ' << tmpch;
        }
        scout << endl;
    }
    scout << endl << "Follow:" << endl;
    //输出FOLLOW集
    for (char ch : nonTerminalVec) {
        scout << "FOLLOW[" << ch << "]:";
        for (char tmpch : followSet[ch]) {
            scout << ' ' << tmpch;
        }
        scout << endl;
    }
    scout << endl;

    //输出预测表
    //判断是否是LL(1)文法，若非LL(1)文法则直接输出错误
    if (!isll1) {
        scout << "This grammar isn't an LL(1) grammar!";
    }
    else {
        scout << "Prediction Table:" << endl << "    ";
        //输出第一行
        for (char ch : terminal) {
            if (ch != epsilon_char)
                scout << setw(20) << ch;
        }
        scout << setw(20) << endSymbol_char << endl;
        //对每个非终结符
        //1.输出其字符
        //2.根据表项大小，输出每一个表项
        for (char left : nonTerminalVec) {
            scout << left << "   ";
            for (char ch : terminal) {
                if (ch == epsilon_char)
                    continue;
                set<Exp2ProductionRule> tmpSet = predictTable[left][ch];
                int tmpsz = (int)tmpSet.size();
                if (tmpsz == 0) {
                    scout << setw(20) << ' ';
                }
                else if (tmpsz == 1) {
                    Exp2ProductionRule tmprule = *tmpSet.begin();
                    scout << setw(17 - tmprule.getRPartLength()) << " ";
                    scout << tmprule.getLeftPart() << "->" << tmprule.getRightPart();
                }
                else {
                    scout << setw(20) << "not LL(1)";
                    return;
                }
            }
            set<Exp2ProductionRule> tmpSet = predictTable[left][endSymbol_char];
            int tmpsz = (int)tmpSet.size();
            if (tmpsz == 0) {
                scout << setw(20) << ' ';
            }
            else if (tmpsz == 1) {
                Exp2ProductionRule tmprule = *tmpSet.begin();
                scout << setw(17 - tmprule.getRPartLength()) << ' ';
                scout << tmprule.getLeftPart() << "->" << tmprule.getRightPart();
            }
            else {
                scout << setw(20) << "not LL(1)";
                return;
            }
            scout << endl;
        }
    }
}
bool Exp2Grammar::startParse(const string &str) {
    if (!isll1)
        return false;
    stack<char> s;
	vector<char> tmpvec;
    s.push(endSymbol_char);
	tmpvec.push_back(endSymbol_char);
    s.push(nonTerminalVec[0]);
	tmpvec.push_back(nonTerminalVec[0]);
    bool flag = true;
    int stri = 0, len = (int)str.size();

    while (flag && stri < len) {
        scout << endl;
        char a = str[stri];
        for (char ch : tmpvec) {
            scout << ch;
        }
        scout << setw(30 - tmpvec.size() - 1) << ' ';
        scout << str.substr(stri);
        char X = s.top();
        if (terminal.find(X) != terminal.end()) {
            if (X != a) {
                return errorExit(parse_error_str, scout, X, a);
            }
            s.pop();
			tmpvec.pop_back();
            stri++;
        }
        else if (X == endSymbol_char) {
            if (X == a) {
                flag = false;
                s.pop();
				tmpvec.pop_back();
            }
            else {
                return errorExit(parse_error_str, scout, X, a);
            }
        }
        else if (predictTable[X][a].size() == 1) {
            s.pop();
			tmpvec.pop_back();
            Exp2ProductionRule tmprule = *(predictTable[X][a].begin());
            if (tmprule.getRPartLength() == 1 && tmprule.getRightPart()[0] == epsilon_char)
                continue;
            for (int i = tmprule.getRPartLength() - 1; i >= 0; i--) {
				char tmpc = tmprule.getRightPart()[i];
                s.push(tmpc);
				tmpvec.push_back(tmpc);
            }
            scout << setw(40 + stri - len) << ' ';
            scout << tmprule.getLeftPart() << "->" << tmprule.getRightPart();
        }
        else {
            return errorExit(parse_error_str, scout, X, a);
        }
    }
    scout << endl << parse_succeed_str << endl;
	return true;
}

string Exp2Grammar::getOutputStr()
{
    return scout.str();
}

Exp2Grammar::~Exp2Grammar()
{
}

/*

13
E TG
G +E
G $
T FH
H T
H $
F PK
K *K
K $
P (E)
P a
P b
P ^
0

6
S a
S ^
S (T)
T SP
P ,SP
P $
0

6
S Ab
A a
A B
A $
B b
B $
0

5
S ABBA
A a
A $
B b
B $
0

6
S aSe
S B
B bBe
B C
C cCe
C d
0
*/