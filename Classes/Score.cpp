//
//  Score.cpp
//  snake-mobile
//
//  Created by Francisco Sanchez on 2/11/21.
//

#include "Score.h"
#include "cocos2d.h"

USING_NS_CC;

Score::Score()
{
    _maxLevel = UserDefault::getInstance()->getIntegerForKey("maxLevel", 1);
    _level = _maxLevel;
    for (int i = 1; i <= _maxLevel; i++) {
        std::string key = StringUtils::format("level%d",i);
        _scoreByLevel[i] = UserDefault::getInstance()->getFloatForKey(key.c_str(), 0);
    }
}

void Score::setScore(float score)
{
    setScoreByLevel(_level, score);
}

void Score::setScoreByLevel(int level, float score)
{
    if (level > _maxLevel) {
        _maxLevel = level;
    }
    if (_scoreByLevel.find(level) == _scoreByLevel.end() || score > _scoreByLevel[level]) {
        _scoreByLevel[level] = score;
    }
}

Score::~Score()
{
    for(int i = 1; i < _maxLevel; i++)
    {
        auto key = StringUtils::format("level%d", i);
        int score = 0;
        if (_scoreByLevel.find(i) != _scoreByLevel.end()) {
            score = _scoreByLevel[i];
        }
        UserDefault::getInstance()->setIntegerForKey(key.c_str(), score);
    }
}

void Score::setLevel(int level)
{
    _level = level;
    if (_level > _maxLevel) {
        _maxLevel = level;
    }
}
