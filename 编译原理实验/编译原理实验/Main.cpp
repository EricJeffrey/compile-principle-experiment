#include "Exp1WordAnalyzer.h"
#include "Exp2Grammar.h"

/*
错误处理
输出错误信息以及额外信息
实验二中用到参数x以及a
*/
bool errorExit(const string &str, char x = 0, char a = 0) {
    cout << endl << "!!!!!!!!!!!!!!!!!!!!" << endl << str << endl;
    if (x != 0 || a != 0)
        cout << "x: " << x << "    a: " << a;
    cout << endl << "!!!!!!!!!!!!!!!!!!!!" << endl;
	return false;
}

int main() {
    //实验一
    Exp1WordAnalyzer();
    //实验二
    //Exp2Grammar();
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