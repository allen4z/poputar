//
//  Section.cpp
//  poputar
//
//  Created by allen on 15-4-28.
//
//

#include "../../Base/POPTBaseDefine.h"
#include <stdlib.h>
//和弦播放控制标识
int chordPlayKeyFlag=-1;
//和弦发送蓝牙控制标识
int chordSendBTKeyFlag = -1;
//发送右手去蓝牙控制标识
int sendRightBTKeyFlag = -1;
//主音控制标识
//int tonicPlayKeyFlag=-1;
//节拍器控制
int metronomeKeyFlag=-1;

Section* Section::createSection(MeasureInfo* measureInfo,int index,int type,int p_index,int s_index){
    Section *item = new Section();
    if(item && item->init()){
    
        PlayConfig* playConfig = (PlayConfig*)poptGlobal->gni->getConfig();
        
        //音乐信息
        MusicModel* musicModel = playConfig->musicModel;
        //小节拍数
        int beatFlag = musicModel->getBeatFlag();
        //单元宽度，拍与拍之间的距离
        float unitWidth = playConfig->unitWidth;
        //小节总宽度
        item->sectionWidth = unitWidth*beatFlag;
        float sectionX = item->sectionWidth*(float)(index-1);
        item->p_index = p_index;
        item->s_index = s_index;
        
        item->sectionIndex = index;
        item->type = type;
        item->playConfig = playConfig;
        item->setAnchorPoint(Vec2::ZERO);
        item->setPosition(Vec2(sectionX,0));
        item->loadMusical(measureInfo);
        item->autorelease();
        
        return item;
    }
    CC_SAFE_DELETE(item);
    return nullptr;
}


void Section::loadRhythm(){
    
    float unitWidth = playConfig->unitWidth;
    //音乐信息
    MusicModel* musicModel = playConfig->musicModel;
    //小节拍数
    int beatFlag = musicModel->getBeatFlag();
    
    //增加节奏线信息
    for(int i=0;i<beatFlag*2;i++){
        float x = unitWidth * i/2;
        ui::Scale9Sprite *fingerRhythem = ui::Scale9Sprite::create("game/play/rhythm.png");
        fingerRhythem->setAnchorPoint(Vec2(0.5,0));
        fingerRhythem->setPosition(x,playConfig->stringSideHeight);
        if(i==0){
            fingerRhythem->setPreferredSize(Size(fingerRhythem->getContentSize().width,playConfig->contentHeight-playConfig->stringSideHeight));
        }else{
            fingerRhythem->setPreferredSize(Size(fingerRhythem->getContentSize().width,playConfig->stringsHeight));
        }
        
        this->addChild(fingerRhythem);
        
    }
}

