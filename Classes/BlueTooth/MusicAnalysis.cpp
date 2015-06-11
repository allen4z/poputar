//
//  ChordAnalysis.cpp
//  poputar
//
//  Created by allen on 15-3-27.
//
//
#include "../Base/POPTBaseDefine.h"

DECLARE_SINGLETON_MEMBER(MusicAnalysis);
map<string,string> chordInfo;

MusicAnalysis::MusicAnalysis(void){
    
    //读取和弦对应关系文件
    rapidjson::Document doc;
    std::string jsonInfo =FileUtils::getInstance()->getStringFromFile("config/chordInfo.json");
    
    doc.Parse<0>(jsonInfo.c_str());
    
    if(!doc.IsObject()){
        log("chordInfo.json is error!");
    }

    const rapidjson::Value &cArray = doc["chords"];
    if(!cArray.IsArray()){
        log("chords array is error!");
    }
    
    for(rapidjson::SizeType i=0;i<cArray.Size();i++){
        const rapidjson::Value &str = cArray[i];
        const rapidjson::Value &cType = str["type"];
        const rapidjson::Value &cData = str["data"];
        string type =cType.GetString();
        string data = cData.GetString();
        chordInfo.insert(pair<string, string>(type,data));
    }
    
}


unsigned char* MusicAnalysis::sendChordStr(string chord, string special){
    map<string,string>::iterator it;
    it = chordInfo.find(chord);
    string chordStr;
    if(it != chordInfo.end()){
        chordStr = chordInfo[chord];
    }
    chordStr=chordStr+"@|@"+special;
    if(!chordStr.empty()){
        return sendMusicChar(chordStr,"");
    }else{
        unsigned char* command = new unsigned char[2];
        command[0] = 0xf5;
        command[1] = 0x01;
        return command;
    }
}

unsigned char* MusicAnalysis::sendMusicChar(string musical,string nextMusical){
    unsigned char* command = new unsigned char[26];
    command[0] = 0xf5;
    command[1] = 0x01;
    
    for (int l=2; l<27; l++) {
        command[l] = 0x00;
    }
    
    vector<string> musicalVec = POPTStringUtils::split(musical, "@|@");
    for(size_t i=0;i<musicalVec.size();i++){
        string info = musicalVec[i];
        if(info.length()>0){
            vector<string> s = POPTStringUtils::split(info, ":");
            int strInfo = POPTStringUtils::stringToInt(s[0]);
            int gradeInfo = POPTStringUtils::stringToInt(s[1]);
            int index = getLeftStrIndex(strInfo, gradeInfo,false);
            command[index]+=getGradeChar(gradeInfo);
        }
    }
    
    vector<string> nextMusicalVec = POPTStringUtils::split(nextMusical, "@|@");
    for(size_t i=0;i<nextMusicalVec.size();i++){
        string info = nextMusicalVec[i];
        if(info.length()>0){
            vector<string> s = POPTStringUtils::split(info, ":");
            int strInfo = POPTStringUtils::stringToInt(s[0]);
            int gradeInfo = POPTStringUtils::stringToInt(s[1]);
            int index = getLeftStrIndex(strInfo, gradeInfo,true);
            command[index]+=getGradeChar(gradeInfo);
        }
    }
    
//    int commandIndex = 2;
//    map<int,int> strMap;
//    vector<string> musicalVec = POPTStringUtils::split(musical, "@|@");
//    for(size_t i=0;i<musicalVec.size();i++){
//        string info = musicalVec[i];
//        if(info.length()>0){
//            vector<string> s = POPTStringUtils::split(info, ":");
//            int key = POPTStringUtils::stringToInt(s[0]);
//            int int_value = POPTStringUtils::stringToInt(s[1]);
//            strMap[key] = int_value;
//        }
//    }
//    
//    for (int i=1; i<=6; i++) {
//        if(strMap.find(i)!=strMap.end() && strMap[i]!=0){
//            int grade = strMap[i];
//            if(grade<8){
//                command[commandIndex] = 0x00;
//                commandIndex++;
//                command[commandIndex] = getGradeChar(grade);
//            }else{
//                command[commandIndex] = getGradeChar(grade);
//                commandIndex++;
//                command[commandIndex] = 0x00;
//                
//            }
//            commandIndex++;
//        }else{
//            for (int l = 0; l<2; l++) {
//                command[commandIndex] = 0x00;
//                commandIndex++;
//            }
//        }
//    }
//    
//    for (int i=1; i<=6; i++) {
//        for (int l=0; l<2; l++) {
//            command[commandIndex] = 0x00;
//            commandIndex++;
//        }
//    }
    return command;
}

unsigned char* MusicAnalysis::sendRightChar(int com, int time){
    unsigned char* command = new unsigned char[5];
    command[0] = 0xf5;
    command[1] = 0x02;
    if(com < 7){
        command[2] = getRightChar(com);
    }else{
        command[2] = com;
    }
    command[3] = 0x00;
    command[4] = time;
    return command;
}

char MusicAnalysis::getGradeChar(int grade){
    char result =0;
    if(grade>7){
        grade-=7;
    }
    grade -= 1;
    result=1<<grade;
    log("%c",result);
    return result;
}

char MusicAnalysis::getRightChar(int strInfo){
    char result;
    switch (strInfo) {
        case 1:
            result = 6;
            break;
        case 2:
            result = 5;
            break;
        case 3:
            result = 4;
            break;
        case 4:
            result = 3;
            break;
        case 5:
            result = 2;
            break;
        case 6:
            result = 1;
            break;
        default:
            break;
    }
    return result;
}

int MusicAnalysis::getLeftStrIndex(int strInfo,int gradeInfo,bool weakFlag){
    int index;
    switch (strInfo) {
        case 1:
            index = 2;
            break;
        case 2:
            index = 4;
            break;
        case 3:
            index = 6;
            break;
        case 4:
            index = 8;
            break;
        case 5:
            index = 10;
            break;
        case 6:
            index = 12;
            break;
            
        default:
            break;
    }
    if(gradeInfo<=7){
        index += 1;
    }
    if(weakFlag){
        index +=12;
    }
    return index;
}


void MusicAnalysis::closeLight(){
    unsigned char* command = new unsigned char[26];
    command[0] = 0xf5;
    command[1] = 0x01;
    for (int i =2; i<27; i++) {
        command[i] = 0x00;
    }
    PluginHelper::sendDate(command);
}


MusicAnalysis::~MusicAnalysis(void){
    chordInfo.clear();
}



