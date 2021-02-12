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
    std::map<int, float> _scoreByLevel;
    int _level;
    int _maxLevel;

public:
    Score();
    ~Score();
    void setLevel(int level) ;
    void setScore(float score);
    void setScoreByLevel(int level, float score);
    float getScore(int level);
};

#endif /* Score_hpp */
