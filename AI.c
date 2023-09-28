#include "gobang.h"

void getSingleScore(int y,int x);
int ifExtend(int y,int x);
void partScoreUpdate(int y,int x);
int ifSearchNextFloor(int n,int m);
void deepSearch(int t);
void quicksort(int s[][3],int left,int right);
void swap(int s[][3],int i,int j);
int max(int a[],int b[],int k);
int min(int a[],int b[],int k);
void roundOne(int y,int x,int *py,int *px);
int dynamicBalance();

// 人机对战AI主程序
void AI(){
    int i,j;
    // 必要参数初始化，获得进入AI函数的时间
    time_t t1;
    t1=time(0); 
    alpha[0]=-2000000;
    beta[0]=2000000;
    matchTimes=0;
    totalTimes=0;
    // 寻找对手刚刚落子点位并进行处理
    if(sign==1){
        for(i=0;i<15;i++){
            for(j=0;j<15;j++){
                // 找到对手刚刚落子点后，设定下一步AI的落子考虑范围，更新局部棋子分数，并且获得新的棋局特征值
                if(inBoard[i][j]==4){
                    inBoard[i][j]=2;
                    partScoreUpdate(i,j);
                    xMax[1]=i+RANGE;
                    xMin[1]=i-RANGE;
                    yMax[1]=j+RANGE;
                    yMin[1]=j-RANGE;
                    chessKey^=zobristWriteMap[i][j];
                }else if(inBoard[i][j]==0){
                    hengMAX=i;
                    zongMAX=j;
                }
            }
        }
    }else if(sign==-1){
        for(i=0;i<15;i++){
            for(j=0;j<15;j++){
                if(inBoard[i][j]==3){
                    inBoard[i][j]=1;
                    partScoreUpdate(i,j);
                    xMax[1]=i+RANGE;
                    xMin[1]=i-RANGE;
                    yMax[1]=j+RANGE;
                    yMin[1]=j-RANGE;
                    chessKey^=zobristBlackMap[i][j];
                    // 如果玩家是先手，且只落下一个子，则有专门的第一轮应对函数落子
                    if(n==1){
                        roundOne(i,j,&hengMAX,&zongMAX);
                        inBoard[hengMAX][zongMAX]=2;
                        partScoreUpdate(hengMAX,zongMAX);
                        inBoard[hengMAX][zongMAX]=4;
                        chessKey^=zobristWriteMap[hengMAX][zongMAX];
                        alpha[0]=wholeScore=-1000;
                        return;
                    }
                }else if(inBoard[i][j]==0){
                    hengMAX=i;
                    zongMAX=j;
                }
            }
        }
    }
    if(n>3){
        zobrist();
    }
    // 设定平衡因子
    factor=(float)2*wholeScore/(float)DENOMINATOR;
    if(factor>0.2){
        factor=0.2;
    }else if(factor<-0.3){
        factor=-0.3;
    }
    if(sign==1){
        corr=1;
    }else if(sign==-1){
        corr=2;
    }
    // 对博弈树搜索所需参数进行初始化
    flooor=0;
    extend=0;
    noreturn=0;
    // 进入博弈树搜索
    deepSearch(t1);
    // 进行落子
    if(sign==1){
        inBoard[hengMAX][zongMAX]=1;
        partScoreUpdate(hengMAX,zongMAX);
        inBoard[hengMAX][zongMAX]=3;
        chessKey^=zobristBlackMap[hengMAX][zongMAX];
    }else if(sign==-1){
        inBoard[hengMAX][zongMAX]=2;
        partScoreUpdate(hengMAX,zongMAX);
        inBoard[hengMAX][zongMAX]=4;
        chessKey^=zobristWriteMap[hengMAX][zongMAX];
    }
    // 对下一次落子设定落子考虑范围
    xMax[0]=hengMAX+RANGE;
    xMin[0]=hengMAX-RANGE;
    yMax[0]=zongMAX+RANGE;
    yMin[0]=zongMAX-RANGE;
}

