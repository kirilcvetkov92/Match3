		#include "ViewGrid.h"

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
	
	// add new gems
	if (auto model = mModel.lock()) {
		const auto& gems = model->GetGems();
		for (auto iterator : gems) {
			auto& coordinate = iterator.first;
			auto& gem = iterator.second;
			auto result = mViews.insert({
				gem,
				std::make_unique<ViewGem>(gem, mGemDebugLabelOffset)
			});
			auto& viewsIterator = result.first;
			auto& view = viewsIterator->second;
			view->SetPosition(
				MapGridCoordinateToPosition(coordinate));
			AddChild(view.get());
		}
	}
}

Position ViewGrid::MapGridCoordinateToPosition(Coordinate coordinate) {
	Position position(
		(mPosition.mX + (coordinate.mX * mGemSpacing)),
		(mPosition.mY + (coordinate.mY * mGemSpacing)));
	return position;
}
