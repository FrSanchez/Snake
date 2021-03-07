//
//  Score.hpp
//  snake-mobile
//
//  Created by Francisco Sanchez on 2/11/21.
//

#ifndef Score_hpp
#define Score_hpp

#include <map>

class Score
{
private:
    typedef struct _sd {
        int points = 0;
        float accuracy = 0;
        unsigned char stars = 0;
        int max = 0;
        int foodEat;
        int target;
        int snakeLength;
        float time;
        bool loaded = false;
    } ScoreData;
    std::map<int, ScoreData> _scoreByLevel;
    int _level;
    int _maxLevel;

public:
    Score();
    void flush();
    void setLevel(int level);
    void setMaxLevel(int maxLevel);
    void setScore(int score);
    void setScoreByLevel(int level, int score);
    void setAccuracy(int level, float value);
    float getAccuracy(int level);
    ScoreData getScoreData(int level);
    int getScore(int level);
    int getMaxScore(int level);
    int getMaxLevel() { return _maxLevel; }
    void addScore(int level, int delta);
    void loadLevel(int level);
    void loadAllLevels();
    void reset();
    void setFoodEat(int level, int value);
    void setTarget(int level, int value);
    void setSnakeLength(int level, int value);
    void setTime(int level, float value);
    unsigned char getStars(int level);
    void setStars(int level, unsigned char mask);
};

#endif /* Score_hpp */
