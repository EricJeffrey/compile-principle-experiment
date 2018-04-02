//vs使用此宏定义后可以使用gets, freopen等
#define _CRT_SECURE_NO_WARNINGS
/*
编译原理实验一
词法分析

申敬飞
2018/3/28
*/
 
/*
预定义单词种别码
*/
#define includev$ 0
#define definev$ 1
#define mainv$ 2
#define ifv$ 3
#define thenv$ 4
#define whilev$ 5
#define dov$ 6
#define staticv$ 7
#define intv$ 8
#define doublev$ 9
#define structv$ 10
#define breakv$ 11
#define elsev$ 12
#define longv$ 13
#define switchv$ 14
#define casev$ 15
#define typedefv$ 16
#define charv$ 17
#define returnv$ 18
#define constv$ 19
#define floatv$ 20
#define shortv$ 21
#define continuev$ 22
#define forv$ 23
#define voidv$ 24
#define sizeofv$ 25
#define defaultv$ 26
#define IDv$ 27
#define integerv$ 28
#define realv$ 29
#define addv$ 30
#define addequalv$ 31
#define addaddv$ 32
#define subv$ 33
#define subsubv$ 34
#define subequalv$ 35
#define multiplev$ 36
#define multipleequalv$ 37
#define dividev$ 38
#define divideequalv$ 39
#define colonv$ 40
#define equalv$ 41
#define lessv$ 42
#define notequalv$ 43
#define lessequalv$ 44
#define greaterv$ 45
#define greaterequalv$ 46
#define assign$ 47
#define semicolonv$ 48
#define leftbracketv$ 49
#define rightbracketv$ 50
#define realev$ 51
#define undefinedv$ 52
#define stringv$ 53

/*
关键字，标识符，整数，实数种别码
#include	0	       	integer		28
#define		1	       	real	    29
main		2	       	+			30
if		    3	       	+=			31
then		4	       	++			32
while		5	       	-			33
do			6	       	--			34
static		7	       	-=			35
int		    8	       	*			36
double		9	       	*=			37
struct		10	       	/			38
break		11	       	/=			39
else		12	       	:			40
long		13	       	==			41
switch		14	       	<			42
case		15	       	!=			43
typedef		16	       	<=			44
char		17	       	>			45
return		18	       	>=			46
const		19	       	=			47
float		20	       	;			48
short		21	       	(			49
continue    22	       	)			50
for			23	       	reale		51
void		24	       	undefined   52
sizeof		25			string		53
default		26			character	55
ID			27
*/

#include <cstdio>
#include <cstdlib>
#include <algorithm>
#include <cstring>
#include <cmath>
#include <Windows.h>

using namespace std;

/*待处理串最大长度*/
const int maxn = 1e5 + 5;
const char *codeToStr[] = {"#include","#define", "main", "if", "then", "while", "do", "static", "int", "double", "struct", 
"break", "else", "long", "switch", "case", "typedef", "char", "return", "const", "float", "short", "continue", "for", 
"void", "sizeof", "default", "identifier", "integer", "real", "add", "addequal", "addadd", "sub", "subsub", "subequal", 
"multiple", "multipleequal", "divide", "divideequal", "colon", "equal", "less", "notequal", "lessequal", "greater", 
"greaterequal", "assign$", "semicolon", "leftbracket", "rightbracket", "reale", "undefined", "string"};

