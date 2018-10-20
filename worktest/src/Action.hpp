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


namespace King
{
    using namespace std::chrono;
    class Action : public King::Updater {
    public:
        enum class State {
            FINISHED,
            RUNNING,
        };
        
        Action(float seconds);
        ~Action();
        virtual void Update() override;
        virtual void setSource(Position &position) = 0;

        State mState;

    protected:
        virtual Position PerformAction(float period) = 0;
        float mSeconds;
        steady_clock::time_point mBegin;
        
    };
}


#endif /* Action_hpp */
