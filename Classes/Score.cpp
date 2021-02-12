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
    CCLOG("maxLevel: %d", _maxLevel);
}

void Score::loadLevel(int level)
{
    std::string key = StringUtils::format("level%d", level);
    _maxByLevel[level] = UserDefault::getInstance()->getFloatForKey(key.c_str(), 0);
    _scoreByLevel[level] = 0;
}

void Score::loadAllLevels()
{
    for(int i = 1; i <= _maxLevel; i++)
    {
        loadLevel(i);
    }
}

void Score::setScore(float score)
{
    setScoreByLevel(_level, score);
}

void Score::addScore(int level, float delta)
{
    float score = getScore(level);
    score += delta;
    setScoreByLevel(level, score);
}

void Score::setScoreByLevel(int level, float score)
{
    _level = level;
    if (level > _maxLevel) {
        _maxLevel = level;
    }
    _scoreByLevel[level] = score;
    if (score > _maxByLevel[level]) {
        _maxByLevel[level] = score;
    }
}

void Score::flush()
{
    std::string log = "";
   
    auto key = StringUtils::format("level%d", _level);
    float score = 0;
    if (_maxByLevel.find(_level) != _maxByLevel.end()) {
        score = _maxByLevel[_level];
    }
    log += StringUtils::format("level %d: %.1f ", _level, score);
    UserDefault::getInstance()->setFloatForKey(key.c_str(), score);

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

float Score::getScore(int level)
{
    if (_scoreByLevel.find(level) != _scoreByLevel.end()) {
        return _scoreByLevel[level];
    }
    _scoreByLevel[level] = 0;
    return 0;
}

float Score::getMaxScore(int level)
{
    if (_maxByLevel.find(level) != _maxByLevel.end()) {
        return _maxByLevel[level];
    }
    _maxByLevel[level] = 0;
    return 0;
}

void Score::setMaxLevel(int maxLevel)
{
    if (maxLevel > _maxLevel)
    {
        _maxLevel = maxLevel;
    }
}