// 单个棋子打分函数
void getSingleScore(int y,int x){
    int d;
    int huoWu=0,huoSi=0,chongSi=0,huoSan=0,mianSan=0,huoEr=0,mianEr=0,changLian=0;
    int *phuoSi=&huoSi;
    int *pchongSi=&chongSi;
    int *phuoSan=&huoSan;
    int *phuoEr=&huoEr;
    int *pchangLian=&changLian;
    int *phuoWu=&huoWu;
    int *pmianSan=&mianSan;
    int *pmianEr=&mianEr;
    chessScore[y][x]=0;
    if(inBoard[y][x]!=0){
        if(inBoard[y][x]==1){
            // 评估落子点棋型
            for(d=1;d<=4;d++){
                isFiveOrLong(y,x,1,d,phuoWu,pchangLian);
                isHuoSiOrChongSi(y,x,1,d,phuoSi,pchongSi);
                isHuoSanOrMianSan(y,x,1,d,phuoSan,pmianSan);
                isHuoErOrMianEr(y,x,1,d,phuoEr,pmianEr);
            }
            // 对不同的棋型给予不同的分数，存入chessScore数组中，且黑子是正分，白子是负分
            if(huoSan+chongSi>=2&&huoWu==0&&huoSi==0){
                chessScore[y][x]=SHA;
            }else if(huoSan>=1&&(huoEr>=2||mianSan>=1)&&huoWu==0&&huoSi==0){
                chessScore[y][x]=KUAISHA;
            }else if(mianSan>=2&&huoEr>=1&&huoWu==0&&huoSi==0){
                chessScore[y][x]=KUAIXIAOSHA;
            }else if(((mianSan>=1&&(mianEr+huoEr)>=2)||mianSan>=2)&&huoWu==0&&huoSi==0){
                chessScore[y][x]=JIAOCHA;
            }else if(huoEr>=2&&mianEr>=1&&huoWu==0&&huoSi==0){
                chessScore[y][x]=YITUAN;
            }else if(huoEr>=2&&huoWu==0&&huoSi==0){
                chessScore[y][x]=YIXIAOTUAN;
            }else{
                chessScore[y][x]=huoWu*HUOWU+huoSi*HUOSI+chongSi*CHONGSI+huoSan*HUOSAN+mianSan*MIANSAN+huoEr*HUOER+mianEr*MIANER;
            }
        }else if(inBoard[y][x]==2){
            for(d=1;d<=4;d++){
                isFiveOrLong(y,x,2,d,phuoWu,pchangLian);
                isHuoSiOrChongSi(y,x,2,d,phuoSi,pchongSi);
                isHuoSanOrMianSan(y,x,2,d,phuoSan,pmianSan);
                isHuoErOrMianEr(y,x,2,d,phuoEr,pmianEr);
            }
            if(huoSan+chongSi>=2&&huoWu==0&&huoSi==0&&changLian==0){
                chessScore[y][x]=-SHA;
            }else if(huoSan>=1&&(huoEr>=2||mianSan>=1)&&huoWu==0&&huoSi==0&&changLian==0){
                chessScore[y][x]=-KUAISHA;
            }else if(mianSan>=2&&huoEr>=1&&huoWu==0&&huoSi==0){
                chessScore[y][x]=-KUAIXIAOSHA;
            }else if(((mianSan>=1&&(mianEr+huoEr)>=2)||mianSan>=2)&&huoWu==0&&huoSi==0){
                chessScore[y][x]=-JIAOCHA;
            }else if(huoEr>=2&&mianEr>=1&&huoWu==0&&huoSi==0){
                chessScore[y][x]=-YITUAN;
            }else if(huoEr>=2&&huoWu==0&&huoSi==0){
                chessScore[y][x]=-YIXIAOTUAN;
            }else{
                chessScore[y][x]=-1*(huoWu*HUOWU+changLian*HUOWU+huoSi*HUOSI+chongSi*CHONGSI+huoSan*HUOSAN+mianSan*MIANSAN+huoEr*HUOER+mianEr*MIANER);
            }
        }
    }
}

// 禁手判断函数
int isBanned(int y,int x){

    // return 0;

    int huoWu=0,huoSi=0,chongSi=0,huoSan=0,mianSan=0,changLian=0,d;
    int *phuoSi=&huoSi;
    int *pchongSi=&chongSi;
    int *phuoSan=&huoSan;
    int *pchangLian=&changLian;
    int *phuoWu=&huoWu;
    int *pmianSan=&mianSan;
    if(inBoard[y][x]!=1){
        return 0;
    }else{
        // 获得落子点棋型
        for(d=1;d<=4;d++){
            isFiveOrLong(y,x,1,d,phuoWu,pchangLian);
            isHuoSiOrChongSi(y,x,1,d,phuoSi,pchongSi);
            isHuoSanOrMianSan(y,x,1,d,phuoSan,pmianSan);
        }
        // 如果符合禁手条件则返回1
        if((huoSi+chongSi>1 || huoSan>1 || changLian>0)&&huoWu==0){
            return 1;
        }
        else{
            return 0;
        }
    }
}

