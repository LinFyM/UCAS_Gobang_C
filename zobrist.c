#include "gobang.h"

// zobbbrist结构，存有特征值和分数
struct HashItem{ 
    unsigned long long checkKey;
    int value;
}zobristMap[ZOBRISTMAPMAX];

// 初始化黑白棋子特征值，将对应数组填满64位随机数
void zobristInit(){
    int i,j;
    for(i=0;i<SIZE;i++){
        for(j=0;j<SIZE;j++){
            zobristBlackMap[i][j]=genrand64_int64();
            zobristWriteMap[i][j]=genrand64_int64();
        }
    }
    chessKey=genrand64_int64();
}

// zobrist置换表，获得棋局分
void zobrist(){
    totalTimes++;
    unsigned long zobristKey;
    zobristKey=(chessKey%ZOBRISTMAPMAX);
    // 如果哈希值相同，且特征值相同，全局分直接取对应zobristMap中存有的值
    if(zobristMap[zobristKey].checkKey==0||zobristMap[zobristKey].checkKey!=chessKey){
        wholeScoreUpdate();
        zobristMap[zobristKey].value=wholeScore;
        zobristMap[zobristKey].checkKey=chessKey;
    // 如果不同，则将全局分存入zobristMap对应的哈希值中
    }else{
        wholeScore=zobristMap[zobristKey].value;
        matchTimes++;
    }
}

// 重置zobristMap
void zobristMapReset(){
    int i;
    for(i=0;i<ZOBRISTMAPMAX;i++){
        zobristMap[i].checkKey=0;
    }
}