/*存放(单词，种别码)的二元组*/
struct wordTuple {
    char *word;
    int len;
    int typeCode;
	void setWrod(char *source, int sz, int tc) {
		len = sz + 3;
		word = (char *)malloc(len);
		memset(word, 0, sizeof(word));
		strncpy(word, source, sz);
		word[sz] = 0;
		typeCode = tc;
	}
	//输出这个二元组
    void printWord() {
        getTypeCode();
        printf("word: %s", word);
        for (int i = 0; i < 26 - len; i++) printf(" ");
        printf("\t typeCode: %3d\t", typeCode);
        if (typeCode == integerv$)
            puts("整数");
        else if (typeCode == realv$)
            puts("实数");
        else if (typeCode == realev$)
            puts("科学计数法");
        else if (typeCode == IDv$)
            puts("标识符");
        else puts("");
    }
	//识别关键字的种别码
    void getTypeCode() {
        if (typeCode == IDv$) {
            for (int i = 2; i <= 26; i++) {
                if (strcmp(word, codeToStr[i]) == 0) {
                    typeCode = i;
                    break;
                }
            }
        }
    }
};

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
void preprocess(char *input, char *output) {
    int len = strlen(input);
    int i = 0, j = 0;
    bool spaceGot = false, lineCommentGot = false, blockCommentGot = false, stringGot = false;
    while (i < len) {
        char tmpch = input[i];
        //a space like character appeared
        if (!lineCommentGot && !blockCommentGot && !stringGot && !spaceGot && (tmpch == ' ' || tmpch == '\n' || tmpch == '\t' || tmpch == '\r')) {
            spaceGot = true;
            output[j] = ' ';
            i++;
            j++;
            continue;
        }
        //a " character appeared
        if (!lineCommentGot && !blockCommentGot && !stringGot && tmpch == '\"' && i - 1 > 0 && input[i - 1] != '\\' && input[i - 1] != '\'') {
            stringGot = true;
            spaceGot = false;
            output[j] = tmpch;
            j++;
            i++;
            continue;
        }
        //a line comment appeared
        if (!blockCommentGot && !stringGot && tmpch == '/' && i != len - 1 && input[i + 1] == '/') {
            i += 2;
            lineCommentGot = true;
            spaceGot = false;
            continue;
        }
        //a block comment appeared
        if (!lineCommentGot && !stringGot && tmpch == '/' && i != len - 1 && input[i + 1] == '*') {
            i += 2;
            blockCommentGot = true;
            spaceGot = false;
            continue;
        }
        //now a space like has appeared, ignore all of these character
        if (!lineCommentGot && !blockCommentGot && !stringGot && spaceGot && (tmpch == ' ' || tmpch == '\n' || tmpch == '\t' || tmpch == '\r')) {
            i++;
            continue;
        }
        //now all these character is in a line comment
        if (!blockCommentGot && !stringGot && lineCommentGot && tmpch != '\n') {
            i++;
            continue;
        }
        //now all these character is a block comment
        if (!lineCommentGot && !stringGot && blockCommentGot && (tmpch != '*' || (i != len - 1 && input[i + 1] != '/'))) {
            i++;
            continue;
        }
        //now the line comment is going to end
        if (!stringGot && !blockCommentGot && lineCommentGot && tmpch == '\n') {
            i++;
            lineCommentGot = false;
            continue;
        }
        //now the block comment is going to end
        if (!stringGot && !lineCommentGot && blockCommentGot && tmpch == '*' && i != len - 1 && input[i + 1] == '/') {
            blockCommentGot = false;
            i += 2;
            continue;
        }
        //now the string is going to end
        if (!lineCommentGot && !blockCommentGot && stringGot && tmpch == '\"' && i - 1 > 0 && input[i - 1] != '\\'  && input[i - 1] != '\'') {
            stringGot = false;
            output[j] = tmpch;
            j++;
            i++;
            continue;
        }
        //common character, just do it
        output[j] = input[i];
        i++;
        j++;
        spaceGot = blockCommentGot = lineCommentGot = false;
    }
    output[j] = 0;
}

bool islower(char ch) {
	return ch >= 'a' && ch <= 'z';
}
bool isupper(char ch) {
	return ch >= 'A' && ch <= 'Z';
}
bool isdigit(char ch) {
	return ch >= '0' && ch <= '9';
}
void errorExit() {
	printf("Error");
	exit(0);
}

