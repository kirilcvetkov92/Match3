#include "ViewGem.h"
#include "Position.h"

#include <king/Updater.h>
#include <string>

ViewGem::ViewGem(
                 const std::weak_ptr<ModelGem> model,
                 float debugLabelOffset)
: mModel(model)
, mDebugLabelOffset(debugLabelOffset) {
    SetTexture(MapGemColorToTexture(mModel.lock()->mColor));
}

ViewGem::~ViewGem()
{
}

void ViewGem::Render(King::Engine& engine) {
    ViewSprite::Render(engine);
    
    if (auto model = mModel.lock()) {
        engine.Render(
                      King::Engine::Texts::TEXT_ITEM_01,
                      MapGemStateToDebugLabel(model->mState),
                      mPosition.mX + mDebugLabelOffset,
                      mPosition.mY + mDebugLabelOffset,
                      mRotation);
    }
}

const char* ViewGem::MapGemStateToDebugLabel(ModelGem::State state) {
    switch (state) {
        case ModelGem::State::DROPPING:
            return "d";
        case ModelGem::State::SWAPPING:
            return "s";
        case ModelGem::State::MATCHED:
            return "m";
        case ModelGem::State::FALLING:
            return "f";
        case ModelGem::State::RESTING:
            return " ";
        default:
            return "?";
    }
}

void ViewGem::UpdateMoveActions()
{
    View::UpdateMoveActions();
    if(!mCurrentMoveAction && mMoveActions.empty())
    {
        if (auto model = mModel.lock()) {
            if(model->mState==ModelGem::State::FALLING || model->mState==ModelGem::State::SWAPPING)
            {
                model->mState = ModelGem::State::RESTING;
            }
        }
    }
}

King::Engine::Texture ViewGem::MapGemColorToTexture(ModelGem::Color color) {
    switch (color) {
        case ModelGem::Color::BLUE:
            return King::Engine::TEXTURE_BLUE;
        case ModelGem::Color::GREEN:
            return King::Engine::TEXTURE_GREEN;
        case ModelGem::Color::RED:
            return King::Engine::TEXTURE_RED;
        case ModelGem::Color::YELLOW:
            return King::Engine::TEXTURE_YELLOW;
        case ModelGem::Color::PURPLE:
            return King::Engine::TEXTURE_PURPLE;
        default:
            return King::Engine::TEXTURE_RED;
    }
}
