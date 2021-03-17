//
//  UnityReward.hpp
//  snake-mobile
//
//  Created by Francisco Sanchez on 3/17/21.
//

#ifndef UnityReward_hpp
#define UnityReward_hpp

class UnityReward
{
public:
    virtual void rewardPlayer(const char *placementId) = 0;
};

#endif /* UnityReward_hpp */