// 活五和长连判断函数
void isFiveOrLong(int y,int x,int color,int d,int *phuoWu,int *pchangLian){
    int h,num=0;
    // d为方向，分为竖直，水平，左斜，右斜，对应数字1，2，3，4
    switch(d){
        case 1:
            // 获得同一直线上相连的同色棋子个数
            for(h=0;x+h<SIZE && inBoard[y][x+h]==color;h++)
                num++;
            for(h=1;x-h>=0 && inBoard[y][x-h]==color;h++)
                num++;
            if(num==5)
                (*phuoWu)++;
            else if(num>5)
                (*pchangLian)++;
            break;
        case 2:
            for(h=0;y+h<SIZE && inBoard[y+h][x]==color;h++)
                num++;
            for(h=1;y-h>=0 && inBoard[y-h][x]==color;h++)
                num++;
            if(num==5)
                (*phuoWu)++;
            else if(num>5)
                (*pchangLian)++;
            break;
        case 3:
            for(h=0;y+h<SIZE && x+h<SIZE && inBoard[y+h][x+h]==color;h++)
                num++;
            for(h=1;y-h>=0 && x-h>=0 && inBoard[y-h][x-h]==color;h++)
                num++;
            if(num==5)
                (*phuoWu)++;
            else if(num>5)
                (*pchangLian)++;
            break;
        case 4:
            for(h=0;y+h<SIZE && x-h>=0 && inBoard[y+h][x-h]==color;h++)
                num++;
            for(h=1;y-h>=0 && x+h<SIZE && inBoard[y-h][x+h]==color;h++)
                num++;
            if(num==5)
                (*phuoWu)++;
            else if(num>5)
                (*pchangLian)++;
            break;
    } 
}

// 活四和冲四判断函数
void isHuoSiOrChongSi(int y,int x,int color,int d,int *phuoSi,int *pchongSi){
    int i,j,h,num=0,nhuowu=0,num2=0,nlong=0;
    switch(d){
        case 1:
            // 获得同一直线上相连同色棋子个数
            for(i=0;x+i<SIZE && inBoard[y][x+i]==color;i++)
                num++;
            for(j=1;x-j>=0 && inBoard[y][x-j]==color;j++)
                num++;
            // 若大于4不可能为冲四或活四，退出
            if(num>4)
                return;
            // 否则判断是否有只隔一个空位的同色棋子以及个数
            if(inBoard[y][x+i]==0&&x+i<SIZE){
                for(num2=0,h=1;x+i+h<SIZE && inBoard[y][x+i+h]==color;h++)
                    num2++;
                if(num+num2==4)
                    nhuowu++;
                else if(num+num2>4){
                    nlong++;
                }
            }
            if(x-j>=0 && inBoard[y][x-j]==0){
                for(num2=0,h=1;x-j-h>=0 && inBoard[y][x-h-j]==color;h++)
                    num2++;
                if(num+num2==4)
                    nhuowu++;
                else if(num+num2>4){
                    nlong++;
                }
            }
            break; 
        case 2:
            for(i=0;y+i<SIZE && inBoard[y+i][x]==color;i++)
                num++;
            for(j=1;y-j>=0 && inBoard[y-j][x]==color;j++)
                num++;
            if(num>4)
                return;
            if(inBoard[y+i][x]==0&&y+i<SIZE){
                for(num2=0,h=1;y+i+h<SIZE && inBoard[y+i+h][x]==color;h++)
                    num2++;
                if(num+num2==4)
                    nhuowu++;
                else if(num+num2>4){
                    nlong++;
                }
            }
            if(y-j>=0 && inBoard[y-j][x]==0){
                for(num2=0,h=1;y-j-h>=0 && inBoard[y-h-j][x]==color;h++)
                    num2++;
                if(num+num2==4)
                    nhuowu++;
                else if(num+num2>4){
                    nlong++;
                }
            }
            break;
        case 3:
            for(i=0;x+i<SIZE && y+i<SIZE && inBoard[y+i][x+i]==color;i++)
                num++;
            for(j=1;x-j>=0 && y-j>=0 && inBoard[y-j][x-j]==color;j++)
                num++;
            if(num>4)
                return;
            if(inBoard[y+i][x+i]==0&&x+i<SIZE && y+i<SIZE){
                for(num2=0,h=1;y+i+h<SIZE && x+i+h<SIZE && inBoard[y+i+h][x+i+h]==color;h++)
                    num2++;
                if(num+num2==4)
                    nhuowu++;
                else if(num+num2>4){
                    nlong++;
                }
            }
            if(x-j>=0 && y-j>=0 && inBoard[y-j][x-j]==0){
                for(num2=0,h=1;y-j-h>=0 && x-j-h>=0 && inBoard[y-j-h][x-j-h]==color;h++)
                    num2++;
                if(num+num2==4)
                    nhuowu++;
                else if(num+num2>4){
                    nlong++;
                }
            }
            break;
        case 4:
            for(i=0;x-i>=0 && y+i<SIZE && inBoard[y+i][x-i]==color;i++)
                num++;
            for(j=1;x+j<SIZE && y-j>=0 && inBoard[y-j][x+j]==color;j++)
                num++;
            if(num>4)
                return;
            if(inBoard[y+i][x-i]==0&&x-i>=0 && y+i<SIZE){
                for(num2=0,h=1;y+i+h<SIZE && x-i-h>=0 && inBoard[y+i+h][x-i-h]==color;h++)
                    num2++;
                if(num+num2==4)
                    nhuowu++;
                else if(num+num2>4){
                    nlong++;
                }
            }
            if(x+j<SIZE && y-j>=0 && inBoard[y-j][x+j]==0){
                for(num2=0,h=1;y-j-h>=0 && x+j+h<SIZE && inBoard[y-j-h][x+j+h]==color;h++)
                    num2++;
                if(num+num2==4)
                    nhuowu++;
                else if(num+num2>4){
                    nlong++;
                }
            }
            break;           
    }
    // 对不同颜色，不同情况，给出不同结果
    if(color==1){
        if(num==4 && nhuowu==2)
            (*phuoSi)++;
        else if(num<4 && nhuowu==2)
            (*pchongSi)+=2;
        else if(nhuowu==1)
            (*pchongSi)++;
    }else if(color==2){
        if(num==4 && (nhuowu+nlong)==2)
            (*phuoSi)++;
        else if(num<4 && (nhuowu+nlong)==2)
            (*pchongSi)+=2;
        else if((nhuowu+nlong)==1)
            (*pchongSi)++;
    }
}

