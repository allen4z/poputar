//
//  PluginHelper.m
//  poputar
//
//  Created by allen on 15-3-15.
//
//

#import <UIKit/UIKit.h>
#import "BlueToothHelper.h"
#import "PluginHelper.h"
#import "GuitarTableView.h"


static BlueToothHelper *btHelper;

PluginHelper* PluginHelper::pluginHelper = NULL;

PluginHelper::PluginHelper(){}

PluginHelper::~PluginHelper(){}

PluginHelper* PluginHelper::getInstance(){
    if (pluginHelper == NULL) {
        pluginHelper = new PluginHelper();
    }
    return pluginHelper;
}

void PluginHelper::init(){
    btHelper = [BlueToothHelper shareManager];
    [btHelper didLoad];
    
}

void PluginHelper::destroy(){
    [btHelper disconnect];
}

void PluginHelper::scanBLEPeripherals(){
    [btHelper scanPeripherals];
}

void* PluginHelper::getBlueToothHelper(){
    return btHelper;
}


void PluginHelper::sendDate(unsigned char dataInfo[]){
    [btHelper sendData:(dataInfo)];
}

bool PluginHelper::isConnected(){
    return [btHelper isConnected];
}

void PluginHelper::connect(string uuid){
    NSString* nsUUID = [NSString stringWithUTF8String:uuid.c_str()];
    [btHelper connectPeripherals:nsUUID];
}


void PluginHelper::addObserver(GuitarInfoObserver *ob){
    obs.push_back(ob);
}

void PluginHelper::removeAllObserver(){
    obs.clear();
}

void PluginHelper::sendGuitarInfo(string uuid,string name){
    for (int i=0 ; i<obs.size(); i++) {
        obs.at(i)->setGuitarInfo(uuid, name);
    }
}