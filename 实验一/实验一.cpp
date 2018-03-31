/*
编译原理实验一
词法分析

申敬飞
2018/3/28
*/

//vs使用此宏定义后可以使用gets, freopen等
#define includev$ 0
#define defaultv$ 26
#define definev$ 1
#define IDv$ 27
#define mainv$ 2
#define integerv$ 28
#define ifv$ 3
#define realv$ 29
#define realev$ 51
#define thenv$ 4
#define addv$ 30
#define whilev$ 5
#define addequalv$ 31
#define dov$ 6
#define addaddv$ 32
#define staticv$ 7
#define subv$ 33
#define intv$ 8
#define subsubv$ 34
#define doublev$ 9
#define subequalv$ 35
#define structv$ 10
#define multiplev$ 36
#define breakv$ 11
#define multipleequalv$ 37
#define elsev$ 12
#define dividev$ 38
#define longv$ 13
#define divideequalv$ 39
#define switchv$ 14
#define colonv$ 40
#define casev$ 15
#define equalv$ 41
#define typedefv$ 16
#define lessv$ 42
#define charv$ 17
#define notequalv$ 43
#define returnv$ 18
#define lessequalv$ 44
#define constv$ 19
#define greaterv$ 45
#define floatv$ 20
#define greaterequalv$ 46
#define shortv$ 21
#define assign$ 47
#define continuev$ 22
#define semicolonv$ 48
#define forv$ 23
#define leftbracketv$ 49
#define voidv$ 24
#define rightbracketv$ 50
#define sizeofv$ 25
#define undefinedv$ sub1

#define errorinfo printf("Error");system("exit -1");

#include <cstdio>
#include <cstdlib>
#include <algorithm>
#include <cstring>
#include <cmath>
#include <ctype.h>

using namespace std;

/*
关键字，标识符，整数，实数种别码
#include	0	       	default		26
#define		1	       	ID			27
main		2	       	integer		28
if		    3	       	real	    29
then		4	       	+			30
while		5	       	+=			31
do			6	       	++			32
static		7	       	-			33
int		    8	       	--			34
double		9	       	-=			35
struct		10	       	*			36
break		11	       	*=			37
else		12	       	/			38
long		13	       	/=			39
switch		14	       	:			40
case		15	       	==			41
typedef		16	       	<			42
char		17	       	!=			43
return		18	       	<=			44
const		19	       	>			45
float		20	       	>=			46
short		21	       	=			47
continue    22	       	;			48
for			23	       	(			49
void		24	       	)			50
sizeof		25	       	undefined   -1

*/

/*待处理串最大长度*/
const int maxn = 1e6 + 5;

/*存放(单词，种别码)的二元组*/
struct wordTuple {
    char *word;
    int typeCode;
    void setWrod(char *source, int sz, int tc) {
        word = (char *)malloc(sz + 1);
        strncpy(word, source, sz);
        typeCode = tc;
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
        if (!lineCommentGot && !blockCommentGot && !stringGot && tmpch == '\"') {
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
        if (!lineCommentGot && !blockCommentGot && stringGot && tmpch == '\"') {
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

/*
词法分析，将识别出的单词放入一个二元组数组中

input: 待处理代码串
output: 分析结果存放位置
*/
int wordParse(char *input, wordTuple *output) {
    int len = strlen(input), j = 0, i = 0;

    while (i < len) {
        char ch = input[i];
        if (islower(ch) || isupper(ch) || ch == '_') {
            int k = i;
            char tmpch = input[k];
            while (islower(ch) || isupper(ch) || isdigit(ch) || ch == '_') {
                tmpch = input[++k];
            }
            output[j++].setWrod(input + i, k - i, IDv$);
            i = k;
            continue;
        }
        //可能是整数
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
                        errorinfo
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
                    output[j++].setWrod(input + i, k - i, realv$);
                    i = k;
                    continue;
                }
                //错误
                else {
                    errorinfo
                }
            }
            //是整数
            else {
                output[j++].setWrod(input + i, k - i, integerv$);
                i = k;
                continue;
            }
        }
    }
    return j;
}

int main() {
    //输入输出重定向
    freopen("data.in", "r", stdin);
    freopen("data.out", "w+", stdout);
    char *input = (char *)malloc(maxn);
    char *output = (char *)malloc(maxn);
    memset(output, 0, maxn);
    memset(input, 0, maxn);
    for (int i = 0; scanf("%c", input + i) != EOF; i++)
        ;
    preprocess(input, output);
    for (int i = 0; i < (int)strlen(output); i++) {
        printf("%c", output[i]);
    }
    return 0;
}
