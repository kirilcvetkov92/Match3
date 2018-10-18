#pragma once

#include "View.h"
#include "Position.h"

#include <king/Engine.h>
#include <king/Updater.h>
#include <string>

class ViewText : public View {
public:
	virtual void Render(King::Engine& engine) override;
	
	void SetTextItem(King::Engine::Texts textItem);

	void SetTextString(std::string textString);

private:
	King::Engine::Texts mTextItem;
	std::string mTextString;
};
