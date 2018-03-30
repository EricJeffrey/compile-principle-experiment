/**
* 编译原理实验一
* 词法分析
*
* 申敬飞
* 2018/3/28
*/

//vs使用此宏定义后可以使用gets, freopen等
#define _CRT_SECURE_NO_WARNINGS
#include <cstdio>
#include <cstdlib>
#include <algorithm>
#include <cstring>

using namespace std;

/*待处理串最大长度*/
const int maxn = 1e6 + 5;

/**
* 预处理，删除多余的空格，换行，回车，制表符，注释
*
* spaceGot：已经遇到过一个“空格类字符”，接下来的所有“空格类”字符都会被忽略
* lineCommentGot：已经遇到了“//”，接下来的字符都会被忽略，直到遇到回车字符
* blockCommentGot：已经遇到了“/*”，接下来的字符都会被忽略，直到遇到“*#(去掉#)/”
* stringGot：应遇到了“"”，接下来的字符都会被忽略，直到遇到下一个“"”
*
* 当前处理字符位于
* 1.字符串中时，忽略空格，注释的影响
* 2.注释中时，忽略字符串，空格，另一种注释的影响
*
* input: 待处理的串，串最大长度由全局变量maxn指定
* output: 结果串存放位置，串最大长度由全局变量maxn指定
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
关键字，标识符，整数，实数种别码
#include	0			default		26
#define		1			ID			27
main		2			integer		28
if			3			real		29
then		4			+			30
while		5			+=			31
do			6			++			32
static		7			-			33
int			8			--			34
double		9			-=			35
struct		10			*			36
break		11			*=			37
else		12			/			38
long		13			/=			39
switch		14			:			40
case		15			==			41
typedef		16			< 			42
char		17			!=			43
return		18			<= 			44
const		19			>			45
float		20			>=			46
short		21			=			47
continue	22			;			48
for			23			(			49
void		24			)			50
sizeof		25			

*/

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
