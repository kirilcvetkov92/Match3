#pragma once

#include "View.h"
#include "Position.h"
#include "ModelGrid.h"
#include "ModelGem.h"
#include "ViewGem.h"

#include <map>
#include <string>

class ViewGrid : public View {
public:
    ViewGrid(
             float gemSpacing,
             float gemDebugLabelOffset);
    
    void SetModel(std::weak_ptr<ModelGrid> model);
    
    void UpdateViews();
    
    Position MapGridCoordinateToPosition(Coordinate coordinate);
    Coordinate MapPositionCoordinateToGrid(Position coordinate);
    Position MapGridPositionToGlobalPosition(Position coordinate);
    
    bool ApplyInteraction(Position onClick, Position onMove);
    
    
private:
    void RemoveGems();
    void UpdateGemViews();
    void CreateNewGem(std::shared_ptr<ModelGem> &gem);
    void UpdateGemTransition(Coordinate &coodinate, std::shared_ptr<ModelGem> &gem, bool c);
    
    std::map<std::weak_ptr<ModelGem>, std::unique_ptr<ViewGem>, std::owner_less<std::weak_ptr<ModelGem>>> mViews;
    std::weak_ptr<ModelGrid> mModel;
    
    float mGemSpacing;
    float mGemDebugLabelOffset;
    
    
};
