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
                auto view = result.first->second.get();
                AddChild(view);
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

                float distance = abs(MapGridCoordinateToPosition(destinationCoordinate).mY - MapGridCoordinateToPosition(sourceCoordinate).mY)+abs(MapGridCoordinateToPosition(destinationCoordinate).mX-MapGridCoordinateToPosition(sourceCoordinate).mX);
                float t = distance/(Settings::SPEED);

                Position destination(MapGridCoordinateToPosition(destinationCoordinate));

                std::shared_ptr<MoveTo> action = std::make_shared<MoveTo>(destination, t);
                view->RunAction(action);
            }
            else
            {
//                std::cout<<"--Coordinate--"<<coordinate.mX<<" "<<coordinate.mY<<std::endl;

                view->SetPosition(MapGridCoordinateToPosition(coordinate));
                view->UpdateAction();
            }
            
        }
        model->ClearTransitions();
    }
}

void ViewGrid::ApplyInteraction(Position onClick, Position onMove)
{
    Coordinate gemOnClick = MapPositionCoordinateToGrid(onClick);
    Coordinate gemOnMove= MapPositionCoordinateToGrid(onMove);

    if(!(gemOnClick==gemOnMove))
    {
        //find nearest neighbours
        if (auto model = mModel.lock()) {
            model->TryMatch(gemOnClick, gemOnMove);
        }
    }
}

Position ViewGrid::MapGridCoordinateToPosition(Coordinate coordinate) {
    Position position(
                      (mPosition.mX + (coordinate.mX * mGemSpacing)),
                      (mPosition.mY + (coordinate.mY * mGemSpacing)));
    return position;
}

Coordinate ViewGrid::MapPositionCoordinateToGrid(Position position) {
    int posX = fmax(0,fmin(Settings::MODEL_GRID_WIDTH-1, floor(position.mX+mGemSpacing/2-mPosition.mX)/(mGemSpacing)));
    int posY = fmax(0, fmin(Settings::MODEL_GRID_HEIGHT-1, floor(position.mY+mGemSpacing/2-mPosition.mY)/(mGemSpacing)));
    
    Coordinate coordinate(posX, posY);
    return coordinate;
}