/*
词法分析，将识别出的单词放入一个二元组数组中

input: 待处理代码串
output: 分析结果存放位置
*/
int wordParse(char *input, wordTuple *output) {
    int len = strlen(input), j = 0, i = 0;
    while (i < len) {
        char ch = input[i];
		//标识符
        if (islower(ch) || isupper(ch) || ch == '_') {
            int k = i;
            char tmpch = input[k];
            while (islower(tmpch) || isupper(tmpch) || isdigit(tmpch) || tmpch == '_') {
                tmpch = input[++k];
            }
            output[j++].setWrod(input + i, k - i, IDv$);
            i = k;
            continue;
        }
        //常数，包括整数和实数
        if (isdigit(ch)) {
            int k = i;
            char tmpch = input[k];
            while (isdigit(tmpch)) {
                tmpch = input[++k];
            }
            //可能是实数
            if (tmpch == '.') {
                tmpch = input[++k];
                while (isdigit(tmpch)) {
                    tmpch = input[++k];
                }
                //可能是科学计数法
                if (tmpch == 'e' || tmpch == 'E') {
                    tmpch = input[++k];
                    if (isdigit(tmpch)) {
                        while (isdigit(tmpch)) {
                            tmpch = input[++k];
                        }
                        output[j++].setWrod(input + i, k - i, realv$);
                        i = k;
                        continue;
                    }
                    else {
						errorExit();
                    }
                }
                //确定是实数
                else {
                    output[j++].setWrod(input + i, k - i, realv$);
                    i = k;
                    continue;
                }
            }
            //可能是科学计数法
            else if (tmpch == 'e' || tmpch == 'E') {
                tmpch = input[++k];
                //科学计数法至少应有一个指数位
                if (isdigit(tmpch)) {
                    while (isdigit(tmpch)) {
                        tmpch = input[++k];
                    }
                    output[j++].setWrod(input + i, k - i, realev$);
                    i = k;
                    continue;
                }
                //错误
                else {
					errorExit();
                }
            }
            //是整数
            else {
                output[j++].setWrod(input + i, k - i, integerv$);
                i = k;
                continue;
            }
        }
		//字符
		if (ch == '\'') {
			if (i + 1 < len) {

			}
			else {
				errorExit();
			}
		}
		//字符串
		if (ch == '"') {

		}
        //宏定义#define和头文件包含#include
		if (ch == '#') {
            if (i + 7 < len && strncmp(input + i, codeToStr[0], 8) == 0) {
                output[j++].setWrod(input + i, 8, includev$);
                i = i + 8;
            }
            else if (i + 6 < len && strncmp(input + i, codeToStr[1], 7) == 0) {
                output[j++].setWrod(input + i, 7, definev$);
                i = i + 7;
            }
            else {
				errorExit();
            }
        }

		i++;
    }
    return j;
}

int main() {
    //输入输出重定向
	freopen("data.out", "w+", stdout);
	freopen("data.in", "r", stdin);

    char *sourceCodeInput = (char *)malloc(maxn);
    char *preprocessOutput = (char *)malloc(maxn);
    memset(preprocessOutput, 0, maxn);
    memset(sourceCodeInput, 0, maxn);

	//读入源代码
    for (int i = 0; scanf("%c", sourceCodeInput + i) != EOF; i++)
        ;
	//预处理
    preprocess(sourceCodeInput, preprocessOutput);

	//输出预处理后的结果
    for (int i = 0; i < (int)strlen(preprocessOutput); i++) {
        printf("%c", preprocessOutput[i]);
    }
	puts("");

	//解析词法
	wordTuple *wordParseOutput = (wordTuple *)malloc(sizeof(wordTuple) * maxn);
	int wordsLen = wordParse(preprocessOutput, wordParseOutput);

	//输出词法分析后的结果
	for (int i = 0; i < wordsLen; i++) {
		wordParseOutput[i].printWord();
	}

    return 0;
}
