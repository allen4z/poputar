//
//  GuitarTableView.h
//  poputar
//
//  Created by allen on 15/6/13.
//
//

#ifndef __poputar__GuitarTableView__
#define __poputar__GuitarTableView__

#include "cocos2d.h"
#include "extensions/cocos-ext.h"
#include "PluginHelper.h"

USING_NS_CC;
USING_NS_CC_EXT;
using namespace std;

struct BleInfo{
    string uuid;
    string name;
} ;

class GuitarTableViewDelegate{
public:
    virtual void selectGuitar(string name)=0;
};

class GuitarTableView:public Layer,public TableViewDelegate,public TableViewDataSource,public GuitarInfoObserver{
public:
    
    GuitarTableViewDelegate* _delegate;
    
    TableView* tableView;
    std::vector<BleInfo> item;
    
    cocos2d::Size tableViewSize;
    
    static GuitarTableView* createWithSize(cocos2d::Size size);
    virtual bool init(cocos2d::Size size);
    
    virtual void scrollViewDidScroll(cocos2d::extension::ScrollView* view) {};//滑动
    virtual void scrollViewDidZoom(cocos2d::extension::ScrollView* view) {};  //放大
    
    virtual ssize_t numberOfCellsInTableView(TableView *table);//设置Cell个数
    virtual void tableCellTouched(TableView* table, TableViewCell* cell);//触摸事件
    virtual cocos2d::Size tableCellSizeForIndex(TableView *table, ssize_t idx);//Cell大小
    virtual TableViewCell* tableCellAtIndex(TableView *table, ssize_t idx);//添加Cell
    
    virtual void setGuitarInfo(string uuid,string name);
    
    void setDelegate(GuitarTableViewDelegate* delegate);
};

#endif /* defined(__poputar__GuitarTableView__) */
