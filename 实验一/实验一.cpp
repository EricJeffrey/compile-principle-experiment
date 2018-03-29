/**
* 编译原理实验一
* 词法分析
*
* 申敬飞
* 2018/3/28
*/

#define _CRT_SECURE_NO_WARNINGS
#include <cstdio>
#include <cstdlib>
#include <algorithm>
#include <cstring>

using namespace std;
const int maxn = 1e6 + 5;

/**
* 预处理，删除多余的空格，换行，回车，制表符注释
* 字符串中，忽略空格，注释的影响
* 注释中，忽略字符串，空格，另一种注释的影响
* 空格状态能被字符串和注释打断被打断
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


int main() {
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
