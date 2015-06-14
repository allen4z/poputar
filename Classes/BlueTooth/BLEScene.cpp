//
//  BLEScene.cpp
//  Guitars
//
//  Created by allen on 15-3-17.
//
//

#include "../Base/POPTBaseDefine.h"
#include "GuitarTableView.h"

using namespace cocostudio::timeline;

int connectCount=0;

ui::Text *label;

Scene* BLEScene::createScene(){

    auto scene = Scene::create();
    auto layer = BLEScene::create();
//    Node *layer = CSLoader::createNode("BLEScene.csd");
    scene->addChild(layer);
    return scene;
}

bool BLEScene::init(){
    
    if ( !Layer::init() )
    {
        return false;
    }
    
    Size visibelSize = Director::getInstance()->getVisibleSize();
    auto rootNode = CSLoader::createNode("ble/ConnectLogo.csb");
    rootNode->setPosition(visibelSize.width/2,visibelSize.height/2);
    auto blinkAction = CSLoader::createTimeline("ble/ConnectLogo.csb");
    blinkAction->gotoFrameAndPlay(0, true);
    rootNode->runAction(blinkAction);
    
    addChild(rootNode);
    label = (ui::Text*)rootNode->getChildByName("Text_1");
    
    
    scheduleOnce(schedule_selector(BLEScene::scanBle), 0);
    
    
    string guitarName = UserDefault::getInstance()->getStringForKey(CURR_BLE_NAME);
    
    if(guitarName.empty()){
        label->setString("请选择要连接的吉他！");
        
        float gtvWidthSide = 240.0f;
        float gtvHeightSide = 150.0f;
        auto gtv = GuitarTableView::createWithSize(Size(gtvWidthSide,gtvHeightSide));
        gtv->setDelegate(this);
        gtv->setPosition(visibelSize.width/2-gtvWidthSide/2,0);
        
        this->addChild(gtv);
        
        PluginHelper::getInstance()->addObserver(gtv);
    }else{
        label->setString("开始连接:"+guitarName);
        
        scheduleOnce(schedule_selector(BLEScene::connectBle), 3.0f);
    }
    
    
    return true;
}

void BLEScene::scanBle(float dt){
    PluginHelper::getInstance()->scanBLEPeripherals();
}

void BLEScene::connectBle(float dt){
    string guitarUUID = UserDefault::getInstance()->getStringForKey(CURR_BLE_UUID);
    PluginHelper::getInstance()->connect(guitarUUID);
    schedule(schedule_selector(BLEScene::getConnectState), 3.0f, kRepeatForever, 0);
}

////连接蓝牙
//void BLEScene::connectBle(float dt){
//    connectCount++;
//    log("connectCount:%i",connectCount);
//    
//    if(PluginHelper::getInstance()->isConnected()){
//        label->setString("连接吉他成功");
//        unschedule(schedule_selector(BLEScene::connectBle));
//    }else if(connectCount >=CONNECT_CONTENT){
//        label->setString("连接吉他失败");
//         unschedule(schedule_selector(BLEScene::connectBle));
//    }
//    
//    if(!PluginHelper::getInstance()->isConnected() && connectCount<CONNECT_CONTENT){
//        PluginHelper::getInstance()->scanBLEPeripherals();
//    }else{
//        scheduleOnce(schedule_selector(BLEScene::nextSence), 2.0f);
//    }
//}

//跳转到下一界面
void BLEScene::nextSence(float dt){

    //保持蓝牙长连接
    BLEConnectSingle::startConnect();
    
    auto nextScene = GameLevel::createScene();
    auto transition = TransitionCrossFade::create(0.5f, nextScene);
    Director::getInstance()->replaceScene(transition);
}


void BLEScene::selectGuitar(string bleInfo){
    
    bleInfoVec = POPTStringUtils::split(bleInfo, "@|@");
    
    label->setString("开始连接:"+bleInfoVec[1]);
    
    if(!bleInfoVec.empty()){
        PluginHelper::getInstance()->connect(bleInfoVec[0]);
    }
    
    schedule(schedule_selector(BLEScene::getConnectState), 3.0f, kRepeatForever, 0);
}



void BLEScene::getConnectState(float dt){
    connectCount++;
    bool connectFlag = PluginHelper::getInstance()->isConnected();
    bool nextScene=false;
    if(connectFlag){
        label->setString("连接吉他成功");
        unschedule(schedule_selector(BLEScene::getConnectState));
        nextScene = true;
        if(!bleInfoVec.empty()){
            UserDefault::getInstance()->setStringForKey(CURR_BLE_UUID, bleInfoVec[0]);
            UserDefault::getInstance()->setStringForKey(CURR_BLE_NAME, bleInfoVec[1]);
        }
        
        
    }
    if(connectCount>=CONNECT_CONTENT){
        label->setString("连接吉他失败");
        unschedule(schedule_selector(BLEScene::getConnectState));
        nextScene = true;
    }
    if(nextScene){
         scheduleOnce(schedule_selector(BLEScene::nextSence), 2.0f);
    }
}

