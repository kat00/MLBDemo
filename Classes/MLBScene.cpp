/****************************************************************************
 Copyright (c) 2017-2018 Xiamen Yaji Software Co., Ltd.
 
 http://www.cocos2d-x.org
 
 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:
 
 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 ****************************************************************************/

#include "MLBScene.h"
#include "SimpleAudioEngine.h"
#include "CJsonQuery.h"
#include "constants.h"
#include <ctime>
#include <string>

const float WIDTH_PAD = 270.0f;

void LoadData(int month, int day, int year, MLBScene* scene);

Scene* MLBScene::createScene()
{
    return MLBScene::create();
}

MLBScene::~MLBScene()
{
    if( mQuery )
    {
        delete mQuery;
    }
    
    Clear();
}

// Print useful error message instead of segfaulting when files are not there.
static void problemLoading(const char* filename)
{
    printf("Error while loading: %s\n", filename);
    printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in HelloWorldScene.cpp\n");
}

// on "init" you need to initialize your instance
bool MLBScene::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Scene::init() )
    {
        return false;
    }
    
    log("starting app");
    mVisibleSize = Director::getInstance()->getVisibleSize();
    mOrigin = Director::getInstance()->getVisibleOrigin();
    
    log("VisibleSize = %4f,%4f", mVisibleSize.width, mVisibleSize.height);
    log("Origin = %4f, %4f", mOrigin.x, mOrigin.y);
    
    
    auto listener = EventListenerKeyboard::create();
    listener->onKeyPressed = CC_CALLBACK_2(MLBScene::onKeyPressed, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
    
    AddBackground();
    CreateLabels();
    
    //set it to the default day
    mMonth = 6;
    mDay = 10;
    mYear = 2018;
    
    mQueryInProgress = false;
    
    //set to today example code
    //time_t today = time(0);
    //tm* lt = localtime(&today);
    //mYear = 1900 + lt->tm_year;
    //mMonth = 1 + lt->tm_mon;
    //mDay = lt->tm_mday;
    
    mQuery = 0;
    QueryData();
    
    return true;
}

void MLBScene::AddBackground()
{
    Sprite *background = Sprite::create("1.jpg", Rect(0, 0, mVisibleSize.width, mVisibleSize.height));
    background->setPosition(Vec2(mVisibleSize.width/2 + mOrigin.x, mVisibleSize.height/2 + mOrigin.y));
    this->addChild(background);
}

void MLBScene::CreateLabels()
{
    //add center text
    mUpperLabel = Label::create("", "fonts/arial.ttf", 30);
    mUpperLabel->setPosition(Vec2(mVisibleSize.width/2 + mOrigin.x, mVisibleSize.height/2 + 100));
    this->addChild(mUpperLabel);
    
    mLowerLabel = Label::create("", "fonts/arial.ttf", 20);
    mLowerLabel->setPosition(Vec2(mVisibleSize.width/2 + mOrigin.x, mVisibleSize.height/2 - 100));
    this->addChild(mLowerLabel);
    
    mDateLabel = Label::create("", "fonts/arial.ttf", 15);
    mDateLabel->setPosition(Vec2(mOrigin.x + 75,  100));
    this->addChild(mDateLabel);
}

void MLBScene::UpdateLabels()
{
    if( mCount == 0)
    {
        mUpperLabel->setString("");
        mLowerLabel->setString("");
        return;
    }
    
    mUpperLabel->setString(mQuery->GetUpperLabel(mItem));
    mLowerLabel->setString(mQuery->GetLowerLabel(mItem));
}

void MLBScene::SetDate()
{
    if( mDateLabel )
    {
        char buffer[64] = { 0 };
        snprintf(buffer, 63, "Date: %d-%d-%d", mMonth, mDay, mYear);
        mDateLabel->setString(buffer);
    }
}

