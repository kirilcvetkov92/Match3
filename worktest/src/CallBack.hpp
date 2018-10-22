//
//  CallBack.hpp
//  worktest
//
//  Created by Kiril Cvetkov on 10/21/18.
//

#ifndef CallBack_hpp
#define CallBack_hpp


#include <stdio.h>
#include "Action.hpp"
#include <functional>

#define     GAME_CALLBACK(__selector__, __target__,...)   std::bind(&__selector__,__target__)
#define     GAME_CALLBACK_0(__selector__, __target__,...)   std::bind(&__selector__,__target__, ##__VA_ARGS__)
#define     GAME_CALLBACK_1(__selector__,__target__, ...) std::bind(&__selector__,__target__, std::placeholders::_1, ##__VA_ARGS__)
#define     GAME_CALLBACK_2(__selector__,__target__, ...) std::bind(&__selector__,__target__, std::placeholders::_1, std::placeholders::_2, ##__VA_ARGS__)
#define     GAME_CALLBACK_3(__selector__,__target__, ...) std::bind(&__selector__,__target__, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, ##__VA_ARGS__)


class CallBack : public King::Action
{
    public :
    CallBack(const std::function<void()> &func, float seconds);
    ~CallBack();
    int GetTimeRemaining();
    
protected:
    
    std::function<void()> mFunction;
    
    void Initialize();
    virtual void PerformAction(float period) override;
    virtual void OnActionFinished() override;
    
};
#endif /* CallBack_hpp */
