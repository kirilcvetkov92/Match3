#pragma once

#include "View.h"

class ViewSprite : public View {
public:
	virtual void Render(King::Engine& engine) override;
	void SetTexture(King::Engine::Texture texture);
	
private:
	King::Engine::Texture mTexture;
};
