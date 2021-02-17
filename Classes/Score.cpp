//
//  Score.cpp
//  snake-mobile
//
//  Created by Francisco Sanchez on 2/11/21.
//

#include "Score.h"
#include "cocos2d.h"
#include <string.h>

USING_NS_CC;

Score::Score()
{
    _maxLevel = UserDefault::getInstance()->getIntegerForKey("maxLevel", 1);
    _level = _maxLevel;
    CCLOG("maxLevel: %d", _maxLevel);
}

void Score::reset()
{
    for(int i = 1; i <= _maxLevel; i++) {
        std::string key = StringUtils::format("level%d", i);
        UserDefault::getInstance()->deleteValueForKey(key.c_str());
    }
    _maxLevel = 1;
    UserDefault::getInstance()->setIntegerForKey("maxLevel", _maxLevel);
    UserDefault::getInstance()->flush();
}

void Score::loadLevel(int level)
{
    std::string key = StringUtils::format("level%d", level);
    Data data = UserDefault::getInstance()->getDataForKey(key.c_str());
    if (data.getSize() > 0) {
        memcpy(&_scoreByLevel[_level], data.getBytes(), data.getSize());
    }
    _scoreByLevel[_level].points = 0;
}

void Score::loadAllLevels()
{
    for(int i = 1; i <= _maxLevel; i++)
    {
        loadLevel(i);
    }
}

void Score::setScore(int score)
{
    setScoreByLevel(_level, score);
}

void Score::addScore(int level, int delta)
{
    float score = getScore(level);
    score += delta;
    setScoreByLevel(level, score);
}

void Score::setScoreByLevel(int level, int score)
{
    _level = level;
    if (level > _maxLevel) {
        _maxLevel = level;
    }
    _scoreByLevel[level].points = score;
    if (score > _scoreByLevel[level].max)
    {
        _scoreByLevel[level].max = score;
    }
}

void Score::setAccuracy(int level, float value)
{
    if (value > _scoreByLevel[level].accuracy) {
        _scoreByLevel[level].accuracy = value;
    }
}

void Score::flush()
{
    std::string log = "";
   
    auto key = StringUtils::format("level%d", _level);
    int score = 0;
    if (_scoreByLevel.find(_level) != _scoreByLevel.end()) {
        score = _scoreByLevel[_level].max;
    }
    Data data;
    data.copy((unsigned char*) &_scoreByLevel[_level], sizeof(ScoreData));

    UserDefault::getInstance()->setDataForKey(key.c_str(), data);

    CCLOG("%s", log.c_str());
    CCLOG("UserDefault flush");
    UserDefault::getInstance()->flush();
}

void Score::setLevel(int level)
{
    _level = level;
    if (_level > _maxLevel) {
        _maxLevel = level;
    }
    loadLevel(level);
}

int Score::getScore(int level)
{
    if (_scoreByLevel.find(level) != _scoreByLevel.end()) {
        return _scoreByLevel[level].points;
    }
    _scoreByLevel[level].points = 0;
    return 0;
}

int Score::getMaxScore(int level)
{
    if (_scoreByLevel.find(level) != _scoreByLevel.end()) {
        return _scoreByLevel[level].max;
    }
    _scoreByLevel[level].max = 0;
    return 0;
}

void Score::setMaxLevel(int maxLevel)
{
    if (maxLevel > _maxLevel)
    {
        _maxLevel = maxLevel;
        UserDefault::getInstance()->setIntegerForKey("maxLevel", _maxLevel);
        UserDefault::getInstance()->flush();
    }
}

float Score::getAccuracy(int level)
{
    return getScoreData(level).accuracy;
}

Score::ScoreData Score::getScoreData(int level)
{
    setMaxLevel(_maxLevel);
    return _scoreByLevel[level];
}
