//
//  MusicModel.cpp
//  poputar
//
//  Created by allen on 15-4-27.
//
//

#include "../Base/POPTBaseDefine.h"

MusicModel* MusicModel::initWithFile(string fileName){
    MusicModel *music = new MusicModel();

    //读取和弦对应关系文件
    rapidjson::Document doc;
    
    std::string jsonInfo =FileUtils::getInstance()->getStringFromFile(fileName+".json");
    
    doc.Parse<0>(jsonInfo.c_str());
    
    if(!doc.IsObject()){
        log("music file is error!");
    }
    
    const rapidjson::Value &mType = doc["type"];
    const rapidjson::Value &mKey = doc["key"];
    const rapidjson::Value &mTitle = doc["title"];
    const rapidjson::Value &mBpm = doc["bpm"];
    const rapidjson::Value &mBeatFlag = doc["beatFlag"];
    const rapidjson::Value &mLoop = doc["loop"];
    const rapidjson::Value &mMeasures = doc["measures"];
    
    music->type = mType.GetString();
    music->key = mKey.GetString();
    music->title = mTitle.GetString();
    vector<string> beatInfo= POPTStringUtils::split(mBeatFlag.GetString(), ":");
    music->beatRef=POPTStringUtils::stringToInt(beatInfo[0]);
    music->beatFlag=POPTStringUtils::stringToInt(beatInfo[1]);
    music->bpm=mBpm.GetInt();
    
    
    string loopStr = mLoop.GetString();
    vector<string> loopStrVec = POPTStringUtils::split(loopStr, "@|@");
    for (int i=0; i<loopStrVec.size(); i++) {
        music->loops.push_back(POPTStringUtils::stringToInt(loopStrVec[i]));
    }
    
    //读取小节信息
    for (rapidjson::SizeType s=0; s<mMeasures.Size(); s++) {
        MeasureInfo *measureInfo = new MeasureInfo();
        
        const rapidjson::Value &measure = mMeasures[s];
        const rapidjson::Value &sIndex = measure["index"];
        measureInfo->s_index = sIndex.GetInt();
        
        //读取节拍信息
        const rapidjson::Value &beats = measure["beats"];
        if(!beats.IsNull()){
            for (rapidjson::SizeType b =0; b<beats.Size(); b++) {
                BeatInfo *beatInfo = new BeatInfo();
                const rapidjson::Value &bBeat = beats[b];
                const rapidjson::Value &bIndex = bBeat["index"];
                const rapidjson::Value &bChord = bBeat["chord"];
                const rapidjson::Value &bLength = bBeat["length"];
                const rapidjson::Value &bStroke = bBeat["stroke"];
                const rapidjson::Value &bNote = bBeat["note"];
                beatInfo->length = bLength.GetDouble();
                
                //读取和弦信息
                if(!bChord.IsNull()){
                    ChordInfo * chordInfo = new ChordInfo();
                    const rapidjson::Value &cChordType =  bChord["chordType"];
                    const rapidjson::Value &cDefaultStrings =  bChord["defaultStrings"];
                    
                    for (rapidjson::SizeType cd = 0; cd<cDefaultStrings.Size(); cd++) {
                        chordInfo->defaultStrings.push_back(cDefaultStrings[cd].GetInt());
                    }
                    chordInfo->chordType = cChordType.GetString();
                    
                    beatInfo->chordInfo = chordInfo;
                }

                //读取扫弦信息
                if(!bStroke.IsNull()){
                    StrokeInfo *strokeInfo = new StrokeInfo();
                    const rapidjson::Value &sDirection = bStroke["direction"];
                    const rapidjson::Value &sStrokSpecial = bStroke["strokSpecial"];
                    const rapidjson::Value &sStrokeStringInfo = bStroke["strokeStringInfo"];
                    
                    for (rapidjson::SizeType ss=0; ss<sStrokeStringInfo.Size(); ss++) {
                        strokeInfo->strokeStringInfo.push_back(sStrokeStringInfo[ss].GetInt());
                    }
                    strokeInfo->direction = sDirection.GetInt();
                    strokeInfo->strokSpecial = sStrokSpecial.GetString();
                    
                    beatInfo->stroke = strokeInfo;
                }
                
                //读取弹奏信息
                if(!bNote.IsNull()){
                    NoteInfo* noteInfo = new NoteInfo();
                    const rapidjson::Value &nEffect = bNote["effect"];
                    const rapidjson::Value &nNoteInfo = bNote["noteInfo"];
                    if(!nEffect.IsNull()){
                        noteInfo->effect= nEffect.GetString();
                    }
                    noteInfo->noteInfo= nNoteInfo.GetString();
                    
                    beatInfo->noteInfo = noteInfo;
                }
                

                measureInfo->beats[bIndex.GetInt()] = beatInfo;
            }
        }
        music->measures[sIndex.GetInt()]=measureInfo;
    }
    
    return music;
}

string MusicModel::getType(){
    return type;
}
string MusicModel::getKey(){
    return key;
}
string MusicModel::getTitle(){
    return title;
}
int MusicModel::getBeatFlag(){
    return beatFlag;
}
int MusicModel::getBeatRef(){
    return beatRef;
}
int MusicModel::getBpm(){
    return bpm;
}

map<int,MeasureInfo*> MusicModel::getMeasures(){
    return measures;
}

vector<int> MusicModel::getLoops(){
    return loops;
}


void MusicModel::unLoadMusicModel(){
    
    loops.clear();
    measures.clear();
    
//    lyricModel->unLoadLyric();
//    delete lyricModel;
}