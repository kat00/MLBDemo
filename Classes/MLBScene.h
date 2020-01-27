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

#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
#include "extensions/cocos-ext.h"
#include "ui/CocosGUI.h"
#include <vector>

USING_NS_CC;
USING_NS_CC_EXT;

class CJsonQuery;

struct ScoreBoard
{
    Label*  homescore;
    Sprite* home;
    Sprite* away;
    Label*  awayscore;
};

class MLBScene : public cocos2d::Scene
{
public:
    static cocos2d::Scene* createScene();

    virtual ~MLBScene();
    virtual bool init();
    
    
    // implement the "static create()" method manually
    CREATE_FUNC(MLBScene);
    
    void onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event);
    
private:
    
    void AddBackground();
    void CreateLabels();
    void SetDate();
    void UpdateLabels();
    void ShowInfoScreen();
    void CloseInfoScreen();
    
    void ScaleScoreBoard(int game, float scale);
    
    void MoveItems(float value);
    
    void Clear();
    
    void QueryData();
    
    //ui::Layout* AddGame(const char* home, const char* away, int homescore, int awayscore );
    void AddGame(ScoreBoard& game, int home, int away, int homescore, int awayscore);
    void NoGames();
    
    ui::Layout* mView;
    Label* mDateLabel;
    Label* mUpperLabel;
    Label* mLowerLabel;
    int mItem;
    int mCount;
    
    std::vector<ScoreBoard> mScores;
    
    int mMonth;
    int mDay;
    int mYear;
    
    Size mVisibleSize;
    Vec2 mOrigin;
    
    CJsonQuery* mQuery;
    bool mQueryInProgress;
    
};

#endif // __HELLOWORLD_SCENE_H__
