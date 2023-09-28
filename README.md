# UCAS_Gobang_C
本程序为国科大C程序设计五子棋个人大作业，仅供学习和对战测试用，请勿抄袭。
该五子棋程序水平大致为网络上普通五子棋AI中等级水平，曾获得21级wcg班前二，但不敌往届第一。
该程序基于Linux环境编写，终端需在“配置文件首选项中”中的“兼容性”选择GBK编码和宽度有歧义的字符为“宽”，以在终端中获得正确的棋盘显示效果。
该程序运行默认有禁手存在，如想要进行无禁手对战请自行将“AI.c”文件第179行的注释改为正常代码（即删去最前面的“// ”）后，重新编译。
代码中有完整的中文注释，如果在ide中打开出现乱码，请自行在ide中选择GKB编码。
对局中输入你想要落子的点的坐标，如“i8”（不区分字母和数字的先后顺序，也不区分大小写），即代表在棋盘上i8坐标落子。其余操作遵照程序中指引即可。

This program is a personal homework for the C program design of the UCAS, only for learning and battle testing, please do not plagiarize.
The Gobang program level is roughly the intermediate level of ordinary Gobang AI on the Internet, and it has won the top two in the 21-level WCG's class, but it is no match for the first in previous editions.
The program is based on the Linux environment, and the terminal needs to select the GBK encoding and width ambiguous characters as "wide" in the "Compatibility" in the "Profile Preferences" to get the correct checkerboard display in the terminal.
If you want to play a no-forbidden hand battle, please change the comment on line 179 of the "AI.c" file to normal code (that is, delete the first "// ") and recompile.
There are complete Chinese comments in the code, if there is garbled characters when opened in the IDE, please select the GKB encoding in the IDE.
Enter the coordinates of the point you want to fall on in the game, such as "i8" (not distinguishing the order of letters and numbers, and not case sensitive), that is, the i8 coordinate drop on the board. Follow the instructions in the procedure.
