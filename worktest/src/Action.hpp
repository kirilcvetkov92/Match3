//
//  Action.hpp
//  worktest
//
//  Created by Kiril Cvetkov on 10/20/18.
//


#ifndef Action_hpp
#define Action_hpp


#include <king/Updater.h>
#include <chrono>
#include "Position.h"



using namespace std::chrono;
class Action : public King::Updater {
public:
    enum class State {
        FINISHED,
        PENDING,
        RUNNING,
    };
    
    Action(float seconds);
    ~Action();
    virtual void Update() override;
    
    State mState;

protected:
    virtual void PerformAction(float period) = 0;
    virtual void OnActionFinished() = 0;

    float mSeconds;
    steady_clock::time_point mBegin;
};



#endif /* Action_hpp */