// 活三和眠三判断函数
void isHuoSanOrMianSan(int y,int x,int color,int d,int *phuoSan,int *pmianSan){
    int i,j,h,num=0,nhuosi=0,nchongsi=0,num2=0;
    int *pnhuosi=&nhuosi;
    int *pnchongsi=&nchongsi;
    switch(d){
        case 1:
            for(i=0;x+i<SIZE && inBoard[y][x+i]==color;i++)
                num++;
            for(j=1;x-j>=0 && inBoard[y][x-j]==color;j++)
                num++;
            if(num>3)
                return;
            if(inBoard[y][x+i]==0&&x+i<SIZE){
                for(num2=0,h=1;x+i+h<SIZE && inBoard[y][x+i+h]==color;h++)
                    num2++;
                if(num+num2==3){
                    // 活三判断函数其余部分同活四判断函数
                    // 只有此处对空位先判断是否为禁手，判断为不是后再假设落子判断是否是活四
                    inBoard[y][x+i]=color;
                    if(isBanned(y,x+i)==0)
                        isHuoSiOrChongSi(y,x,color,d,pnhuosi,pnchongsi);
                    inBoard[y][x+i]=0;
                }
            }
            if(x-j>=0 && inBoard[y][x-j]==0){
                for(num2=0,h=1;x-j-h>=0 && inBoard[y][x-h-j]==color;h++)
                    num2++;
                if(num+num2==3){
                    inBoard[y][x-j]=color;
                    if(isBanned(y,x-j)==0)
                        isHuoSiOrChongSi(y,x,color,d,pnhuosi,pnchongsi);
                    inBoard[y][x-j]=0;
                }
            }
            break; 
        case 2:
            for(i=0;y+i<SIZE && inBoard[y+i][x]==color;i++)
                num++;
            for(j=1;y-j>=0 && inBoard[y-j][x]==color;j++)
                num++;
            if(num>3)
                return;
            if(inBoard[y+i][x]==0&&y+i<SIZE){
                for(num2=0,h=1;y+i+h<SIZE && inBoard[y+i+h][x]==color;h++)
                    num2++;
                if(num+num2==3){
                    inBoard[y+i][x]=color;
                    if(isBanned(y+i,x)==0)
                        isHuoSiOrChongSi(y,x,color,d,pnhuosi,pnchongsi);
                    inBoard[y+i][x]=0;
                }
            }
            if(y-j>=0 && inBoard[y-j][x]==0){
                for(num2=0,h=1;y-j-h>=0 && inBoard[y-h-j][x]==color;h++)
                    num2++;
                if(num+num2==3){
                     inBoard[y-j][x]=color;
                    if(isBanned(y-j,x)==0)
                        isHuoSiOrChongSi(y,x,color,d,pnhuosi,pnchongsi);
                    inBoard[y-j][x]=0;
                }
            }
            break;
        case 3:
            for(i=0;x+i<SIZE && y+i<SIZE && inBoard[y+i][x+i]==color;i++)
                num++;
            for(j=1;x-j>=0 && y-j>=0 && inBoard[y-j][x-j]==color;j++)
                num++;
            if(num>3)
                return;
            if(inBoard[y+i][x+i]==0&&x+i<SIZE && y+i<SIZE){
                for(num2=0,h=1;y+i+h<SIZE && x+i+h<SIZE && inBoard[y+i+h][x+i+h]==color;h++)
                    num2++;
                if(num+num2==3){
                    inBoard[y+i][x+i]=color;
                    if(isBanned(y+i,x+i)==0)
                        isHuoSiOrChongSi(y,x,color,d,pnhuosi,pnchongsi);
                    inBoard[y+i][x+i]=0;
                }
            }
            if(x-j>=0 && y-j>=0 && inBoard[y-j][x-j]==0){
                for(num2=0,h=1;y-j-h>=0 && x-j-h>=0 && inBoard[y-j-h][x-j-h]==color;h++)
                    num2++;
                if(num+num2==3){
                    inBoard[y-j][x-j]=color;
                    if(isBanned(y-j,x-j)==0)
                        isHuoSiOrChongSi(y,x,color,d,pnhuosi,pnchongsi);
                    inBoard[y-j][x-j]=0;
                }
            }
            break;
        case 4:
            for(i=0;x-i>=0 && y+i<SIZE && inBoard[y+i][x-i]==color;i++)
                num++;
            for(j=1;x+j<SIZE && y-j>=0 && inBoard[y-j][x+j]==color;j++)
                num++;
            if(num>3)
                return;
            if(inBoard[y+i][x-i]==0&&x-i>=0 && y+i<SIZE){
                for(num2=0,h=1;y+i+h<SIZE && x-i-h>=0 && inBoard[y+i+h][x-i-h]==color;h++)
                    num2++;
                if(num+num2==3){
                    inBoard[y+i][x-i]=color;
                    if(isBanned(y+i,x-i)==0)
                        isHuoSiOrChongSi(y,x,color,d,pnhuosi,pnchongsi);
                    inBoard[y+i][x-i]=0;
                }
            }
            if(x+j<SIZE && y-j>=0 && inBoard[y-j][x+j]==0){
                for(num2=0,h=1;y-j-h>=0 && x+j+h<SIZE && inBoard[y-j-h][x+j+h]==color;h++)
                    num2++;
                if(num+num2==3){
                    inBoard[y-j][x+j]=color;
                    if(isBanned(y-j,x+j)==0)
                        isHuoSiOrChongSi(y,x,color,d,pnhuosi,pnchongsi);
                    inBoard[y-j][x+j]=0;
                }
            }
            break;      
    }
    if(nhuosi>0){
        (*phuoSan)++;
    }
    else if(nchongsi>0){
        (*pmianSan)++;
    }
}

