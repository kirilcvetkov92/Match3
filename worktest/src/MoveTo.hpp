//
//  MoveTo.hpp
//  worktest
//
//  Created by Kiril Cvetkov on 10/20/18.
//

#ifndef MoveTo_hpp
#define MoveTo_hpp

#include <stdio.h>
#include "Action.hpp"

class MoveTo : public King::Action {
    typedef Position Step;
    
    public :
        MoveTo(Position destination, float seconds);
        virtual void setSource(Position &position);
        Position mCurrentPosition;
        Position mDestination;

protected:
    void Initialize();
    virtual void PerformAction(float period) override;
    virtual void OnActionFinished() override;
    Position mSource;
    Step mStep;
};



#endif /* MoveTo_hpp */
