#include "gobang.h"

char line[MAXLEN];
//棋盘使用的是GBK编码，每一个中文字符占用2个字节。

//空棋盘模板 
char empBoard[SIZE][SIZE*CHARSIZE+1] = 
{
		"┏┯┯┯┯┯┯┯┯┯┯┯┯┯┓",
		"┠┼┼┼┼┼┼┼┼┼┼┼┼┼┨",
		"┠┼┼┼┼┼┼┼┼┼┼┼┼┼┨",
		"┠┼┼┼┼┼┼┼┼┼┼┼┼┼┨",
		"┠┼┼┼┼┼┼┼┼┼┼┼┼┼┨",
		"┠┼┼┼┼┼┼┼┼┼┼┼┼┼┨",
		"┠┼┼┼┼┼┼┼┼┼┼┼┼┼┨",
		"┠┼┼┼┼┼┼┼┼┼┼┼┼┼┨",
		"┠┼┼┼┼┼┼┼┼┼┼┼┼┼┨",
		"┠┼┼┼┼┼┼┼┼┼┼┼┼┼┨",
		"┠┼┼┼┼┼┼┼┼┼┼┼┼┼┨",
		"┠┼┼┼┼┼┼┼┼┼┼┼┼┼┨",
		"┠┼┼┼┼┼┼┼┼┼┼┼┼┼┨",
		"┠┼┼┼┼┼┼┼┼┼┼┼┼┼┨",
		"┗┷┷┷┷┷┷┷┷┷┷┷┷┷┛"
};
//此数组存储用于显示的棋盘 
char disBoard[SIZE][SIZE*CHARSIZE+1];
 
char play1Pic[]="●";//黑棋子;
char play1CurPic[]="▲"; 

char play2Pic[]="◎";//白棋子;
char play2CurPic[]="△";

//此数组用于记录当前的棋盘的格局 
int inBoard[SIZE][SIZE];
// 一些必要的变量
int quit;
int n;
int mode;
int sign;

int extend;
int noreturn;
int wholeScore;
float factor;
int balanceScore;
int matchTimes;
int flooor;
int corr;
int hengMAX,zongMAX;
long int totalTimes;
unsigned long long chessKey=0;

int chessScore[SIZE][SIZE];
unsigned long long zobristBlackMap[SIZE][SIZE];
unsigned long long zobristWriteMap[SIZE][SIZE];

int alpha[DEEP+EXTENDMAX+1];
int beta[DEEP+EXTENDMAX+1];
int xMax[DEEP+EXTENDMAX+1];
int xMin[DEEP+EXTENDMAX+1];
int yMax[DEEP+EXTENDMAX+1];
int yMin[DEEP+EXTENDMAX+1];

// 主程序
int main(){
    int len;
    begin:
    n=0;
    quit=0;
    system("clear");
    init(); 
    printf("pvp: please press 1\n");
    printf("pve: please press 2\n");
    printf("press 'quit' to quit\n");
    printf("Work of 戴玉鸣\n");
    // 获得玩家选择的游玩模式
    if((len=mygetline(line,MAXLEN))>0){
        if(strcmp(line,"quit\n")==0)
            return 0;
        line[len-1]='\0';
        mode = atoi(line);
    }else{
        return 0;
    }
    switch(mode){
        case 1:
            pvp();   
            break;
        case 2:
            pve();    
            break;
        default:
            printf("Error input, please input mode again!\n");      
            goto begin;
    }
    goto begin;
    return 0;
}

