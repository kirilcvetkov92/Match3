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
            
            auto gemKeyValue = mViews.find(gem);

            auto &gemView = gemKeyValue->second;
           
            auto result = transitions.equal_range(gem);
           
            for (auto it = result.first; it != result.second; it++)
            {
                
                auto transition = it->second;
          
                Position sourceCoordinateS = transition.first;
                Position destinationCoordinateS = transition.second;
                
                if(it==result.first)
                    gemView->SetPosition(MapGridPositionToGlobalPosition(sourceCoordinateS));
                
                float distance = abs(MapGridPositionToGlobalPosition(destinationCoordinateS).mY - MapGridPositionToGlobalPosition(sourceCoordinateS).mY)+abs(MapGridPositionToGlobalPosition(destinationCoordinateS).mX-MapGridPositionToGlobalPosition(sourceCoordinateS).mX);
                float t = distance/(Settings::SPEED);
                Position destination(MapGridPositionToGlobalPosition(destinationCoordinateS));
                
                std::shared_ptr<MoveTo> action = std::make_shared<MoveTo>(destination, t);
                gemView->RunMoveAction(action);
            }
        
            if(gemView->mCurrentMoveAction || !gemView->mMoveActions.empty())
                gemView->UpdateMoveActions();
            else
                gemView->SetPosition(MapGridCoordinateToPosition(coordinate));
        }
        model->ClearTransitions();
    }
}

bool ViewGrid::ApplyInteraction(Position onClick, Position onMove)
{
    Coordinate gemOnClick = MapPositionCoordinateToGrid(onClick);
    Coordinate gemOnMove= MapPositionCoordinateToGrid(onMove);

    if(!(gemOnClick==gemOnMove))
    {
        //find nearest neighbours
        if (auto model = mModel.lock()) {
            model->TryMatch(gemOnClick, gemOnMove);
            return true;
        }
    }
    return false;
}

Position ViewGrid::MapGridCoordinateToPosition(Coordinate coordinate) {
    Position position(
                      (mPosition.mX + (coordinate.mX * mGemSpacing)),
                      (mPosition.mY + (coordinate.mY * mGemSpacing)));
    return position;
}

Position ViewGrid::MapGridPositionToGlobalPosition(Position coordinate) {
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


