#ifndef _GOBANG_H
#define _GOBANG_H
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <time.h>

// 头文件，存有必要的宏及变量和函数引用

#define SIZE 15
#define CHARSIZE 2
#define MAXLEN 10
#define M 5

#define DEEP 6
#define RANGE 5
#define SCOPE 2
#define EXTENDMAX 2
#define MAXPOINT 14
#define TIMELIMIT 15
#define ZOBRISTMAPMAX 21474895

#define DENOMINATOR 10000
#define HUOWU 100000
#define HUOSI 10000
#define CHONGSI 1000
#define HUOSAN 800
#define MIANSAN 300
#define HUOER 200
#define MIANER 100
#define SHA 2400
#define KUAISHA 1600
#define KUAIXIAOSHA 1200
#define JIAOCHA 1000
#define YITUAN 700
#define YIXIAOTUAN 550

extern char empBoard[][SIZE*CHARSIZE+1];
extern int inBoard[][SIZE];
extern char disBoard[][SIZE*CHARSIZE+1];
extern char play1Pic[];
extern char play1CurPic[];
extern char play2Pic[];
extern char play2CurPic[];
extern char line[];
extern int quit;
extern int n;
extern int mode;
extern int sign;

extern int extend;
extern int matchTimes;
extern long int totalTimes;
extern int wholeScore;
extern float factor;
extern int balanceScore;
extern int flooor;
extern int corr;
extern int hengMAX,zongMAX;
extern int noreturn;
extern unsigned long long chessKey;

extern int chessScore[SIZE][SIZE];
extern unsigned long long zobristBlackMap[SIZE][SIZE];
extern unsigned long long zobristWriteMap[SIZE][SIZE];
extern int alpha[DEEP+EXTENDMAX+1];
extern int beta[DEEP+EXTENDMAX+1];
extern int xMax[DEEP+EXTENDMAX+1];
extern int xMin[DEEP+EXTENDMAX+1];
extern int yMax[DEEP+EXTENDMAX+1];
extern int yMin[DEEP+EXTENDMAX+1];

void init();
void gameStart();
void innerBoard();
void displayBoard();
int mygetline(char [],int);
void player();
void pvp();
void pve();
void getPosition(int *pnum1,int *pnum2);
int judge(int *banned);
void AI();

void wholeScoreUpdate();
void zobristInit();
void zobrist();
void zobristMapReset();

int isBanned(int y,int x);
void isFiveOrLong(int y,int x,int color,int dir,int *plive5,int *plong);
void isHuoSiOrChongSi(int y,int x,int color,int dir,int *plive4,int *pdash4);
void isHuoSanOrMianSan(int y,int x,int color,int dir,int *plive3,int *pdash3);
void isHuoErOrMianEr(int y,int x,int color,int dir,int *plive2,int *pdash2);


/* initializes mt[NN] with a seed */
void init_genrand64(unsigned long long seed);

/* initialize by an array with array-length */
/* init_key is the array for initializing keys */
/* key_length is its length */
void init_by_array64(unsigned long long init_key[], 
             unsigned long long key_length);

/* generates a random number on [0, 2^64-1]-interval */
unsigned long long genrand64_int64(void);


/* generates a random number on [0, 2^63-1]-interval */
long long genrand64_int63(void);

/* generates a random number on [0,1]-real-interval */
double genrand64_real1(void);

/* generates a random number on [0,1)-real-interval */
double genrand64_real2(void);

/* generates a random number on (0,1)-real-interval */
double genrand64_real3(void);


#endif