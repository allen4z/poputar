//
//  GameConfig.h
//  poputar
//
//  Created by allen on 15-3-31.
//
//

#ifndef __poputar__GameConfig__
#define __poputar__GameConfig__

#include <stdio.h>
#include "MusicInfo.h"
USING_NS_CC;

class GameConfig{

public:
    ~GameConfig();
    //界面宽度
    float contentWidth;
    //界面高度
    float contentHeight;
    //音乐信息
    MusicInfo *musicInfo;
    //单元时间 音符间隔时间
    float unitTime;
    //开始时间
    float startTime=3;
    //歌曲总时长
    float musicTime;
    //结尾增加的时间
    float endTime=3;
    
    GameConfig(float contentWidth,float contentHeight,MusicInfo *music);
        
    static void Release();

};

#endif /* defined(__poputar__GameConfig__) */
