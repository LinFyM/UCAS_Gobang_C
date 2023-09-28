#include "gobang.h"

char line[MAXLEN];
//����ʹ�õ���GBK���룬ÿһ�������ַ�ռ��2���ֽڡ�

//������ģ�� 
char empBoard[SIZE][SIZE*CHARSIZE+1] = 
{
		"���өөөөөөөөөөөөө�",
		"�ĩ��������������",
		"�ĩ��������������",
		"�ĩ��������������",
		"�ĩ��������������",
		"�ĩ��������������",
		"�ĩ��������������",
		"�ĩ��������������",
		"�ĩ��������������",
		"�ĩ��������������",
		"�ĩ��������������",
		"�ĩ��������������",
		"�ĩ��������������",
		"�ĩ��������������",
		"���۩۩۩۩۩۩۩۩۩۩۩۩۩�"
};
//������洢������ʾ������ 
char disBoard[SIZE][SIZE*CHARSIZE+1];
 
char play1Pic[]="��";//������;
char play1CurPic[]="��"; 

char play2Pic[]="��";//������;
char play2CurPic[]="��";

//���������ڼ�¼��ǰ�����̵ĸ�� 
int inBoard[SIZE][SIZE];
// һЩ��Ҫ�ı���
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

// ������
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
    printf("Work of ������\n");
    // ������ѡ�������ģʽ
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

// pve������
void pve(){
    system("clear");
    // ��Ҫ�ı������ü���ʼ��
    int order,len,banned,i,j;
    int *pbanned=&banned;
    // ��AI��ʼ���ӿ��Ƿ�Χ���г�ʼ�����趨Ϊȫ������
    xMax[0]=14;
    xMin[0]=0;
    yMax[0]=14;
    yMin[0]=0;
    // �����ӷ������г�ʼ����ȫ������Ϊ��
    for(i=0;i<SIZE;i++){
        for(j=0;j<SIZE;j++){
            chessScore[i][j]=0;
        }
    }
    // ��zobristMap�������ã���ÿ�����ӵ�ĺڰ��ӵ�����ֵ��64λ��������
    zobristInit();
    zobristMapReset();
    banned=0;
    // �����Ļ��ʾ
    printf("Please choose the order:\n");
    printf("(1) player goes first: press 1\n");
    printf("(2) AI goes first: press 2\n");
    // ���ѡ���Ⱥ���
    fool:
    // �������Ⱥ���ѡ�������Ϊ���֣�signΪ-1���������ִ�ף���֮��Ȼ
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
    // AI�������ӣ�ֱ���������������룬�����趨ȫ�ַ�Ϊ1000��
    if(sign==1){
        alpha[0]=wholeScore=1000;
        inBoard[(SIZE-1)/2][(SIZE-1)/2]=3;
        chessKey^=zobristBlackMap[(SIZE-1)/2][(SIZE-1)/2];
        innerBoard();
        displayBoard();
        ++n;
    // �����趨ȫ�ַ�Ϊ-1000��
    }else{
        alpha[0]=wholeScore=-1000;
    }
    // ���û�г��ֽ��ֻ���û��һ��ʤ����������û��������ѭ������˫������
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
    // ���������ͬ����Ծֽ��
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

// pvp������
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

// ��ҽ�������
void player(){
    int num1,num2;
    // pvpģʽ
    if(mode==1){
        if(n%2==1){
            printf("Player2's turn\n");
            printf("press 'quit' to quit\nCreator : Dai YuMing\n");
        }else{ 
            printf("Player1's turn\n");
            printf("press 'quit' to quit\nCreator : Dai YuMing\n");
        }
    // pveģʽ
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
        printf("Work of ������\n");
    }
    // ������ѡ�����ӵ�
    getPosition(&num1,&num2);
    if(quit==1){
        return;
    }
    // �������
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
    // �غ�������
    ++n;
}

// ����Ƿ�һ��ʤ�����߳��ֽ����жϺ���
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

// ������ӵ��ú���
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

//��ʼ��һ�������̸�� 
void init()
{
	//ͨ��˫��ѭ������inboard��0
    int i,j;
    for(i=0;i<SIZE;++i){
        for(j=0;j<SIZE;++j){
            inBoard[i][j]=0;
        }
    }
}

//��inboard�м�¼������λ�ã�ת����disboard��
void innerBoard()
{
	//��һ������empboard�м�¼�Ŀ����̣����Ƶ�disboard��
    int i,j;
    for(i=0;i<SIZE;++i)
        for(j=0;j<SIZE*CHARSIZE+1;++j)
            disBoard[i][j]=empBoard[i][j];
	//�ڶ�����ɨ��inboard����������0��Ԫ�أ�������ߡ��Ƶ�disboard����Ӧλ����
	//ע�⣺disboard����¼���ַ��������ַ���ÿ���ַ�ռ2���ֽڡ���͡�Ҳ�������ַ���ÿ��Ҳռ2���ֽڡ�
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

//��ʾ���̸�� 
void displayBoard()
{
	int i;
	//��һ��������
	system("clear");   //����  
	//�ڶ�������disboard�������Ļ��
    for(i=0;i<SIZE;++i)
        printf("%3d %s\n",SIZE-i,disBoard[i]);
	//������������������һ����ĸA B .... 
    printf("    ");
    for(i=0;i<SIZE;++i)
        printf("%2c",'A'+i);
    printf("\n");
} 

// mygetline����
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