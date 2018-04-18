#define _CRT_SECURE_NO_WARNINGS
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <set>
#include <vector>
#include <map>
#include <algorithm>

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
	bool operator < (ProductionRule t) const {
		return rPartLength < t.rPartLength;
	}
};

/*
定义文法
文法由非终结符，终结符，产生式组成
产生式中的ε使用$代替
*/
struct Grammar {
	/*
	规定产生式的长度最大值
	*/
    int ruleMaxLen = 50;
	/*
	记录开始符
	*/
    char startSymbol = endSymbol_char;
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
	构造文法

	读取文法
	构造FIRST集
	构造FOLLOW集
	*/
    Grammar() {
        readRules();
        generateFirst();
        generateFollow();
		generatePredictTable();
    }
    /*
    确定终结符与非终结符
    将每个产生式的左部作为一个非终结符加入相应的集合中
    确定非终结符后，将每个非非终结符加入终结符集合中
    */
    void getTerminalAndNon() {
        for (int i = 0; i < (int)rules.size(); i++) {
			char lp = rules[i].leftPart;
			if (nonTerminal.find(lp) == nonTerminal.end()) {
				nonTerminal.insert(lp);
				nonTerminalVec.push_back(lp);
			}
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
            if (startSymbol == endSymbol_char)
                startSymbol = tmpc;
            rules.push_back(ProductionRule(tmpc, tmpstr));
        }
        getTerminalAndNon();
    }
    /*
	输出文法
    
	输出产生式、终结符以及非终结符
    */
    void printGrammar() {
		//输出得到的所有产生式
        puts("production rule:");
        for (int i = 0; i < (int)rules.size(); i++) {
            printf("%c -> %s\n", rules[i].leftPart, rules[i].rightPart);
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
		for (int i = 0; i < (int) nonTerminalVec.size(); i++) {
			char tmp = nonTerminalVec[i];
			printf("FOLLOW[%c]: ", tmp);
			for (it = followSet[tmp].begin(); it != followSet[tmp].end(); it++) {
				printf(" %c", *it);
			}
			puts("");
		}
		puts("");
		//输出预测表
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
		for (int i = 0; i < (int) nonTerminalVec.size(); i++) {
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
					for (int i = 0; i < 17 - pit->rPartLength; i++) printf(" ");
					printf("%c->%s", pit->leftPart, pit->rightPart);
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
				for (int i = 0; i < 17 - pit->rPartLength; i++) printf(" ");
				printf("%c->%s", pit->leftPart, pit->rightPart);
			}
			else {
				printf("%20s", "not LL(1)");
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
        followSet[startSymbol].insert(endSymbol_char);
        int cursz = 0, lasz = -1;
        while (cursz != lasz) {
            lasz = cursz;
            for (int i = 0; i < (int)rules.size(); i++) {
                ProductionRule tmprule = rules[i];
                char A = tmprule.leftPart;
                int rlen = tmprule.rPartLength;
                if (rlen == 1) {
                    char B = tmprule.rightPart[0];
                    if (nonTerminal.find(B) != nonTerminal.end()) {
                        followSet[B].insert(followSet[A].begin(), followSet[A].end());
                    }
                }
                else {
                    //对于产生式A->αBβ
                    for (int i = 0; i < rlen; i++) {
                        char B = tmprule.rightPart[i];
                        //若B是非终结符
                        if (nonTerminal.find(B) != nonTerminal.end()) {
                            int k = i + 1;
                            bool betaHasEpsilon = true;
                            //确定FIRST[β]是否包含epsilon
                            //若包含，则将FOLLOW[A]加至FOLLOW[B]中
                            for (; k < rlen; k++) {
                                char beta = tmprule.rightPart[k];
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
		for (int i = 0; i < (int) nonTerminalVec.size(); i++) {
			followSet[nonTerminalVec[i]].erase(epsilon_char);
		}
    }
	/*
	构造预测分析表
	*/
	void generatePredictTable() {
		//遍历每个产生式
		for (int i = 0; i < (int)rules.size(); i++) {
			ProductionRule tmprule = rules[i];
			char A = tmprule.leftPart;
			bool firstHasEpsilon = false;
			//1.对于产生式右部的每一个符号sym
			//2.对于sym的FIRST集中的每个符号it
			//3.将列为it，行为A的预测表项加入该产生式
			//4.重复操作2,3
			//5.若该FIRST集中包epsilon，则回到1，考察下一个符号
			//6.否则退出考察下一个产生式
			for (int j = 0; j < tmprule.rPartLength; j++) {
				char sym = tmprule.rightPart[j];
				bool hasEpsilon = false;
				set<char>::iterator it;
				for (it = firstSet[sym].begin(); it != firstSet[sym].end(); it++) {
					if (*it == epsilon_char) {
						hasEpsilon = true;
						firstHasEpsilon = true;
						continue;
					}
					predictTable[A][*it].insert(tmprule);
				}
				if (!hasEpsilon) {
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
	}
};

int main() {
    freopen("exp-two-data.in.txt", "r", stdin);
    freopen("exp-two-data.out.txt", "w+", stdout);
    Grammar().printGrammar();
    return 0;
}

/*

习题2
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

习题1
S a
S ^
S (T)
T SP
P ,SP
P $

4.2
E TG
G +TG
G $
T FH
H *FH
H $
F (E)
F i

习题3.1
S Abc
A a
A $
B b

习题3.2
S Ab
A a
A B
A $
B b
B $

习题3.3
S ABBA
A a
A $
B b
B $

习题3.4
S aSe
S B
B bBe
B C
C cCe
C d

*/