void Section::loadMusical(MeasureInfo* measureInfo){
    
    //和弦声音控制
    __NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(Section::chordVoiceCallback), POPT_CHORD_VOICE , NULL);
    //主音声音控制
    __NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(Section::toincVoiceCallBack), POPT_TOINC_VOICE , NULL);
    //节拍器声音控制
    __NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(Section::metronomeVoiceCallback), POPT_METRONOME_VOICE , NULL);
    //音调控制（品夹）
    __NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(Section::capoChangeCallback), POPT_CAPO_VALUE , NULL);
    
    
    //默认和弦声音关闭，主音开放，节拍器关闭
    chordVoice = false;
    toincVoice = false;
    metronomeVoice = false;
    capoValue = 1.0f;
    
    float unitWidth = playConfig->unitWidth;

    //音乐信息
    MusicModel* musicModel = playConfig->musicModel;
    //小节拍数
    int beatFlag = musicModel->getBeatFlag();
    
    //正常部分和展示部分
    if(type==SECTION_FORMAL_CHORD || type == SECTION_FORMAL_TONIC ||type==SECTION_SHOW){
        loadRhythm();
    }
    float minBeatWidth = unitWidth/minFlag; //最小单元长度 代表 0.25个节拍长度
    double minLength = 1.0000000f/(double)minFlag;
    int beatIndex=0;
    BeatInfo* beat;
    double bLengthFlag = 0;
    
    //正常部分
    if(type == SECTION_FORMAL_CHORD){
        //和弦信息
        map<int,BeatInfo*> beats = measureInfo->beats;
        //组装头部和弦部分
        string lastChordFlag;
        for (int b=0; b<minFlag*beatFlag; b++) {
            float bX = minBeatWidth *b      ; //当前x位置
            
            if(bLengthFlag == 0 && beatIndex<beats.size()){
                beat = beats.at(beatIndex+1);//[beatIndex];
                beatIndex++;
                bLengthFlag = beat->length;
                //新建一个和弦
                
                if(beat->chordInfo == nullptr){
                    continue;
                }
                string chordType = beat->chordInfo->chordType;
                if(chordType=="" && chordType.empty()){
                    continue;
                }
                float chordY = playConfig->contentHeight-playConfig->chordHeight;
                
                if(lastChordFlag.empty() || lastChordFlag != chordType){
                    //头部和弦信息
                    lastChordFlag = chordType;
                    
                    //创建和弦色块
                    //createHeadChord(chordType, Vec2(bX,chordY), Size(minBeatWidth*bLengthFlag/minLength, playConfig->chordHeight));
                    //和弦切换 如果第一个和弦和上一个小节最后一个
                    if(b!=0 || lastSection.empty() || lastSection != chordType){
                        
                        //头部和弦标识信息
                        Label *l = Label::createWithTTF(chordType, "fonts/STHeiti-Light.ttc", 50);
                        l->setColor(getChordColor(chordType));
                        l->setAnchorPoint(Vec2(0,0.5));
                        l->setPosition(Vec2(bX+20,chordY+playConfig->chordHeight/2));
                        this->addChild(l);
                        

                        auto chordPic = Sprite::create("game/play/"+getChordFlag(chordType)+"_chord.png");
                        chordPic->setAnchorPoint(Vec2(0,0.5));
                        chordPic->setPosition(Vec2(l->getPositionX()+l->getContentSize().width+20,chordY+playConfig->chordHeight/2));
                        this->addChild(chordPic);
                        
                        //log("label：%f --- pic：%f",bX+20,l->getPositionX()+l->getContentSize().width+20);
                    }
                    
                }else{
                    //createHeadChord(chordType, Vec2(bX,chordY), Size(minBeatWidth*bLengthFlag/minLength, playConfig->chordHeight));
                }
            }
            bLengthFlag -= minLength;
        }
    }
    
    //正常部分和试听部分
    if(type == SECTION_FORMAL_CHORD||type == SECTION_FORMAL_TONIC){
        //和弦信息
        map<int,BeatInfo*> beats = measureInfo->beats;
        //和弦弹奏部分
        beatIndex=0;
        bLengthFlag = 0;
        
        for (int b=0; b<minFlag*beatFlag; b++) {
            
            float bX = minBeatWidth *b; //当前x位置
            
            if(bLengthFlag == 0 && beatIndex<beats.size()){
                beat = beats.at(beatIndex+1);
                beatIndex++;
                bLengthFlag = beat->length;
                string chordFlag;
                //和弦信息不等于空
                
                string lineFileName = "game/play/C_line.png";
                string circleFileName = "game/play/C_circle.png";
                
                
                if(type == SECTION_FORMAL_CHORD){
                    
                    //蓝牙信息
                    if(beat->chordInfo!= nullptr){
                        
                        string chordType = beat->chordInfo->chordType;
                        chordFlag = getChordFlag(chordType);
                        lineFileName = "game/play/"+chordFlag+"_line.png";
                        circleFileName = "game/play/"+chordFlag+"_circle.png";
                        
                        string stringSpecialNote = "";
                        if(beat->stroke != nullptr){
                            stringSpecialNote = beat->stroke->strokSpecial;
                        }
                        //初始化左手和弦信息
                        unsigned char* chordDatas = musicAnalysis->sendChordStr(beat->chordInfo->chordType, stringSpecialNote);
                        leftBlueToothDatas[b] = chordDatas;
                        unsigned char* leftChars= musicAnalysis->sendRightChar(13, 5);
                        rightBlueToothDatas[b] = leftChars;
                    }else{
                        unsigned char* tonicDatas = musicAnalysis->sendMusicChar(beat->noteInfo->noteInfo,"");
                        leftBlueToothDatas[b] = tonicDatas;
                        vector<string> musicalVec =  POPTStringUtils::split(beat->noteInfo->noteInfo, ":");
                        int strInfo4Int = POPTStringUtils::stringToInt(musicalVec[0]);
                        unsigned char* leftChars= musicAnalysis->sendRightChar(strInfo4Int, playConfig->rhythmTime*1000/10);
                        rightBlueToothDatas[b] = leftChars;
                    }
                    
                    //界面拼装
                    if(beat->chordInfo!=nullptr && beat->stroke != nullptr){
                        string stringSpecialNote = "";
                        stringSpecialNote = beat->stroke->strokSpecial;
                        //新建一个扫弦区域
                        PlayChord* chord = PlayChord::createPlayChord(beat, lineFileName,circleFileName, bX);
                        this->addChild(chord);
                        playChords.insert(b, chord);
                            
                        //特殊弹奏
                        if(stringSpecialNote!=""){
                            vector<string> strings = POPTStringUtils::split(stringSpecialNote, "@|@");
                            for (int i=0; i<strings.size(); i++) {
                                vector<string> strVec = POPTStringUtils::split(strings[i], ":");
                                string strInfo = strVec[0];
                                string cop = strVec[1];
                                auto playStr = PlayString::createPlayString(strInfo,cop,circleFileName,bX);
                                this->addChild(playStr);
                            }
                        }
                    }else{
                        vector<string> strings = POPTStringUtils::split(beat->noteInfo->noteInfo, "@|@");
                        for (int i=0; i<strings.size(); i++) {
                            vector<string> strVec = POPTStringUtils::split(strings[i], ":");
                            string strInfo = strVec[0];
                            string cop = strVec[1];
                            auto playStr = PlayString::createPlayString(strInfo,cop,circleFileName,bX);
                            this->addChild(playStr);
                        }
                        
                        
                    }
                }else if(type == SECTION_FORMAL_TONIC){
                    unsigned char* tonicDatas = musicAnalysis->sendMusicChar(beat->noteInfo->noteInfo,"");
                    leftBlueToothDatas[b] = tonicDatas;
                    vector<string> musicalVec =  POPTStringUtils::split(beat->noteInfo->noteInfo, ":");
                    int strInfo4Int = POPTStringUtils::stringToInt(musicalVec[0]);
                    unsigned char* leftChars= musicAnalysis->sendRightChar(strInfo4Int, playConfig->rhythmTime*1000/10);
                    rightBlueToothDatas[b] = leftChars;
                    
                    
                    vector<string> strings = POPTStringUtils::split(beat->noteInfo->noteInfo, "@|@");
                    for (int i=0; i<strings.size(); i++) {
                        vector<string> strVec = POPTStringUtils::split(strings[i], ":");
                        string strInfo = strVec[0];
                        string cop = strVec[1];
                        auto playStr = PlayString::createPlayString(strInfo,cop,circleFileName,bX);
                        this->addChild(playStr);
                    }

                }
            }
            bLengthFlag -= minLength;
        }
    }
