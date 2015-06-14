//
//  PluginHelper.h
//  poputar
//
//  Created by allen on 15-3-15.
//
//  cocos2d-x调用ios api的桥梁类
//
#ifndef __poputar__PluginHelper__
#define __poputar__PluginHelper__

#include <stdio.h>

using namespace std;

class GuitarInfoObserver{
public:
    virtual void setGuitarInfo(string uuid,string name)=0;
};



class PluginHelper{
    
private:
    vector<GuitarInfoObserver*> obs;
    
    PluginHelper();
    ~PluginHelper();
    
    static PluginHelper* pluginHelper;
public:
    
    static PluginHelper* getInstance();
    
    //初始化蓝牙信息
    void init();
    
    void destroy();
    
    //扫描并连接
    void scanBLEPeripherals();
    
    //static void connectPeripherals();
    
    void sendDate(unsigned char dataInfo[]);

    void* getBlueToothHelper();
    
    bool isConnected();
    
    void connect(string uuid);
    
    void addObserver(GuitarInfoObserver* ob);
    
    void removeAllObserver();
    
    void sendGuitarInfo(string uuid,string name);
};
#endif 