// 活二和眠二判断函数
void isHuoErOrMianEr(int y,int x,int color,int d,int *phuoEr,int *pmianEr){
    // 活二判断函数机制同活三判断函数
    int i,j,h,num=0,nhuosan=0,nmiansan,num2=0;
    int *pnhuosan=&nhuosan;
    int *pnmiansan=&nmiansan;
    switch(d){
        case 1:
            for(i=0;x+i<SIZE && inBoard[y][x+i]==color;i++)
                num++;
            for(j=1;x-j>=0 && inBoard[y][x-j]==color;j++)
                num++;
            if(num>2)
                return;
            if(inBoard[y][x+i]==0&&x+i<SIZE){
                for(num2=0,h=1;x+i+h<SIZE && inBoard[y][x+i+h]==color;h++)
                    num2++;
                if(num+num2==2){
                    inBoard[y][x+i]=color;
                    if(isBanned(y,x+i)==0)
                        isHuoSanOrMianSan(y,x,color,d,pnhuosan,pnmiansan);
                    inBoard[y][x+i]=0;
                }
            }
            if(x-j>=0 && inBoard[y][x-j]==0){
                for(num2=0,h=1;x-j-h>=0 && inBoard[y][x-h-j]==color;h++)
                    num2++;
                if(num+num2==2){
                    inBoard[y][x-j]=color;
                    if(isBanned(y,x+i)==0)
                        isHuoSanOrMianSan(y,x,color,d,pnhuosan,pnmiansan);
                    inBoard[y][x-j]=0;
                }
            }
            break;
        case 2:
            for(i=0;y+i<SIZE && inBoard[y+i][x]==color;i++)
                num++;
            for(j=1;y-j>=0 && inBoard[y-j][x]==color;j++)
                num++;
            if(num>2)
                return;
            if(inBoard[y+i][x]==0&&y+i<SIZE){
                for(num2=0,h=1;y+i+h<SIZE && inBoard[y+i+h][x]==color;h++)
                    num2++;
                if(num+num2==2){
                    inBoard[y+i][x]=color;
                    if(isBanned(y,x+i)==0)
                        isHuoSanOrMianSan(y,x,color,d,pnhuosan,pnmiansan);
                    inBoard[y+i][x]=0;
                }
            }
            if(y-j>=0 && inBoard[y-j][x]==0){
                for(num2=0,h=1;y-j-h>=0 && inBoard[y-h-j][x]==color;h++)
                    num2++;
                if(num+num2==2){
                    inBoard[y-j][x]=color;
                    if(isBanned(y,x+i)==0)
                        isHuoSanOrMianSan(y,x,color,d,pnhuosan,pnmiansan);
                    inBoard[y-j][x]=0;
                }
            }
            break;
        case 3:
            for(i=0;x+i<SIZE && y+i<SIZE && inBoard[y+i][x+i]==color;i++)
                num++;
            for(j=1;x-j>=0 && y-j>=0 && inBoard[y-j][x-j]==color;j++)
                num++;
            if(num>2)
                return;
            if(inBoard[y+i][x+i]==0&&x+i<SIZE && y+i<SIZE){
                for(num2=0,h=1;y+i+h<SIZE && x+i+h<SIZE && inBoard[y+i+h][x+i+h]==color;h++)
                    num2++;
                if(num+num2==2){
                    inBoard[y+i][x+i]=color;
                    if(isBanned(y,x+i)==0)
                        isHuoSanOrMianSan(y,x,color,d,pnhuosan,pnmiansan);
                    inBoard[y+i][x+i]=0;
                }
            }
            if(x-j>=0 && y-j>=0 && inBoard[y-j][x-j]==0){
                for(num2=0,h=1;y-j-h>=0 && x-j-h>=0 && inBoard[y-j-h][x-j-h]==color;h++)
                    num2++;
                if(num+num2==2){
                    inBoard[y-j][x-j]=color;
                    if(isBanned(y,x+i)==0)
                        isHuoSanOrMianSan(y,x,color,d,pnhuosan,pnmiansan);
                    inBoard[y-j][x-j]=0;
                }
            }
            break;
        case 4:
            for(i=0;x-i>=0 && y+i<SIZE && inBoard[y+i][x-i]==color;i++)
                num++;
            for(j=1;x+j<SIZE && y-j>=0 && inBoard[y-j][x+j]==color;j++)
                num++;
            if(num>2)
                return;
            if(inBoard[y+i][x-i]==0&&x-i>=0 && y+i<SIZE){
                for(num2=0,h=1;y+i+h<SIZE && x-i-h>=0 && inBoard[y+i+h][x-i-h]==color;h++)
                    num2++;
                if(num+num2==2){
                    inBoard[y+i][x-i]=color;
                    if(isBanned(y,x+i)==0)
                        isHuoSanOrMianSan(y,x,color,d,pnhuosan,pnmiansan);
                    inBoard[y+i][x-i]=0;
                }
            }
            if(x+j<SIZE && y-j>=0 && inBoard[y-j][x+j]==0){
                for(num2=0,h=1;y-j-h>=0 && x+j+h<SIZE && inBoard[y-j-h][x+j+h]==color;h++)
                    num2++;
                if(num+num2==2){
                    inBoard[y-j][x+j]=color;
                    if(isBanned(y,x+i)==0)
                        isHuoSanOrMianSan(y,x,color,d,pnhuosan,pnmiansan);
                    inBoard[y-j][x+j]=0;
                }
            }
            break;      
    }
    if(nhuosan>0)
        (*phuoEr)++;
    else if(nmiansan>0)
        (*pmianEr)++;
}