//        //主音信息
//        map<int,TonicInfo*> tonics = sectionInfo->tonics;
//        int tonicIndex=0;
//        double tLengthFlag = 0;
//        TonicInfo *tonic;
//        
//        string fileTemp = POPTStringUtils::intToString((rand()%7)+1);
//        
//        for (int t=0; t<minFlag*beatFlag; t++) {
//            float tX = minBeatWidth *t;
//            if(tLengthFlag == 0 && tonicIndex<tonics.size()){
//                tonic = tonics.at(tonicIndex+1);
//                tonicIndex++;
//                tLengthFlag = tonic->length;
//                if(!tonic->note.empty() && tonic->note!="" && tonic->note!="0"){
//                    __String *note = __String::create(tonic->note);
//                    playTonics[t]=tonic;
//                    if(type==SECTION_FORMAL_TONIC){
//                        //得到弦信息
//                        string tonicStrInfo = tonic2StrSingleton->getStrInfo(note->getCString());
//                        vector<string> strVec = POPTStringUtils::split(tonicStrInfo, ":");
//                        string strInfo = strVec[0];
//                        string cop = strVec[1];
//                        
//                        auto playStr = PlayString::createPlayString(strInfo,cop,"game/finger/"+fileTemp+"_circle.png",tX);
//                        
//                        this->addChild(playStr);
//                        
//                        //当前是主音弹奏，则将主音信息放入蓝牙map
//                        unsigned char* tonicDatas = musicAnalysis->sendMusicChar(tonicStrInfo,"");
//                        leftBlueToothDatas[t] = tonicDatas;
//                        
//                        vector<string> musicalVec =  POPTStringUtils::split(tonicStrInfo, ":");
//                        int strInfo4Int = POPTStringUtils::stringToInt(musicalVec[0]);
//                        unsigned char* leftChars= musicAnalysis->sendRightChar(strInfo4Int, playConfig->rhythmTime*1000/10);
//                        rightBlueToothDatas[t] = leftChars;
//                    }
//                }
//            }
//            tLengthFlag -= minLength;
//        }

}

