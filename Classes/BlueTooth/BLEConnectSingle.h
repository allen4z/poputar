//
//  BLEConnectSingle.h
//  poputar
//
//  Created by allen on 15-6-11.
//
//

#ifndef __poputar__BLEConnectSingle__
#define __poputar__BLEConnectSingle__

#include <stdio.h>
#include "../Base/Singleton.h"
#include <cocos2d.h>

#define CURR_BLE_UUID "ble_uuid"
#define CURR_BLE_NAME "ble_name"

USING_NS_CC;
class BLEConnectSingle:public Node{
public:
    static void startConnect();
    static void stopConnect();
    static void pauseConnect();
    static void resumeConnect();
    
    void bleConnectUpdate(float at);
private:
    BLEConnectSingle();
    ~BLEConnectSingle();
    
    static BLEConnectSingle* m_bleConnectSingle;
    
};
#endif /* defined(__poputar__BLEConnectSingle__) */
