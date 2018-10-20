#include "ViewGrid.h"
#include "Settings.h"

ViewGrid::ViewGrid(
	float gemSpacing,
	float gemDebugLabelOffset)
: mGemSpacing(gemSpacing)
, mGemDebugLabelOffset(gemDebugLabelOffset)
{}

void ViewGrid::SetModel(std::weak_ptr<ModelGrid> model) {
	mModel = model;
}

void ViewGrid::UpdateViews() {
    //remove erased gems
    for (auto iterator = mViews.begin(); iterator != mViews.end();) {
        if (iterator->first.expired()) {
            RemoveChild(iterator->second.get());
            iterator = mViews.erase(iterator);
        } else {
            iterator++;
        }
    }
    
    // add new gems according to new coordinate
    if (auto model = mModel.lock()) {
        
        auto &transitions = model->GetTransitions();

        const auto& gems = model->GetGems();
        for (auto iterator : gems) {
            
            auto& coordinate = iterator.first;
            auto& gem = iterator.second;
            
            if (!mViews.count(gem))
            {
                
                auto result = mViews.insert({
                    gem,
                    std::make_unique<ViewGem>(gem, mGemDebugLabelOffset)}
                );
                AddChild(result.first->second.get());
            }
            
            auto result = mViews.find(gem);

            auto& view = result->second;
            auto transitionIterator = transitions.find(gem);
            
            if (transitionIterator != transitions.end())
            {
                auto transition = transitionIterator->second;
                Coordinate sourceCoordinate = transition.first;
                Coordinate destinationCoordinate = transition.second;

                view->SetPosition(MapGridCoordinateToPosition(sourceCoordinate));

                float distance = MapGridCoordinateToPosition(destinationCoordinate).mY - MapGridCoordinateToPosition(sourceCoordinate).mY;
                float t = distance/(Settings::SPEED*15);

                Position destination(MapGridCoordinateToPosition(destinationCoordinate));

                std::shared_ptr<MoveTo> action = std::make_shared<MoveTo>(destination, t);
                view->RunAction(action);
            }
            else
            {
                view->UpdateAction();
                if(!view->mAction)
                {
                    view->SetPosition(MapGridCoordinateToPosition(coordinate));
                }
            }
            
        }
        model->ClearTransitions();
    }
}

        
Position ViewGrid::MapGridCoordinateToPosition(Coordinate coordinate) {
    Position position(
                      (mPosition.mX + (coordinate.mX * mGemSpacing)),
                      (mPosition.mY + (coordinate.mY * mGemSpacing)));
    return position;
}


