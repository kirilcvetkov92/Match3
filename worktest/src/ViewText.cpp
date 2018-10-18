#include "ViewText.h"

void ViewText::Render(King::Engine& engine) {
	engine.Render(mTextItem, mTextString.c_str(), mPosition.mX, mPosition.mY, mRotation);
}

void ViewText::SetTextItem(King::Engine::Texts textItem) {
	mTextItem = textItem;
}

void ViewText::SetTextString(std::string textString) {
	mTextString = textString;
}
