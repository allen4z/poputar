//
//  BLEConnectSingle.cpp
//  poputar
//
//  Created by allen on 15-6-11.
//
//

#include "../Base/POPTBaseDefine.h"

#define SCHEDULE Director::getInstance()->getScheduler()

BLEConnectSingle* BLEConnectSingle::m_bleConnectSingle = NULL;

BLEConnectSingle::BLEConnectSingle(){
    SCHEDULE-> schedule(schedule_selector(BLEConnectSingle::bleConnectUpdate), this, 10.0f, kRepeatForever, 0.0f, false);
}

BLEConnectSingle::~BLEConnectSingle(){
    SCHEDULE->unschedule(schedule_selector(BLEConnectSingle::bleConnectUpdate),this);
}

void BLEConnectSingle::startConnect(){
    m_bleConnectSingle = new BLEConnectSingle();
}

void BLEConnectSingle::stopConnect(){
    CC_SAFE_DELETE(m_bleConnectSingle);
}

void BLEConnectSingle::pauseConnect(){
    SCHEDULE->pauseTarget(m_bleConnectSingle);
}

void BLEConnectSingle::resumeConnect(){
    SCHEDULE->resumeTarget(m_bleConnectSingle);
}

void BLEConnectSingle::bleConnectUpdate(float at){
    //判断吉他是否连接
    bool isConnect =PluginHelper::isConnected();
    
    //吉他未连接则连接吉他
    if (!isConnect) {
        PluginHelper::scanBLEPeripherals();
    }
    
    //吉他已经连接则发送保持连接垃圾指令
    if(isConnect){
        unsigned char command[2];
        command[0] = 0xf5;
        command[1] = 0x00;
        PluginHelper::sendDate(command);
        log("ble ble ble connecting.......");
    }
}