// pve主程序
void pve(){
    system("clear");
    // 必要的变量设置及初始化
    int order,len,banned,i,j;
    int *pbanned=&banned;
    // 对AI初始落子考虑范围进行初始化，设定为全部棋盘
    xMax[0]=14;
    xMin[0]=0;
    yMax[0]=14;
    yMin[0]=0;
    // 对棋子分数进行初始化，全部重置为零
    for(i=0;i<SIZE;i++){
        for(j=0;j<SIZE;j++){
            chessScore[i][j]=0;
        }
    }
    // 对zobristMap进行重置，对每个落子点的黑白子的特征值用64位随机数填充
    zobristInit();
    zobristMapReset();
    banned=0;
    // 输出屏幕显示
    printf("Please choose the order:\n");
    printf("(1) player goes first: press 1\n");
    printf("(2) AI goes first: press 2\n");
    // 玩家选择先后手
    fool:
    // 获得玩家先后手选择，若玩家为先手，sign为-1，代表电脑执白，反之亦然
    if((len=mygetline(line,MAXLEN))>0){
        if(strcmp(line,"quit\n")==0)
            return;
        line[len-1]='\0';
        order = atoi(line);
    }
    switch(order){
        case 1:
            sign = -1; 
            break;
        case 2:
            sign = 1; 
            break;
        default:
            printf("Error input, please input order again!\n");      goto fool;
    }
    innerBoard();
    displayBoard();
    // AI先手落子，直接落子在棋盘中央，并且设定全局分为1000分
    if(sign==1){
        alpha[0]=wholeScore=1000;
        inBoard[(SIZE-1)/2][(SIZE-1)/2]=3;
        chessKey^=zobristBlackMap[(SIZE-1)/2][(SIZE-1)/2];
        innerBoard();
        displayBoard();
        ++n;
    // 否则设定全局分为-1000分
    }else{
        alpha[0]=wholeScore=-1000;
    }
    // 如果没有出现禁手或者没有一方胜利或者棋盘没有下满，循环进行双方落子
    while(judge(pbanned)&&n<225){
        player();
        if(quit==1)
            return;
        innerBoard();  
        displayBoard();   
        if(!judge(pbanned)||n>=225)
            break;
        AI();
        ++n;
        innerBoard();
        displayBoard();
    }
    // 根据情况不同输出对局结果
    if(n<225){
        if(banned==0){
            if((n+((sign==1)?0:1))%2)
                printf("AI win!\n");
            else
                printf("Player win!\n");
        }else{
            printf("Player placed the forbidden hand, lose!\n");
        }
    }else{
        printf("Draw!\n");
    }
    printf("Press any key to contiune\n");
    getchar();
    return;
}

// pvp主程序
void pvp(){
    innerBoard(); 
    displayBoard(); 
    int banned;
    int *pbanned=&banned;
    banned=0;
    while(judge(pbanned)){
        player();   
        if(quit==1)
            return;
        innerBoard(); 
        displayBoard(); 
    }
    if(banned==0){
        if(n%2==1)
            printf("Player1 win!\n");
        else
            printf("Player2 win!\n");
    }else{
        printf("Player1 lose!\n");
    }
    printf("Press any key to contiune\n");
    getchar();
    return;
}

// 玩家交互函数
void player(){
    int num1,num2;
    // pvp模式
    if(mode==1){
        if(n%2==1){
            printf("Player2's turn\n");
            printf("press 'quit' to quit\nCreator : Dai YuMing\n");
        }else{ 
            printf("Player1's turn\n");
            printf("press 'quit' to quit\nCreator : Dai YuMing\n");
        }
    // pve模式
    }else if(mode==2){
        char zong='A'+zongMAX;
        printf("AI Score = %d, factor = %1.3f\n",alpha[0],factor);
        if(n==0){
            ;
        }else if(n==1){
            printf("AI's position: (%c,%d)\n",'H',8);
        }else{
            printf("AI's position: (%c,%d)\n",zong,SIZE-hengMAX);
        }
        printf("Player's turn\n");
        printf("press 'quit' to quit\n");
        printf("Work of 戴玉鸣\n");
    }
    // 获得玩家选择落子点
    getPosition(&num1,&num2);
    if(quit==1){
        return;
    }
    // 玩家落子
    int i,j;
    for(i=0;i<SIZE;++i)
        for(j=0;j<SIZE;++j){
            if(inBoard[i][j]==3)
                inBoard[i][j]=1;
            else if(inBoard[i][j]==4)
                inBoard[i][j]=2;    
        }
    switch(n%2){
        case 0:
            inBoard[SIZE-num1][num2]=3;
            break;
        case 1:
            inBoard[SIZE-num1][num2]=4;
            break;
    }
    // 回合数增加
    ++n;
}

// 棋局是否一方胜利或者出现禁手判断函数
int judge(int *pbanned){
    int dir,i,j;
    int huoWu=0,changLian=0;
    int *pchangLian=&changLian;
    int *phuoWu=&huoWu;
    for(i=0;i<SIZE;i++){
        for(j=0;j<SIZE;j++){
            if(inBoard[i][j]==3){
                inBoard[i][j]=1;
                if(isBanned(i,j)==1){
                    *pbanned=1;
                    inBoard[i][j]=3;
                    return 0;
                }
                for(dir=1;dir<=4;dir++){
                    isFiveOrLong(i,j,1,dir,phuoWu,pchangLian);
                }
                inBoard[i][j]=3;
                if(huoWu>0&&changLian==0){
                    return 0;
                }
            }else if(inBoard[i][j]==4){
                inBoard[i][j]=2;
                for(dir=1;dir<=4;dir++){
                    isFiveOrLong(i,j,2,dir,phuoWu,pchangLian);
                }
                inBoard[i][j]=4;
                if(huoWu+changLian>0){
                    return 0;
                }
            }
        }
    }
    return 1;
}

