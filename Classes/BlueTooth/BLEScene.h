//
//  BLEScene.h
//  Guitars
//  蓝牙连接界面
//  Created by allen on 15-3-17.
//
//

#ifndef __Guitars__BLEScene__
#define __Guitars__BLEScene__

#include "GuitarTableView.h"

USING_NS_CC;
using namespace std;


class BLEScene : public Layer,public GuitarTableViewDelegate{
private:
    
    vector<string> bleInfoVec;
    
    //连接蓝牙
    void connectBle(float dt);
    
    void scanBle(float dt);
    
    void getConnectState(float dt);
    //跳转到下一个场景
    void nextSence(float dt);
public:
    static cocos2d::Scene* createScene();
    
    virtual bool init();
    
    CREATE_FUNC(BLEScene);
    
    virtual void selectGuitar(string bleInfo);
};

#endif /* defined(__Guitars__BLEScene__) */
