/*
编译原理实验一
词法分析

申敬飞
2018/3/28

存在的bug:
	1e-6无法识别
	无法识别包含的头文件（但能够判断#include以及#define)
*/

#include "exp1Header.h"
#include "Exp1WordAnalyzer.h"

//处理串最大长度，种别码到种别码信息映射
const string oneCodeToStr[] = { "#include","#define", "main", "if", "then", "while", "do", "static", "int", "double", "struct",
"break", "else", "long", "switch", "case", "typedef", "char", "return", "const", "float", "short", "continue", "for",
"void", "sizeof", "default", "identifier", "integer", "real", "add", "addequal", "addadd", "sub", "subsub", "subequal",
"multiple", "multipleequal", "divide", "divideequal", "colon", "equal", "less", "notequal", "lessequal", "greater",
"greaterequal", "assign$", "semicolon", "leftbracket", "rightbracket", "reale", "undefined", "string",
"character", "leftbrace", "rightbrace" };
extern bool errorExit(const string &str, char x = 0, char a = 0);

Exp1WordTuple::Exp1WordTuple(const string &source, int tc) {
    word = source;
    len = (int)word.size();
    typeCode = tc;
}
void Exp1WordTuple::printWord() {
    if (typeCode == IDv$) {
        for (int i = 2; i <= 26; i++) {
            if (word == oneCodeToStr[i]) {
                typeCode = i;
                break;
            }
        }
    }
    cout << "word: " << word;
    for (int i = 0; i < 26 - len; i++) 
        cout << ' ';
    cout << '\t' << "typecode: " << typeCode << '\t';
    if (typeCode >= 27)
        cout << oneCodeToStr[typeCode] << endl;
    else
        cout << keyWordDesc << endl;
}
Exp1WordTuple::~Exp1WordTuple() {
    
}

Exp1WordAnalyzer::Exp1WordAnalyzer() {
    freopen("exp-one-data.out.txt", "w+", stdout);
    freopen("exp-one-data.in.txt", "r", stdin);
    string sourceCodeInput;
    string preprocessOutput;

    //读入源代码
    char tmpch;
    while (cin.get(tmpch)) 
        sourceCodeInput += tmpch;
    preprocessOutput.resize(sourceCodeInput.size());
    //预处理
    preprocess(sourceCodeInput, preprocessOutput);
    preprocessOutput.erase(preprocessOutput.find_last_not_of('\0') + 1);

    //输出预处理后的结果
    cout << preprocessOutput << endl;

    //解析词法
    vector<Exp1WordTuple> wordParseOutput;
    int wordsLen = wordParse(preprocessOutput, wordParseOutput);

    //输出词法分析后的结果
    for (int i = 0; i < wordsLen; i++) {
        wordParseOutput[i].printWord();
    }}
