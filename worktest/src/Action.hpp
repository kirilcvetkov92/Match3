#include <king/Updater.h>

#include <chrono>
#include "View.h"



namespace King
{
    using namespace std::chrono;
    
    class Action : public King::Updater {
        
        enum class State {
            FINISHED,
            RUNNING,
        };
        
    public:
        Action(float seconds, std::weak_ptr<View> view);
        virtual void Update() override;
        virtual void PerformAction(float period) = 0;
        State mState;

    protected:
        float mSeconds;
        steady_clock::time_point mBegin;
        std::weak_ptr<View> mView;
    };
}