string Section::getChordFlag(string chordType){
    string chordFlag = chordType.substr(0,1);
    if(chordFlag[0]>='a' && chordFlag[0]<='z'){
        
        chordFlag = POPTStringUtils::toUpper(chordFlag);
    }
    return chordFlag;
}

Color3B Section::getChordColor(string chordType){
    
    string chordFlag = getChordFlag(chordType);
    Color3B color;
    if (chordFlag=="C") {
        color=Color3B(240,209,0);
    }else if (chordFlag=="D") {
        color=Color3B(83,144,238);
    }else if (chordFlag=="E") {
        color=Color3B(240,209,0);
    }else if (chordFlag=="F") {
        color=Color3B(240,209,0);
    }else if (chordFlag=="G") {
        color=Color3B(161,201,14);
    }else if (chordFlag=="A") {
        color=Color3B(11,239,253);
    }else if (chordFlag=="B") {
        color=Color3B(240,209,0);
    }
    return color;
}


void Section::createHeadChord(string chordType, cocos2d::Vec2 position, cocos2d::Size preferredSize){
    //根据和弦类型来取和弦类型及颜色
    string fileName ;

    fileName = "game/base/chordbanner.png";
    
    ui::Scale9Sprite *css = ui::Scale9Sprite::create(fileName);
    css->setAnchorPoint(Vec2::ZERO);
    css->setPosition(position);
    css->setPreferredSize(preferredSize);
    this->addChild(css);
}


ActionInterval* Section::move(int sectionCount){
    
    int moveCount = (int)playConfig->contentWidth/(int)sectionWidth;
    if((int)playConfig->contentWidth%(int)sectionWidth >0){
        moveCount+=1;
    }
    moveCount+=sectionCount;
    float time = playConfig->allTime*moveCount;
    float distance = playConfig->rhythmWidth;
    MoveBy *beginMove = MoveBy::create(time, Vec2(-distance*moveCount,0));
    return beginMove;
}



