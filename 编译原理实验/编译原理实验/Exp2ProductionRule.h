#pragma once
#include "exp2Header.h"

/*
产生式结构
包括产生式左部，右部
*/
class Exp2ProductionRule {
private:
    char leftPart;
    string rightPart;
    int rPartLength;
public:
    Exp2ProductionRule(char lpart, string &rpart);
    bool operator < (Exp2ProductionRule t) const {
        if (leftPart < t.leftPart) return true;
        else if (leftPart == t.leftPart) {
            if (rPartLength < t.rPartLength) return true;
            else if (rPartLength == t.rPartLength) {
                if (rightPart == t.rightPart) return true;
                else return false;
            }
            else return false;
        }
        else return false;
    }
    char getLeftPart();
    string getRightPart();
    int getRPartLength();
    ~Exp2ProductionRule();
};