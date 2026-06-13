#include "FlashcardsActivity.h"

#include <GfxRenderer.h>
#include <I18n.h>

#include "fontIds.h"

void FlashcardsActivity::onEnter() {
	Activity::onEnter();

	const auto pageHeight = renderer.getScreenHeight();

	renderer.clearScreen();
	renderer.drawCenteredText(SMALL_FONT_ID, pageHeight / 2, "It's flashcarding time!");
	renderer.displayBuffer();
};
