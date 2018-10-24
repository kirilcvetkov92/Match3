//
//  StateMachine.hpp
//  worktest
//
//  Created by Kiril Cvetkov on 10/23/18.
//

#ifndef StateMachine_hpp
#define StateMachine_hpp
#include <stdio.h>
#include "CallBack.hpp"

class StateMachine
{
   
 public :
    
    enum State
    {
        NEW,
        READY,
        TOUCH_BEGIN1,
        TOUCH_END1,
        TOUCH_BEGIN2,
        SWIPE,
        CORRECT_START_POSITION,
        SWIPE_CLICK,
        IDLE,
        END,
    };
    
    enum Event
    {
        NONE,
        INITIALIZED,
        TOUCH_END,
        TOUCH_BEGIN,
        PAUSE,
        RESUME,
        NOSWIPE,
        EXIT,
    };
    
    std::function<void(Event)> mCurrentStateFunction;
    StateMachine::State mCurrentState;
    
    StateMachine();
    
    void SetCurrentState(State state);
    State& GetCurrentState();
    void SendEvent(Event event);
    
    private :
        void OnNew(Event event);
        void OnReady(Event event);
        void OnTouchBegin1(Event event);
        void OnTouchEnd1(Event event);
        void OnTouchBegin2(Event event);
        void OnSwipe(Event event);
        void OnSwipeClick(Event event);
        void OnIdle(Event event);
        void OnEnd(Event event);
    void OnCorrectStartPosition(Event event);

};
#endif /* StateMachine_hpp */