bool Section::updateState(float relativePosX,int nType){
    bool result= false;
    PlayConfig* playConfig = (PlayConfig*)poptGlobal->gni->getConfig();
    
    float unitWidth = playConfig->unitWidth;
    float minBeatWidth = unitWidth/minFlag;
    int key = relativePosX/minBeatWidth;
    
    
    
    //如果key为奇数，则是每拍的开始和中间位置，节奏线的位置
    //应该在此处节拍器 默认开
    if(metronomeVoice && (nType == UPDATE_TYPE_CHORD ||nType == UPDATE_TYPE_TONIC) && key != metronomeKeyFlag && key%minFlag == 0){
        CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("audio/snare.caf",false,6,0,1);
        metronomeKeyFlag = key;
    }
    
    if(nType == UPDATE_TYPE_CHORD || nType == UPDATE_TYPE_TONIC){
        if(key!=chordPlayKeyFlag){  //切换播放和弦
            if(playChords.find(key) != playChords.end()){
                PlayChord* pc = playChords.at(key);
                if(pc->beatInfo->chordInfo!=nullptr){
                    string type = pc->beatInfo->chordInfo->chordType;
                    //播放声音
                    string chordFileName = "audio/chord/"+type+"_clean.caf";
                    if(chordVoice){
                        CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(chordFileName.c_str(),false,capoValue,0,1);
                    }
                }
               

                if(nType == UPDATE_TYPE_CHORD){
                    result = true;
                }
            }
            chordPlayKeyFlag = key;
        }
        
        
//        if(key!=tonicPlayKeyFlag){
//            
//            if(playTonics.find(key)!=playTonics.end()){
//                TonicInfo *tonic = playTonics[key];
//                
//                string note4Str = tonic->note;
//                if(!note4Str.empty() && note4Str != "" && note4Str !="0" ){
//                    //通知代理歌词信息
//                    _delegate->lyricCallbak(p_index, s_index, tonic->t_index);
//                    
//                    string scaleFileName ="audio/scale/"+note4Str+".mp3";
//                    if (toincVoice) {
//                        CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(scaleFileName.c_str(),false,capoValue,0,1);
//                    }
//                    
//                    if(nType == UPDATE_TYPE_TONIC){
//                        //通知右手区
//                        //string tonicStrInfo = tonic2StrSingleton->getStrInfo(tonic->note);
//                        
////                        if(rightBlueToothDatas.find(key) != rightBlueToothDatas.end()){
////                            unsigned char* rightDatas = rightBlueToothDatas[key];
////                            PluginHelper::sendDate(rightDatas);
////                        }
//                        result = true;
//                    }
//                }
//            }
//            tonicPlayKeyFlag = key;
//        }
    }else if(nType == UPDATE_TYPE_LEFT_BLUETOOTH){
        string musicModelType =  playConfig->musicModel->getType();
        if(key != chordSendBTKeyFlag){ //切换蓝牙发送和弦
            if(musicModelType == MUSICMODEL_TYPE_CHORD){
                if(leftBlueToothDatas.find(key)!=leftBlueToothDatas.end()){
                    unsigned char* tc = leftBlueToothDatas[key];
                    PluginHelper::getInstance()->sendDate(tc);
                    
                }
            }else if(musicModelType == MUSICMODEL_TYPE_TONIC){
                if(leftBlueToothDatas.find(key)!=leftBlueToothDatas.end()){
                    unsigned char* tc = leftBlueToothDatas[key];
                    PluginHelper::getInstance()->sendDate(tc);
                    
                }                
            }
            chordSendBTKeyFlag = key;
        }
    }else if(nType == UPDATE_TYPE_RIGHT_BLUETOOTH){
        if(key != sendRightBTKeyFlag){
            if(rightBlueToothDatas.find(key) != rightBlueToothDatas.end()){
                unsigned char* rightDatas = rightBlueToothDatas[key];
                PluginHelper::getInstance()->sendDate(rightDatas);
            }
            sendRightBTKeyFlag = key;
        }
    }
    
    return result;
}

void Section::chordVoiceCallback(Ref* ref){
    __Bool *b =(__Bool*)ref;
    chordVoice = b->getValue();
}

void Section::toincVoiceCallBack(Ref* ref){
    __Bool *b =(__Bool*)ref;
    toincVoice = b->getValue();
}

void Section::metronomeVoiceCallback(Ref *ref){
    __Bool *b =(__Bool*)ref;
    metronomeVoice  = b->getValue();
}

void Section::capoChangeCallback(cocos2d::Ref *ref){
    __Float* f = (__Float*) ref;
    capoValue = f->getValue();
}

//void Section::sendBlueTooth4Chord(PlayChord *chord){
//    string type = chord->beatInfo->chordInfo->chordType;
//}
//
//void Section::sendBlueTooth4ChordRight(PlayChord *chord){
//    
//}
//
//void Section::sendBlueTooth4Tonic(string strInfo){
////    MusicAnalysis::sendMusicChar(strInfo);
//}
//
//void Section::sendBlueTooth4tonicRight(string strInfo){
////    vector<string> musicalVec =  POPTStringUtils::split(strInfo, ":");
////    int strInfo4Int = POPTStringUtils::stringToInt(musicalVec[0]);
//////    MusicAnalysis::sendRightChar(strInfo4Int, 20);
//}

Section::~Section(){
    
    leftBlueToothDatas.clear();
    rightBlueToothDatas.clear();
    
    __NotificationCenter::getInstance()->removeObserver(this, POPT_CHORD_VOICE);
    __NotificationCenter::getInstance()->removeObserver(this, POPT_METRONOME_VOICE);
    __NotificationCenter::getInstance()->removeObserver(this, POPT_TOINC_VOICE);
    __NotificationCenter::getInstance()->removeObserver(this, POPT_CAPO_VALUE);
}

void Section::setDelegate(SectionDelegate *delegate){
    _delegate = delegate;
}