// 玩家落子点获得函数
void getPosition(int *pnum1,int *pnum2){
    int len;
    char num[MAXLEN];
    int c,i,j;
    int n1,n2;
    node:
    i=j=0;
    if((len=mygetline(line,MAXLEN))<=0){
        quit=1;
        return;
    }
    if(strcmp(line,"quit\n")==0){
        quit=1;
        return;
    }
    while(!isalpha(c=line[i++]) && i<len)
        ;
    if(c>='a' && c<='o')
        n2=c-'a';
    else if(c>='A' && c<='O')
        n2=c-'A';
    else{
        printf("Error: please reinput your chess coord\n");
        goto node;
    }
    while(isalpha(line[i++]) && i<len){
        printf("Error: please reinput your chess coord\n");
        goto node;
    }
    i=0;
    while(!isdigit((c=line[j++])) && j<len)
        ;
    if(isdigit(c)){
        num[i]=c;
        while(isdigit(num[++i]=line[j++]) && j<len)
            ;
        num[i]='\0';
        n1=atoi(num);
        if(n1<1 || n1>15){
            printf("Error: please input right number!\n");
            goto node;
        }
    }else{
        printf("Error: please input number!\n");
        goto node;
    }
    if(inBoard[SIZE-n1][n2]!=0){
        printf("Error: there is already a chess!\n");
        goto node;
    }
    *pnum1=n1;
    *pnum2=n2;
    return;
}

//初始化一个空棋盘格局 
void init()
{
	//通过双重循环，将inboard清0
    int i,j;
    for(i=0;i<SIZE;++i){
        for(j=0;j<SIZE;++j){
            inBoard[i][j]=0;
        }
    }
}

//将inboard中记录的棋子位置，转化到disboard中
void innerBoard()
{
	//第一步：将empboard中记录的空棋盘，复制到disboard中
    int i,j;
    for(i=0;i<SIZE;++i)
        for(j=0;j<SIZE*CHARSIZE+1;++j)
            disBoard[i][j]=empBoard[i][j];
	//第二步：扫描inboard，当遇到非0的元素，将●或者◎复制到disboard的相应位置上
	//注意：disboard所记录的字符是中文字符，每个字符占2个字节。●和◎也是中文字符，每个也占2个字节。
    for(i=0;i<SIZE;++i)
        for(j=0;j<SIZE;++j)
            if(inBoard[i][j]==1){
                disBoard[i][j*CHARSIZE]=play1Pic[0];
                disBoard[i][j*CHARSIZE+1]=play1Pic[1];
            }else if(inBoard[i][j]==2){
                disBoard[i][j*CHARSIZE]=play2Pic[0];
                disBoard[i][j*CHARSIZE+1]=play2Pic[1];
            }else if(inBoard[i][j]==3){
                disBoard[i][j*CHARSIZE]=play1CurPic[0];
                disBoard[i][j*CHARSIZE+1]=play1CurPic[1];
            }else if(inBoard[i][j]==4){
                disBoard[i][j*CHARSIZE]=play2CurPic[0];
                disBoard[i][j*CHARSIZE+1]=play2CurPic[1];
            }else{
                printf("Error: unknown chess\n");
            }
}

//显示棋盘格局 
void displayBoard()
{
	int i;
	//第一步：清屏
	system("clear");   //清屏  
	//第二步：将disboard输出到屏幕上
    for(i=0;i<SIZE;++i)
        printf("%3d %s\n",SIZE-i,disBoard[i]);
	//第三步：输出最下面的一行字母A B .... 
    printf("    ");
    for(i=0;i<SIZE;++i)
        printf("%2c",'A'+i);
    printf("\n");
} 

// mygetline函数
int mygetline(char s[],int lim)
{
    int c,i;
    i=0;
    while((c=getchar())!=EOF&&c!='\n'){
        s[i++]=c;
    }
    if(c=='\n')
        s[i++]='\n';
    s[i]='\0';
    return i;
}