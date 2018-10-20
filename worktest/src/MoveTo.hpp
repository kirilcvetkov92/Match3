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

namespace King
{
    class MoveTo : public King::Action {
        typedef Position Step;
        
        public :
        MoveTo(Position destination, float seconds, std::weak_ptr<View> view);
    private:
        void Initialize();
        virtual void PerformAction(float period) override;
        Position mDestination;
        Step mStep;
    };
}


#endif /* MoveTo_hpp */
