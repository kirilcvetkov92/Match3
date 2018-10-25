#pragma once

#include "ViewSprite.h"
#include "ModelGem.h"

#include <king/Engine.h>

class ViewGem : public ViewSprite {
public:
    ViewGem(
            const std::weak_ptr<ModelGem> model,
            float debugLabelOffset);
    ~ViewGem();
    virtual void Render(King::Engine& engine) override;
    virtual void UpdateMoveActions() override;
private:
    static const char* MapGemStateToDebugLabel(ModelGem::State state);
    static King::Engine::Texture MapGemColorToTexture(ModelGem::Color color);
    
    std::weak_ptr<ModelGem> mModel;
    float mDebugLabelOffset;
};