void MLBScene::onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event)
{
    log("Key with keycode %d pressed", keyCode);
    
    //throttle input if query is in progress since we deleted the pointer
    if( mQueryInProgress )
        return;
    
    switch(keyCode)
    {
    case EventKeyboard::KeyCode::KEY_ENTER:
        {
            if( mCount == 0 )
                return;
            
            if(mView)
            {
                CloseInfoScreen();
            }
            else
            {
                ShowInfoScreen();
            }
        }
        break;
    case EventKeyboard::KeyCode::KEY_UP_ARROW:
        {
            CloseInfoScreen();
            
            AdvanceDate(mMonth,mDay,mYear);
            
            Clear();
            
            QueryData();
            
        }
        break;
    case EventKeyboard::KeyCode::KEY_DOWN_ARROW:
        {
            CloseInfoScreen();
            
            DecrementDate(mMonth,mDay,mYear);
            
            Clear();
            
            QueryData();
        }
        break;
    case EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
        {
            //dont do anything if are no games
            if( mCount == 0 )
                return;
            
            CloseInfoScreen();
            
            int loc = mItem;
            mItem = mItem == (mCount-1) ? mItem : mItem + 1;
            
            if( loc != mItem)
            {
                ScaleScoreBoard(loc, 1.0f);
                
                MoveItems(WIDTH_PAD * -1.0f);
                
                ScaleScoreBoard(mItem, 1.5f);
                UpdateLabels();
            }
        }
        break;
    case EventKeyboard::KeyCode::KEY_LEFT_ARROW:
        {
            //dont do anything if are no games
            if( mCount == 0 )
                return;
            
            CloseInfoScreen();
            
            int loc = mItem;
            mItem = mItem == 0 ? mItem : mItem - 1;
            
            if( loc != mItem)
            {
                ScaleScoreBoard(loc, 1.0f);
                
                MoveItems(WIDTH_PAD);
                
                ScaleScoreBoard(mItem, 1.5f);
                UpdateLabels();
            }
        }
    default:
        break;
    }
}

void MLBScene::MoveItems(float value)
{
    for(auto item : mScores)
    {
        if( item.homescore )
        {
            Vec2 pos = item.homescore->getPosition();
            pos.x += value;
            item.homescore->setPosition(pos);
        }
        
        if(item.home)
        {
            Vec2 pos = item.home->getPosition();
            pos.x += value;
            item.home->setPosition(pos);
        }
        
        if(item.away)
        {
            Vec2 pos = item.away->getPosition();
            pos.x += value;
            item.away->setPosition(pos);
        }
        
        if(item.awayscore)
        {
            Vec2 pos = item.awayscore->getPosition();
            pos.x += value;
            item.awayscore->setPosition(pos);
        }
    }
}

void MLBScene::QueryData()
{
    if( mQuery )
        delete mQuery;
    
    log("Querying date: %d-%d-%d", mMonth, mDay, mYear);
    
    mQueryInProgress = true;
    
    mQuery = new CJsonQuery();
    mQuery->SendRequest(mMonth, mDay, mYear);
    
    SetDate();
    
    //data is loaded so add elements
    mCount = mQuery->GetGameCount();
    if( mCount <= 0 )
    {
        NoGames();
        return;
    }
    
    for( int x = 0; x < mCount; ++x)
    {
        //const char* home = TeamNameFromID(mQuery->GetHomeTeamID(x));
        //const char* away = TeamNameFromID(mQuery->GetAwayTeamID(x));
        //Label* text = AddGame(home, away, mQuery->GetHomeScore(x), mQuery->GetAwayScore(x));
        float offsetx = mVisibleSize.width / 2 + mOrigin.x + ( x * WIDTH_PAD) - 90;
        float offsety = mVisibleSize.height/2 + mOrigin.y;
        
        ScoreBoard item;
        AddGame(item, mQuery->GetHomeTeamID(x), mQuery->GetAwayTeamID(x), mQuery->GetHomeScore(x), mQuery->GetAwayScore(x));
        
        //home score label
        if( item.homescore )
        {
            item.homescore->setPosition(Vec2(offsetx,offsety));
            this->addChild(item.homescore);
        }
        
        if(item.home)
        {
            offsetx += 65;
            item.home->setPosition(Vec2(offsetx, offsety));
            this->addChild(item.home);
        }
        
        if(item.away)
        {
            offsetx += 95;
            item.away->setPosition(Vec2(offsetx, offsety));
            this->addChild(item.away);
        }
        
        if(item.awayscore)
        {
            offsetx += 55;
            item.awayscore->setPosition(Vec2(offsetx,offsety));
            this->addChild(item.awayscore);
        }
        
        mScores.emplace_back(item);
    }
    
    mItem = 0;
    mView = 0;
    ScaleScoreBoard(0, 1.5f);
    UpdateLabels();
    
    mQueryInProgress = false;
}

void MLBScene::ScaleScoreBoard(int game, float scale)
{
    auto& scoreitem = mScores[game];
    
    if( scoreitem.home )
        scoreitem.home->setScale(scale);
    
    if( scoreitem.away )
        scoreitem.away->setScale(scale);
    
    if( scoreitem.homescore )
        scoreitem.homescore->setScale(scale);
    
    if( scoreitem.awayscore )
        scoreitem.awayscore->setScale(scale);
}