// 博弈树搜索层数是否增加判断函数
int ifExtend(int y,int x){
    int huoSi=0,chongSi=0,huoSan=0,mianSan=0,d;
    int *phuoSi=&huoSi;
    int *pchongSi=&chongSi;
    int *phuoSan=&huoSan;
    int *pmianSan=&mianSan;
    if(inBoard[y][x]!=0){
        if(inBoard[y][x]==1){
            for(d=1;d<=4;d++){
                isHuoSiOrChongSi(y,x,1,d,phuoSi,pchongSi);
            }
        }else if(inBoard[y][x]==2){
            for(d=1;d<=4;d++){
                isHuoSiOrChongSi(y,x,2,d,phuoSi,pchongSi);
            }
        }
        // 如果该落子点形成了冲四，则增加最大搜索深度
        if(chongSi>=1){
            return 1;
        }else{
            return 0;
        }
    }else{
        return 0;
    }
}

// 局部棋子分数更新函数
void partScoreUpdate(int y,int x){
    int i,j;
    // 重新获得落子点周围四条直线上距离最近的6个棋子的分数
    for(i=-6;i<=6;i++){
        if(y+i<15&&y+i>=0){
            getSingleScore(y+i,x);
        }
    }
    for(j=-6;j<=6;j++){
        if(x+j<15&&x+j>=0){
            getSingleScore(y,x+j);
        }
    }
    for(i=j=-6;i<=6&&j<=6;i++,j++){
        if(y+i<15&&y+i>=0&&x+j<15&&x+j>=0){
            getSingleScore(y+i,x+j);
        }
    }
    for(i=j=-6;i<=6&&j<=6;i++,j++){
        if(y+i<15&&y+i>=0&&x-j<15&&x-j>=0){
            getSingleScore(y+i,x-j);
        }
    }
}

// 全局分更新函数
void wholeScoreUpdate(){    
    int i,j;
    wholeScore=0;
    // 将所有位置上的棋子分数相加得到全局分数
    for(i=0;i<SIZE;i++){
        for(j=0;j<SIZE;j++){
            wholeScore+=chessScore[i][j];
        }
    }
    // 白子的全局分要取负数
    wholeScore=sign*wholeScore;
}

