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
    virtual void setSource(Position &position) override;
    Position mCurrentPosition;

private:
    void Initialize();
    virtual Position PerformAction(float period) override;
    Position mDestination;
    Position mSource;
    Step mStep;
};



#endif /* MoveTo_hpp */
