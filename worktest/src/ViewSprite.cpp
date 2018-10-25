#include "ViewSprite.h"
#include "Position.h"

void ViewSprite::Render(King::Engine& engine) {
    engine.Render(mTexture, mPosition.mX, mPosition.mY, mRotation);
    View::Render(engine);
}

void ViewSprite::SetTexture(King::Engine::Texture texture) {
    mTexture = texture;
}