void MLBScene::AddGame(ScoreBoard& game, int home, int away, int homescore, int awayscore)
{
    game.homescore = Label::create(std::to_string(homescore), "fonts/arial.ttf", 30);
    game.awayscore = Label::create(std::to_string(awayscore), "fonts/arial.ttf", 30);
    
    std::string png = std::to_string(home);
    png += ".png";
    
    game.home = Sprite::create(png, Rect(0, 0, 64, 64));
    
    png = std::to_string(away);
    png += ".png";
    game.away = Sprite::create(png, Rect(0, 0, 64, 64));
    
}

/*Label* MLBScene::AddGame(const char* home, const char* away, int homescore, int awayscore )
{
    std::string teams = home;
    teams += "\n";
    teams += away;
    Label* text = Label::create(teams, "fonts/arial.ttf", 30);
    
    this->addChild(text);
    
    return text;
} */

void MLBScene::NoGames()
{
    Label* text = Label::create("No Games", "fonts/arial.ttf", 30);
    text->setPosition(Vec2(mVisibleSize.width / 2 + mOrigin.x , mVisibleSize.height/2));
    this->addChild(text);
    
    //add so we can remove easily later
    ScoreBoard board;
    board.homescore = text;
    board.away = 0;
    board.home = 0;
    board.awayscore = 0;
    mScores.push_back(board);
    
    //clear text labels
    mUpperLabel->setString("");
    mLowerLabel->setString("");
}


void MLBScene::Clear()
{
    for(auto iter : mScores)
    {
        if(iter.away)
            this->removeChild(iter.away);
        
        if(iter.home)
            this->removeChild(iter.home);
        
        if(iter.homescore)
            this->removeChild(iter.homescore);
        
        if(iter.awayscore)
            this->removeChild(iter.awayscore);
    }
    
    mScores.clear();
}

void MLBScene::CloseInfoScreen()
{
    if(mView)
    {
        this->removeChildByTag(50, true);
        mView = 0;
    }
}

void MLBScene::ShowInfoScreen()
{
    mView = ui::Layout::create();
    mView->setLayoutType( ui::Layout::Type::VERTICAL );
    mView->setSizeType( ui::Widget::SizeType::ABSOLUTE );
    mView->setSizePercent( Vec2( 1, 1 ) );
    mView->setBackGroundColorType( ui::Layout::BackGroundColorType::SOLID );
    mView->setBackGroundColor( Color3B::BLACK );
    
    mView->setPositionType( ui::Widget::PositionType::ABSOLUTE );
    mView->setPosition( Vec2( mVisibleSize.width * .25 + mOrigin.x, mVisibleSize.height * .25 ) );
    mView->setTag(50);
    mView->setContentSize(Size(mVisibleSize.width * .5, mVisibleSize.height * .5));
    
    std::string labelstring = TeamNameFromID(mQuery->GetHomeTeamID(mItem));
    labelstring += "(";
    labelstring += std::to_string(mQuery->GetHomeWins(mItem));
    labelstring += "-";
    labelstring += std::to_string(mQuery->GetHomeLosses(mItem));
    labelstring += ")    ";
    labelstring += std::to_string(mQuery->GetHomeScore(mItem));
    labelstring += "\n";
    
    labelstring += TeamNameFromID(mQuery->GetAwayTeamID(mItem));
    labelstring += "(";
    labelstring += std::to_string(mQuery->GetAwayWins(mItem));
    labelstring += "-";
    labelstring += std::to_string(mQuery->GetAwayLosses(mItem));
    labelstring += ")    ";
    labelstring += std::to_string(mQuery->GetAwayScore(mItem));
    labelstring += "\n\n";
    
    //pitcher stuff
    labelstring += "Winning Pitcher: ";
    labelstring += mQuery->GetWinner(mItem);
    labelstring += "\n";
    
    labelstring += "Losing Pitcher: ";
    labelstring += mQuery->GetLoser(mItem);
    labelstring += "\n";
    
    const char* save = mQuery->GetSave(mItem);
    if( save )
    {
        labelstring += "Save: ";
        labelstring += save;
        labelstring += "\n";
    }
    
    labelstring += mQuery->GetUpperLabel(mItem);
    labelstring += "\n";
    labelstring += mQuery->GetLowerLabel(mItem);
    
    Label* labeltext = Label::create(labelstring, "fonts/arial.ttf", 30);
    labeltext->setPosition( Vec2( mVisibleSize.width * 0.25, mVisibleSize.height * .25) );
    mView->addChild(labeltext);
    
    this->addChild(mView);
}


