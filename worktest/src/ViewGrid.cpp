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
    RemoveGems();
    UpdateGemViews();
}

void ViewGrid::RemoveGems()
{
    for (auto iterator = mViews.begin(); iterator != mViews.end();) {
        if (iterator->first.expired()) {
            RemoveChild(iterator->second.get());
            iterator = mViews.erase(iterator);
        } else {
            iterator++;
        }
    }
}

void ViewGrid::UpdateGemViews()
{
    if (auto model = mModel.lock()) {
        
        const auto& gems = model->GetGems();
        for (auto iterator : gems) {
            
            auto coordinate = iterator.first;
            auto &gemModel = iterator.second;
            
            bool c =false;
            if (!mViews.count(gemModel))
            {
                CreateNewGem(gemModel);
                c=true;
            }
            //update all gem transitions
            UpdateGemTransition(coordinate, gemModel,c);
            
        }
        //clear all gem transitions from model
        model->ClearTransitions();
    }
}

void ViewGrid::CreateNewGem(std::shared_ptr<ModelGem> &gem)
{
    auto result = mViews.insert({
        gem,
        std::make_unique<ViewGem>(gem, mGemDebugLabelOffset)}
                                );
    
    
    auto view = result.first->second.get();
    AddChild(view);
}

void ViewGrid::UpdateGemTransition(Coordinate &coordinate, std::shared_ptr<ModelGem> &gemModel, bool c)
{
    
    if (auto model = mModel.lock()) {
        auto &transitions = model->GetTransitions();
        auto &gemView = mViews.find(gemModel)->second;
        
        //get all gem transitions from the multimap
        auto allGemTransitions = transitions.equal_range(gemModel);
        
        for (auto it = allGemTransitions.first; it != allGemTransitions.second; it++)
        {
            
            auto transition = it->second;
            
            Position sourceCoordinate = transition.first;
            Position destinationCoordinate = transition.second;
            
            // if it's the first gem transition, set the initial position
            if(it==allGemTransitions.first && c)
                gemView->SetPosition(MapGridPositionToGlobalPosition(sourceCoordinate));
            
            float distance = abs(MapGridPositionToGlobalPosition(destinationCoordinate).mY - MapGridPositionToGlobalPosition(sourceCoordinate).mY)+abs(MapGridPositionToGlobalPosition(destinationCoordinate).mX-MapGridPositionToGlobalPosition(sourceCoordinate).mX);

            float time = distance/(Settings::SPEED);
            
            Position destination(MapGridPositionToGlobalPosition(destinationCoordinate));
            
            std::shared_ptr<MoveTo> action = std::make_shared<MoveTo>(destination, time);
            gemView->RunMoveAction(action);
        }
        
        bool containsActions = (gemView->mCurrentMoveAction || !gemView->mMoveActions.empty());
        if(containsActions)
            gemView->UpdateMoveActions();
        else
        {
            gemView->SetPosition(MapGridCoordinateToPosition(coordinate));
            
            auto &roof = model->GetRoof();
            Position destinationPosition = Position(coordinate.mX, coordinate.mY);
            if(roof.count(destinationPosition))
            {
                auto itr = roof.find(destinationPosition);
                Position sourcePosition = itr->second;
                roof.erase(sourcePosition);
                roof.erase(destinationPosition);
            }
        }
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


