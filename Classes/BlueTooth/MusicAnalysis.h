//
//  ChordAnalysis.h
//  poputar
//  音乐信息翻译为蓝牙所需信息
//  Created by allen on 15-3-27.
//  
//

#ifndef __poputar__ChordAnalysis__
#define __poputar__ChordAnalysis__

#include <stdio.h>
#include "../Base/Singleton.h"
USING_NS_CC;
using namespace std;

class MusicAnalysis:public Singleton<MusicAnalysis>,public Node{
private:
    char getGradeChar(int grade);
    char getRightChar(int strInfo);
public:
    
    MusicAnalysis(void);
    ~MusicAnalysis(void);
    
    
    //和弦解析为音符字符串  和弦 特殊把位
    unsigned char* sendChordStr(string chord,string special);
    
    //将字符串解析为二维数组并组装成为02命令的指令
    unsigned char* sendMusicChar(string musical,string nextMusical);
    
    unsigned char* sendRightChar(int command,int time);
    
    int getLeftStrIndex(int strInfo,int greadInfo,bool weakFlag);
    
    void closeLight();
    
};
#define musicAnalysis MusicAnalysis::instance()
#endif /* defined(__poputar__ChordAnalysis__) */
