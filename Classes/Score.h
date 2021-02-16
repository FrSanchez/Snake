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
        int points;
        float accuracy;
        int stars;
    } ScoreData;
    std::map<int, int> _scoreByLevel;
    std::map<int, float> _maxByLevel;
    int _level;
    int _maxLevel;

public:
    Score();
    void flush();
    void setLevel(int level);
    void setMaxLevel(int maxLevel);
    void setScore(float score);
    void setScoreByLevel(int level, float score);
    float getScore(int level);
    float getMaxScore(int level);
    int getMaxLevel() { return _maxLevel; }
    void addScore(int level, float delta);
    void loadLevel(int level);
    void loadAllLevels();
    void reset();
};

#endif /* Score_hpp */
