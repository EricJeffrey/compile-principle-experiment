#include "Exp1WordAnalyzer.h"
#include "Exp2Grammar.h"

/*
错误处理
输出错误信息以及额外信息
实验二中用到参数x以及a
*/
ostringstream scout;
bool errorExit(const string &str, ostringstream & scout, char x = 0, char a = 0) {
    scout << endl << "!!!!!!!!!!!!!!!!!!!!" << endl << str << endl;
    if (x != 0 || a != 0)
        scout << "x: " << x << "    a: " << a;
    scout << endl << "!!!!!!!!!!!!!!!!!!!!" << endl;
	return false;
}

int main() {
    string data = "\
        /*存放(单词，种别码)的二元组*/\
        struct wordTuple {\
        char *word;\
        int len;\
        int typeCode;\
        void setWrod(char *source, int sz, int tc) {\
            len = sz + 3;\
            word = (char *)malloc(len);\
            memset(word, 0, sizeof(word));\
            strncpy(word, source, sz);\
            word[sz] = 0;\
            typeCode = tc;\
        }";
    //实验一
    Exp1WordAnalyzer t(data);
    cout << t.getOutputStr();
    //实验二
    data = "8\n\
E TG\n\
G +TG\n\
G $\n\
T FH\n\
H *FH\n\
H $\n\
F (E)\n\
F i\n\
4\n\
i+i * i*iiii*sdfadg asd i + i#\ni*i+i#\nislgais#\ni*(i + i * (i + i * i + i + i))#";
    Exp2Grammar s(data);
    cout << s.getOutputStr();
    system("pause");
}

/*
实验二：
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
B $

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