// 动态平衡分数评估函数
int dynamicBalance(){
    int i,j;
    balanceScore=0;
    for(i=0;i<SIZE;i++){
        for(j=0;j<SIZE;j++){
            if(inBoard[i][j]==1){
                balanceScore+=chessScore[i][j];
            }else{
                balanceScore+=(-chessScore[i][j]);
            }
        }
    }
    // 对最终分数进行平衡
    balanceScore=factor*balanceScore;
}

// 判断该点是否值得继续搜索函数
int ifSearchNextFloor(int n,int m){
    int i,j;
    int temp=0;
    for(j=-SCOPE;j<=SCOPE;j++){
        if(inBoard[n][m+j]!=0&&m+j>=0&&m+j<15){
            temp++;
        }
    }
    for(i=-SCOPE;i<=SCOPE;i++){
        if(inBoard[n+i][m]!=0&&n+i>=0&&n+i<15){
            temp++;
        }
    }
    for(i=j=-SCOPE;i<=SCOPE;i++,j++){
        if(inBoard[n+i][m+j]!=0&&n+i>=0&&n+i<15&&m+j>=0&&m+j<15){
            temp++;
        }
    }
    for(i=j=-SCOPE;i<=SCOPE;i++,j++){
        if(inBoard[n-i][m+j]!=0&&n-i>=0&&n-i<15&&m+j>=0&&m+j<15){
            temp++;
        }
    }
    // 如果该落子点周围有已经落下的棋子，代表该落子点可以进行再进一步的搜索
    if(temp==0){
        return 0;
    }else{
        return 1;
    }
}

// 博弈树搜索函数
void deepSearch(int t){
    // 如果搜索层数没有超出限制
    if(flooor<DEEP+extend){
        int i,j,k=0,back=0;
        int heuristicSearch[224][3];
        for(i=0;i<15;i++){
            for(j=0;j<15;j++){
                // 如果搜索到的点满足条件，将其纳入启发式搜索缓存数组中
                if(inBoard[i][j]==0&&ifSearchNextFloor(i,j)==1){
                    inBoard[i][j]=2;   
                    getSingleScore(i,j);
                    heuristicSearch[k][0]=-1*chessScore[i][j];  
                    heuristicSearch[k][1]=i;
                    heuristicSearch[k][2]=j;
                    inBoard[i][j]=1;   
                    getSingleScore(i,j);
                    if(isBanned(i,j)==0&&chessScore[i][j]>heuristicSearch[k][0]){  
                        heuristicSearch[k][0]=chessScore[i][j];
                    }
                    if(
                    (i>=xMin[flooor]&&i<=xMax[flooor]&&j>=yMin[flooor]&&j<=yMax[flooor])||
                    (i>=xMin[flooor+1]&&i<=xMax[flooor+1]&&j>=yMin[flooor+1]&&j<=yMax[flooor+1])||
                    heuristicSearch[k][0]>=HUOSAN
                    ){
                        k++;
                    }
                    inBoard[i][j]=0;
                    chessScore[i][j]=0;
                }
            }
        }
        // 对启发式搜索到的点进行排序
        quicksort(heuristicSearch,0,k-1);
        for(i=0;i<k&&i<MAXPOINT;i++){
            // 如果不满足剪枝条件
            if(alpha[flooor]<beta[flooor]){
                // 将alpha和beta的值向下一层传递
                alpha[flooor+1]=alpha[flooor];
                beta[flooor+1]=beta[flooor];
                // 在棋盘上落子
                inBoard[heuristicSearch[i][1]][heuristicSearch[i][2]]=sign*(flooor%2)+corr;
                // 局部分数更新
                partScoreUpdate(heuristicSearch[i][1],heuristicSearch[i][2]);
                // 异或棋子特征值得到棋局特征值
                if((sign*(flooor%2)+corr)==1){
                    chessKey^=zobristBlackMap[heuristicSearch[i][1]][heuristicSearch[i][2]];
                }else if((sign*(flooor%2)+corr)==2){
                    chessKey^=zobristWriteMap[heuristicSearch[i][1]][heuristicSearch[i][2]];
                }
                // 增加搜索层数
                flooor++;
                // 如果满足条件，增加最大搜索深度
                if(ifExtend(heuristicSearch[i][1],heuristicSearch[i][2])==1&&extend<EXTENDMAX){
                    extend+=2;
                    back=1;
                }
                if(isBanned(heuristicSearch[i][1],heuristicSearch[i][2])==0){
                    if(flooor!=DEEP+extend){
                        xMax[flooor+1]=heuristicSearch[i][1]+RANGE;
                        xMin[flooor+1]=heuristicSearch[i][1]-RANGE;
                        yMax[flooor+1]=heuristicSearch[i][2]+RANGE;
                        yMin[flooor+1]=heuristicSearch[i][2]-RANGE;
                    }
                    if(heuristicSearch[i][0]<HUOWU){
                        // 满足条件，进入递归，搜索下一层
                        deepSearch(t);
                    }else{
                        // 如果搜索到活五，评估棋局分数，不再进入下一层
                        if(chessScore[heuristicSearch[i][1]][heuristicSearch[i][2]]>=HUOWU||chessScore[heuristicSearch[i][1]][heuristicSearch[i][2]]<=-HUOWU){
                            zobrist();
                            dynamicBalance();
                            alpha[flooor]=beta[flooor]=wholeScore+balanceScore;
                        // 否则搜索下一层
                        }else{
                            deepSearch(t);
                        }
                    }
                    // 搜索结束，进行alpha-beta剪枝
                    if(flooor==DEEP+extend&&noreturn==0){
                        zobrist();
                        dynamicBalance();
                        alpha[flooor]=beta[flooor]=wholeScore+balanceScore;
                        beta[flooor-1]=min(alpha,beta,flooor);
                    }else if(flooor==1&&noreturn==0){
                        if(alpha[flooor-1]!=max(alpha,beta,flooor)){
                            hengMAX=heuristicSearch[i][1];
                            zongMAX=heuristicSearch[i][2];
                            alpha[flooor-1]=max(alpha,beta,flooor);
                        }
                    }else if(flooor%2==0&&flooor!=DEEP+extend&&noreturn==0){
                        beta[flooor-1]=min(alpha,beta,flooor);
                    }else if(flooor%2==1&&flooor!=1&&noreturn==0){
                        alpha[flooor-1]=max(alpha,beta,flooor);
                    }
                }
                // 撤回设想的落子，更新局部棋子分数，减回搜索层数
                inBoard[heuristicSearch[i][1]][heuristicSearch[i][2]]=0;
                partScoreUpdate(heuristicSearch[i][1],heuristicSearch[i][2]);
                flooor--;
                // 退回棋局特征值
                if((sign*(flooor%2)+corr)==1){
                    chessKey^=zobristBlackMap[heuristicSearch[i][1]][heuristicSearch[i][2]];
                }else if((sign*(flooor%2)+corr)==2){
                    chessKey^=zobristWriteMap[heuristicSearch[i][1]][heuristicSearch[i][2]];
                }
                // 退回增加的最大搜索深度
                if(back==1){
                    extend-=2;
                    back=0;
                }
                // 如果到达时间限制，退出搜索此层，且该博弈树分支不向上层返还alpha和beta值
                if((time(0)-t)>=TIMELIMIT){
                    noreturn=1;
                    break;
                }
            }else{
                break;
            }
        }
    }
}

