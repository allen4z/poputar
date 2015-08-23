//
//  MusicModel.h
//  poputar
//
//  Created by allen on 15-4-27.
//
//

#ifndef __poputar__MusicModel__
#define __poputar__MusicModel__

#include <stdio.h>
#include "./LyricModel.h"

USING_NS_CC;

//音乐类型 -- 和弦
#define MUSICMODEL_TYPE_CHORD "CHORD"
//音乐类型 -- 主音
#define MUSICMODEL_TYPE_TONIC "TONIC"

using namespace std;

class BeatInfo;

class MeasureInfo;

class ChordInfo;

class NoteInfo;

class StrokeInfo;

class MusicModel{
private:
    string type; //音乐类型
    string key;
    string title; //标题
    int beatRef;//以及分音符为一拍
    int beatFlag; //每小节拍数
    float bpm; //速度
    vector<int> loops;
    map<int,MeasureInfo*> measures;//小节信息
    
public:
    static MusicModel* initWithFile(string fileName);
    string getType();
    string getTitle();
    int getBeatFlag();
    int getBeatRef();
    int getBpm();
    string getKey();
    map<int,MeasureInfo*> getMeasures();
    vector<int> getLoops();
    
    void unLoadMusicModel();
};

/*
 小节信息
 */
class MeasureInfo{
public:
    int s_index; //索引
    map<int,BeatInfo*> beats;
};


/*
 节拍信息
 */
class BeatInfo{
public:
    int b_index; //索引
    ChordInfo* chordInfo; //和弦类型
    double length; //长度 与主音长度一致
    StrokeInfo *stroke;
    NoteInfo* noteInfo;
};

/*
 和弦信息
 */
class ChordInfo{
public:
    string chordType;
    vector<int> defaultStrings;
};


class StrokeInfo{
public:
    int direction;
    vector<int> strokeStringInfo;
    string strokSpecial;
};

/*
 音符信息
 */
class NoteInfo{
public:
    string effect;
    string noteInfo;
};


#endif /* defined(__poputar__MusicModel__) */