bool Exp1WordAnalyzer::isDigit(char ch) {
    return ch >= '0' && ch <= '9';
}
bool Exp1WordAnalyzer::isLetter(char ch) {
    return (ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z');
}

void Exp1WordAnalyzer::preprocess(const string &input, string &output) {
    int len = (int) input.size();
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
        if (!lineCommentGot && !blockCommentGot && !stringGot && tmpch == '"' && i - 1 > 0 && input[i - 1] != '\\' && input[i - 1] != '\'') {
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
        if (!lineCommentGot && !blockCommentGot && stringGot && tmpch == '"' && i - 1 > 0 && input[i - 1] != '\\'  && input[i - 1] != '\'') {
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
int Exp1WordAnalyzer::wordParse(const string &input, vector<Exp1WordTuple> &output) {
        int len = (int) input.size(), i = 0;
        while (i < len) {
            char ch = input[i];
            //标识符
            if (isLetter(ch) || ch == '_') {
                int k = i;
                char tmpch = input[k];
                while (isLetter(tmpch) || isDigit(tmpch) || tmpch == '_') {
                    tmpch = input[++k];
                }
                output.push_back(Exp1WordTuple(input.substr(i, k - i), IDv$));
                i = k;
                continue;
            }
            //常数，包括整数和实数
            if (isDigit(ch)) {
                int k = i;
                char tmpch = input[k];
                while (isDigit(tmpch)) {
                    tmpch = input[++k];
                }
                //可能是实数
                if (tmpch == '.') {
                    tmpch = input[++k];
                    while (isDigit(tmpch)) {
                        tmpch = input[++k];
                    }
                    //可能是科学计数法
                    if (tmpch == 'e' || tmpch == 'E') {
                        tmpch = input[++k];
                        if (isDigit(tmpch)) {
                            while (isDigit(tmpch)) {
                                tmpch = input[++k];
                            }
                            output.push_back(Exp1WordTuple(input.substr(i, k - i), realv$));
                            i = k;
                            continue;
                        }
                        else {
                            errorExit(u8"科学计数法至少应当有一个指数位");
                        }
                    }
                    //确定是实数
                    else {
                        output.push_back(Exp1WordTuple(input.substr(i, k - i), realv$));
                        i = k;
                        continue;
                    }
                }
                //可能是科学计数法
                else if (tmpch == 'e' || tmpch == 'E') {
                    tmpch = input[++k];
                    //科学计数法至少应有一个指数位
                    if (isDigit(tmpch)) {
                        while (isDigit(tmpch)) {
                            tmpch = input[++k];
                        }
                        output.push_back(Exp1WordTuple(input.substr(i, k - i), realev$));
                        i = k;
                        continue;
                    }
                    //错误
                    else {
                        errorExit(u8"科学计数法至少应当有一个指数位");
                    }
                }
                //是整数
                else if (tmpch == ' ' || tmpch == ')' || tmpch == ']' || tmpch == '\n' || tmpch == '\t'
                    || tmpch == '\r' || tmpch == ';' || tmpch == ',') {
                    output.push_back(Exp1WordTuple(input.substr(i, k - i), integerv$));
                    i = k;
                    continue;
                }
				else {
					errorExit(u8"整数或实数后面不能跟非空格");
				}
            }
            //字符
            if (ch == '\'') {
                if (i + 1 < len) {
                    char tmpch = input[i + 1];
                    if (tmpch == '\\' && +3 < len && input[i + 3] == '\'') {
                        output.push_back(Exp1WordTuple(input.substr(i, 4), characterv$));
                        i = i + 4;
                        continue;
                    }
                    else if (tmpch >= 20 && tmpch <= 126 && input[i + 2] == '\'') {
                        output.push_back(Exp1WordTuple(input.substr(i, 3), characterv$));
                        i = i + 3;
                        continue;
                    }
                    else {
                        errorExit(u8"不可显示字符或无字符结束符");
                    }
                }
                else {
                    errorExit(u8"无字符结束符");
                }
            }
            //字符串
            if (ch == '"') {
                if (i + 1 < len) {
                    int k = i + 1;
                    char tmpch = input[k];
                    while (tmpch != '"') {
                        k++;
                        if (tmpch == '\\')
                            k++;
                        if (k >= len)
                            errorExit(u8"无字符串结束符");
                        tmpch = input[k];
                    }
                    output.push_back(Exp1WordTuple(input.substr(i, k - i + 1), stringv$));
                    i = k + 1;
                    continue;
                }
                else {
                    errorExit(u8"无字符串结束符");
                }
            }
            //宏定义#define和头文件包含#include
            if (ch == '#') {
                if (i + 7 < len && input.substr(i, 8) == oneCodeToStr[0]) {
                    output.push_back(Exp1WordTuple(input.substr(i, 8), includev$));
                    int k = i;
                    while (input[k] != '>' && input[k] != 0 && input[k] != EOF)
                        k++;
                    if (k - i >= 23)
                        errorExit(u8"头文件包含过长或缺少包含结束符>");
                    if (input[k] == '>')
                        k++;
                    i = k;
                }
                else if (i + 6 < len && input.substr(i, 7) == oneCodeToStr[1]) {
                    output.push_back(Exp1WordTuple(input.substr(i, 7), definev$));
                    i = i + 7;
                }
                else {
                    errorExit(u8"不是宏定义也不是头文件包含");
                }
            }
            //+ ++ +=
            if (ch == '+') {
                if (i + 1 < len && (input[i + 1] == '+' || input[i + 1] == '=')) {
                    if (input[i + 1] == '+')
                        output.push_back(Exp1WordTuple(input.substr(i, 2), addaddv$));
                    else
                        output.push_back(Exp1WordTuple(input.substr(i, 2), addequalv$));
                    i = i + 2;
                    continue;
                }
                else {
                    output.push_back(Exp1WordTuple(input.substr(i, 1), addv$));
                    i++;
                    continue;
                }
            }
            //- -- -=
            if (ch == '-') {
                if (i + 1 < len && (input[i + 1] == '-' || input[i + 1] == '=')) {
                    if (input[i + 1] == '-')
                        output.push_back(Exp1WordTuple(input.substr(i, 2), subsubv$));
                    else
                        output.push_back(Exp1WordTuple(input.substr(i, 2), subequalv$));
                    i += 2;
                    continue;
                }
                else {
                    output.push_back(Exp1WordTuple(input.substr(i, 1), subv$));
                    i++;
                    continue;
                }
            }
            //* *=
            if (ch == '*') {
                if (i + 1 < len && input[i + 1] == '=') {
                    output.push_back(Exp1WordTuple(input.substr(i, 2), multipleequalv$));
                    i += 2;
                    continue;
                }
                else {
                    output.push_back(Exp1WordTuple(input.substr(i, 1), multiplev$));
                    i++;
                    continue;
                }
            }
            // / /=
            if (ch == '/') {
                if (i + 1 < len && input[i + 1] == '=') {
                    output.push_back(Exp1WordTuple(input.substr(i, 2), divideequalv$));
                    i += 2;
                    continue;
                }
                else {
                    output.push_back(Exp1WordTuple(input.substr(i, 1), dividev$));
                    i++;
                    continue;
                }
            }
            // :
            if (ch == ':') {
                output.push_back(Exp1WordTuple(input.substr(i, 1), colonv$));
                i++;
                continue;
            }
            // = ==
            if (ch == '=') {
                if (i + 1 < len && input[i + 1] == '=') {
                    output.push_back(Exp1WordTuple(input.substr(i, 2), equalv$));
                    i += 2;
                    continue;
                }
                output.push_back(Exp1WordTuple(input.substr(i, 1), assign$));
                i++;
                continue;
            }
            // < <=
            if (ch == '<') {
                if (i + 1 < len && input[i + 1] == '=') {
                    output.push_back(Exp1WordTuple(input.substr(i, 2), lessequalv$));
                    i += 2;
                    continue;
                }
                output.push_back(Exp1WordTuple(input.substr(i, 1), lessv$));
                i++;
                continue;
            }
            // > >=
            if (ch == '>') {
                if (i + 1 < len && input[i + 1] == '=') {
                    output.push_back(Exp1WordTuple(input.substr(i, 2), greaterequalv$));
                    i += 2;
                    continue;
                }
                output.push_back(Exp1WordTuple(input.substr(i, 1), greaterv$));
                i++;
                continue;
            }
            // !=
            if (ch == '!' && i + 1 < len && input[i + 1] == '=') {
                output.push_back(Exp1WordTuple(input.substr(i, 2), notequalv$));
                i += 2;
                continue;
            }
            // ;
            if (ch == ';') {
                output.push_back(Exp1WordTuple(input.substr(i, 1), semicolonv$));
                i++;
                continue;
            }
            // (
            if (ch == '(') {
                output.push_back(Exp1WordTuple(input.substr(i, 1), leftbracketv$));
                i++;
                continue;
            }
            // )
            if (ch == ')') {
                output.push_back(Exp1WordTuple(input.substr(i, 1), rightbracketv$));
                i++;
                continue;
            }
            // {
            if (ch == '{') {
                output.push_back(Exp1WordTuple(input.substr(i, 1), leftbracev$));
                i++;
                continue;
            }
            // }
            if (ch == '}') {
                output.push_back(Exp1WordTuple(input.substr(i, 1), rightbracev$));
                i++;
                continue;
            }
            i++;
        }
        return (int) output.size();
    }
Exp1WordAnalyzer::~Exp1WordAnalyzer() {

}