// 启发式搜索缓存数组快排函数
void quicksort(int s[][3],int left,int right){
    int i,last;
    void swap(int s[][3],int i,int j);
    if(left>=right)
        return;
    swap(s,left,(left+right)/2);
    last=left;
    for(i=left+1;i<=right;i++){
        if(s[i][0]>s[left][0]){
            swap(s,++last,i);
        }
    }
    swap(s,left,last);
    quicksort(s,left,last-1);
    quicksort(s,last+1,right);
}

// 快排所需swap函数
void swap(int s[][3],int i,int j){
    int tempScore,tempX,tempY;
    tempScore=s[i][0];
    tempX=s[i][1];
    tempY=s[i][2];
    s[i][0]=s[j][0];
    s[i][1]=s[j][1];
    s[i][2]=s[j][2];
    s[j][0]=tempScore;
    s[j][1]=tempX;
    s[j][2]=tempY;
}

// 剪枝所需取最大值函数
int max(int a[],int b[],int k){
    if(a[k]>b[k]){
        if(a[k-1]>a[k]){
            return a[k-1];
        }else{
            return a[k];
        }
    }else{
        if(a[k-1]>b[k]){
            return a[k-1];
        }else{
            return b[k];
        }
    }
}

// 剪枝所需取最小值函数
int min(int a[],int b[],int k){
    if(a[k]>b[k]){
        if(b[k]>b[k-1]){
            return b[k-1];
        }else{
            return b[k];
        }
    }else{
        if(a[k]>b[k-1]){
            return b[k-1];
        }else{
            return a[k];
        }
    }
}

// 后手第一轮落子函数
void roundOne(int y,int x,int *py,int *px){
    static int temp=0;
    if(y==7&&x==7){
        switch(temp%4){
            case 0:
                *py=y+1;
                *px=x;
                temp++;
                break;
            case 1:
                *py=y;
                *px=x+1;
                temp++;
                break;
            case 2:
                *py=y;
                *px=x-1;
                temp++;
                break;
            case 3:
                *py=y-1;
                *px=x;
                temp++;
                break;
        }
    }else if(y<=x&&y<=SIZE-x){
        *py=y+1;
        *px=x;
    }else if(SIZE-y<=x&&SIZE-y<=SIZE-x){
        *py=y-1;
        *px=x;
    }else if(x<=y&&x<=SIZE-y){
        *py=y;
        *px=x+1;
    }else if(SIZE-x<=y&&SIZE-x<=SIZE-y){
        *py=y;
        *px=x-1;
    }
}