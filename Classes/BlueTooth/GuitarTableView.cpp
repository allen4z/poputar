//
//  GuitarTableView.cpp
//  poputar
//
//  Created by allen on 15/6/13.
//
//

#include "../Base/POPTBaseDefine.h"

GuitarTableView* GuitarTableView::createWithSize(Size size){
    GuitarTableView *layer = new GuitarTableView();
    if(layer && layer->init(size))
    {
        layer->autorelease();
        return layer;
    }
    CC_SAFE_DELETE(layer);
    return nullptr;
}

bool GuitarTableView::init(Size size){
    if(!Layer::init()){
        return false;
    }
    tableViewSize=size;
    
    tableView = TableView::create(this, size);
    tableView->setDelegate(this);
    tableView->setDirection(cocos2d::extension::ScrollView::Direction::VERTICAL);//设置TableView方向
    tableView->ignoreAnchorPointForPosition(false);
    tableView->setAnchorPoint(Vec2::ZERO);//设置锚点
    tableView->setPosition(Vec2::ZERO);//设置位置
    tableView->setVerticalFillOrder(TableView::VerticalFillOrder::TOP_DOWN);//设置TableView的属性

    this->addChild(tableView);
    tableView->reloadData();//更新数据
    this->setTouchEnabled(true);//可触摸
    
    return true;
}

ssize_t GuitarTableView::numberOfCellsInTableView(TableView* tableView){
    return item.size();
}

//设置Cell大小
Size GuitarTableView::tableCellSizeForIndex(cocos2d::extension::TableView *table, ssize_t idx)
{
    return Size(tableViewSize.width,50);
}

//Item触摸事件
void GuitarTableView::tableCellTouched(cocos2d::extension::TableView *table, cocos2d::extension::TableViewCell *cell)
{
    __String* bleInfo = (__String*)cell->getUserData();
    
    if(_delegate != NULL){
        _delegate->selectGuitar(bleInfo->_string);
    }
}

//添加Cell
TableViewCell* GuitarTableView::tableCellAtIndex(cocos2d::extension::TableView *table, ssize_t idx)
{
    TableViewCell* cell = table->dequeueCell();
    if(!cell)
    {
        cell = new TableViewCell();
        cell->autorelease();
    }
    cell->removeAllChildrenWithCleanup(true);
    log("idx = %zd",idx);
    
//    auto menu = ItemAdapter::create();//适配类
//    menu->itemIcon->loadTexture(itemIcon.at(idx));//添加Icon
//    menu->itemName->setString(item.at(idx));//添加Text
//    menu->touchBtn->setTag(10+idx);
//    
//    menu->setContentSize(Size(winSize.width,winSize.height/8));//设置视图大小
//    log("menu.width = %lf,menu.height = %lf",menu->getContentSize().width,menu->getContentSize().height);
//    
//    menu->setSize(Size(winSize.width,winSize.height/8));//设置大小
//    menu->setLayoutType(cocos2d::ui::Layout::Type::RELATIVE);//设置布局类型
//    menu->ignoreAnchorPointForPosition(false);
//    menu->setAnchorPoint(Vec2::ZERO);//设置锚点
//    menu->touchBtn->addTouchEventListener(CC_CALLBACK_2(MenuTableView::touchEvent, this));//设置Item触摸事件
//    
//    cell->addChild(menu);
    
//    auto btn = ui::Button::create();
//    btn->setTitleFontName("fonts/STHeiti-Light.ttc");
//    btn->setTitleText(item.at(idx).name);
//    btn->setTitleFontSize(30);
//    btn->setAnchorPoint(Vec2::ZERO);
//    btn->setPosition(Vec2::ZERO);
//    cell->addChild(btn);
    
    BleInfo bi = item.at(idx);

    cell->setUserData(new __String(bi.uuid+"@|@"+bi.name));
    
    auto text = Label::createWithTTF(bi.name, "fonts/STHeiti-Light.ttc", 30);
    text->setAnchorPoint(Vec2::ZERO);
    text->setPosition(Vec2::ZERO);
    
    cell->addChild(text);
    
    
    return cell;
}

void GuitarTableView::setGuitarInfo(string uuid, string name){
    
    bool flag=false;
    for (int i=0; i<item.size(); i++) {
        BleInfo biInfo = item.at(i);
        if(biInfo.uuid == uuid){
            flag = true;
        }
    }
    if(!flag){
        BleInfo bi = {uuid,name};
        item.push_back(bi);
        tableView->reloadData();
    }
    
//    if(find(item.begin(), item.end(), bi) == item.end()){
//        item.push_back(bi);
//    }
}


void GuitarTableView::setDelegate(GuitarTableViewDelegate *delegate){
    _delegate = delegate;
}
