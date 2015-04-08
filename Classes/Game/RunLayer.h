//
//  RunLayer.h
//  poputar
//
//  Created by allen on 15-3-30.
//
//

#ifndef __poputar__RunLayer__
#define __poputar__RunLayer__

#include <stdio.h>
//#include "Common.h"
#include "POPTHelper.h"
#include "MusicInfo.h"
#include "GameConfig.h"

USING_NS_CC;
class RunLayer : public LayerColor{
protected:
    //画面大小
    Size visibleSize = Director::getInstance()->getVisibleSize();
public:
    //节拍是否播放
    bool metronomePlay;
    //音符/和弦 声音是否播放
    bool musicalPlay;
    //公告属性
    GameConfig *gameConfig;
    //初始化
    virtual bool init(const Color4B &color,MusicInfo *musicInfo);
    
    //发送蓝牙数据
    virtual void sendDataToBluetooth(){};
    //返回时清空当前播放的参数
    virtual void stopMusic(){};
    //动画结束后设置属性
    virtual void endAnimationSetting(){};
    //从index重新开始
    virtual void restart(int musicalIndex){};
    
    
};

#endif /* defined(__poputar__RunLayer__) */
