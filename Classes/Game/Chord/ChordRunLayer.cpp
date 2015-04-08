//
//  ChordRunLayer.cpp
//  Guitars
//
//  Created by allen on 14-12-23.
//
//

#include "ChordRunLayer.h"
#include "MusicAnalysis.h"
#include "math.h"
//小节计数 当前播放的小节
int currentBeat;
//蓝牙计数器 用于提前获得下一个和弦
int nextBlueIndex = 0;
//当前小节和弦容器
Vector<Chord*> currBeatChords;
//下一小节的和弦容器
Vector<Chord*> waitBeatChords;
//当前弹奏和弦
Sprite* currCollision;
//是否第一个真实的节奏线 （第一个真实节奏线生成时需要传递蓝牙信息）
bool isFirst = true;

ChordRunLayer* ChordRunLayer::createChordRunLayer(MusicInfo *musicInfo){
    ChordRunLayer *layer = new ChordRunLayer();
    if(layer && layer->init4Chord(Color4B(0,0,0,0),musicInfo))
    {
        layer->autorelease();
        return layer;
    }
    CC_SAFE_DELETE(layer);
    return nullptr;
}

bool ChordRunLayer::init4Chord(const cocos2d::Color4B &color,MusicInfo *musicInfo){
//    common = Common::getInstance4Chord(visibleSize.width, visibleSize.height*0.7, musicInfo);
    chordConfig = new ChordConfig(visibleSize.width, visibleSize.height*0.8, musicInfo);
    gameConfig = chordConfig;
    bool result =  init(color,musicInfo);

    //初始化当前小节
//    currentBeat=0;
    //初始化第一小节的和弦
    this->getNewChords(currentBeat,true);
    //创建第一个节奏线
    this->getNewRhythm(true);
    
    //节拍音效
    

    schedule(schedule_selector(ChordRunLayer::rhythmMove), chordConfig->rhythm_time, kRepeatForever, chordConfig->startTime);
    
    return result;
}

void ChordRunLayer::endAnimationSetting(){
    currentBeat = 1;
}



//第一次碰撞时开始节拍器
bool isFirstCollision =true;
void ChordRunLayer::update(float dt){
    //当前小节的节奏线
    Rhythm *rhythm = (Rhythm *)this->getChildByTag(currentBeat);
    //循环当前和弦，查看是否发生碰撞
    for (auto &e:currBeatChords) {
        if(!e->isCollision && rhythm->isReal && rhythm->boundingBox().intersectsRect(e->boundingBox())){
            if(currCollision == NULL || e!=currCollision){
                if(isFirstCollision){
                    if(metronomePlay){
                        CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("snare.caf",false,6,0,1);
                    }
                    schedule(schedule_selector(ChordRunLayer::metronome), chordConfig->unitTime);
                    isFirstCollision = false;
                }
                currCollision = e;
                e->collisionAction(chordConfig);
                if(musicalPlay){
                    e->chordVoice();
                }
                e->isCollision = true;
                this->sendDataToBluetooth();
            }
        }
    }
}

void ChordRunLayer::metronome(float dt){
    if(metronomePlay){
        CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("snare.caf",false,6,0,1);
    }
}


void ChordRunLayer::rhythmMove(float dt){
    if(currentBeat){
        this->moveChords();
        this->getNewChords(currentBeat,true);
        currentBeat+=1;
    }
    this->getNewRhythm(false);
    
    if(isFirst){
        sendDataToBluetooth();
        isFirst = false;
    }
    
}

void ChordRunLayer::moveChords(){
    //当前弹奏的和弦移动到屏幕外
    for (auto &e:currBeatChords) {
        ActionInterval *ai =  e->moveOut(chordConfig);
        Sequence *removeSq=Sequence::create(ai,CallFunc::create([this,e](){
                this->removeChild(e);
        }),NULL);
        e->runAction(removeSq);

    }
    currBeatChords.clear();
    //等待弹奏的和弦移动到弹奏行
    for(auto &e : waitBeatChords){
        ActionInterval *ai = e->moveToCurrent(chordConfig);
        e->runAction(ai);
        currBeatChords.pushBack(e);
    }
    waitBeatChords.clear();
}

