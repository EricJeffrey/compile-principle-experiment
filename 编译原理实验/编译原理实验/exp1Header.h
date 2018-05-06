#pragma once
#include "commonHeader.h"

//实验一
/*
关键字，标识符，整数，实数种别码
#include	0	       	real	    29
#define		1	       	+			30
main		2	       	+=			31
if		    3	       	++			32
then		4	       	-			33
while		5	       	--			34
do			6	       	-=			35
static		7	       	*			36
int		    8	       	*=			37
double		9	       	/			38
struct		10	       	/=			39
break		11	       	:			40
else		12	       	==			41
long		13	       	<			42
switch		14	       	!=			43
case		15	       	<=			44
typedef		16	       	>			45
char		17	       	>=			46
return		18	       	=			47
const		19	       	;			48
float		20	       	(			49
short		21	       	)			50
continue    22	       	reale		51
for			23	       	undefined   52
void		24	       	string		53
sizeof		25			character	54
default		26			{           55
ID			27          }           56
integer		28

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
#define characterv$ 54
#define leftbracev$ 55
#define rightbracev$ 56

const string keyWordDesc = "key word";