void ChordRunLayer::sendDataToBluetooth(){
    
//    long c = POPTHelper::getCurrentTime();
//    log("发送时间：%d",c);
    vector<string> bluetoothChord = chordConfig->musicInfo->getBluetoothChord();
    if(!(nextBlueIndex >= bluetoothChord.size())){
        string nextBlueType = bluetoothChord.at(nextBlueIndex);
        log("send BlueTooth : %s",nextBlueType.c_str());
        MusicAnalysis::getInstance()->sendChordStr(nextBlueType);
    }
//    long c1 = POPTHelper::getCurrentTime();
//    log("结束时间：%d",c1);
    nextBlueIndex++;
}



void ChordRunLayer::getNewChords(int currentBeat,bool isWait){
    //初始化和弦，并从Y轴的0移动的array4Y的第一个位置
    log("getNewChords:%i",currentBeat);
    ValueVector beatChords = chordConfig->musicInfo->getChords();
    int beatCount = chordConfig->musicInfo->getBeat();
     ValueVector lyrics = chordConfig->musicInfo->getLyircs();
    if(currentBeat<beatChords.size()){
        ValueVector lyricVector;
        if(currentBeat<lyrics.size()){
             lyricVector = lyrics.at(currentBeat).asValueVector();
        }
        
        ValueVector beatVector=beatChords.at(currentBeat).asValueVector();
        for (int j=0; j<beatCount; j++) {
            string type = beatVector.at(j).asString();
            
            string lyric_str;
            if(lyricVector.size()>0){
                lyric_str = lyricVector.at(j).asString();
            }
            Chord *chord = Chord::createChord(chordConfig, j, type,lyric_str);
            this->addChild(chord,2);
            if(isWait){
                chord->runAction(chord->moveToWait(chordConfig));
                waitBeatChords.pushBack(chord);
            }else{
                chord->runAction(chord->moveToCurrent(chordConfig));
                currBeatChords.pushBack(chord);
            }
        }
        
    }else{
        currentBeat = 999;
    }
}



void ChordRunLayer::restart(int musicalIndex){
    //停止任务
    unschedule(schedule_selector(ChordRunLayer::metronome));
    unschedule(schedule_selector(ChordRunLayer::rhythmMove));
    unscheduleUpdate();
    isFirstCollision =true;
    
    this->removeAllChildren();
    //计算音符所在小节

    currBeatChords.clear();
    waitBeatChords.clear();
    
    float temp = (float)musicalIndex/(float)chordConfig->beat;
    int beatFlag = ceil(temp);
    currentBeat = beatFlag+2;
    //设置蓝牙标记
    nextBlueIndex = musicalIndex;
    
    log("restart %i",currentBeat);

    this->getNewChords(beatFlag,false);
    this->getNewRhythm(false);
    this->getNewChords(beatFlag+1, true);
    sendDataToBluetooth();
    
    schedule(schedule_selector(ChordRunLayer::rhythmMove), chordConfig->rhythm_time, kRepeatForever, chordConfig->rhythm_time);
    scheduleUpdate();
    
}

void ChordRunLayer::getNewRhythm(bool first){
    //创建信息节奏线
    Rhythm *rhythmLine = Rhythm::createRhythm(chordConfig, currentBeat);
    this->addChild(rhythmLine,1);
    ActionInterval *ai;
    if(first){
        ai = rhythmLine->firstMoveRhythm(chordConfig);
    }else{
        ai = rhythmLine->moveRhythm(chordConfig);
    }
    rhythmLine->isReal = !first;
    
    Sequence *sequence = Sequence::create(ai,CallFunc::create([this,rhythmLine](){
        ActionInterval *leftAi =rhythmLine->leftMoveRhythm(chordConfig);
        Sequence *removeSq=Sequence::create(leftAi,CallFunc::create([this](){
            this->removeChildByTag(currentBeat>2 ? currentBeat-2 : 0);
        }), NULL);
        rhythmLine->runAction(removeSq);
    }),NULL);
    rhythmLine->runAction(sequence);
}

void ChordRunLayer::stopMusic(){
    unschedule(schedule_selector(ChordRunLayer::rhythmMove));
    unscheduleUpdate();
    unschedule(schedule_selector(ChordRunLayer::metronome));
    this->removeAllChildren();
    
    currBeatChords.clear();
    waitBeatChords.clear();

    nextBlueIndex = 0;
    
    isFirst = true;
    isFirstCollision =true;
    
    delete chordConfig;
}

