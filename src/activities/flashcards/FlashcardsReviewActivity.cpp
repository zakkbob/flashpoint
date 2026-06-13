#include "FlashcardsReviewActivity.h"

#include <GfxRenderer.h>
#include <HalGPIO.h>
#include <I18n.h>

#include "components/UITheme.h"
#include "fontIds.h"

void FlashcardsReviewActivity::drawButtonHints() {
  switch (side) {
    case FRONT:
      GUI.drawButtonHints(renderer, tr(STR_BACK), "", "", tr(STR_SHOW), true);
      break;
    case BACK:
      GUI.drawButtonHints(renderer, tr(STR_AGAIN), tr(STR_HARD), tr(STR_GOOD), tr(STR_EASY), true);
      break;
  }
}

void FlashcardsReviewActivity::onEnter() {
  Activity::onEnter();

  requestUpdate();
};

void FlashcardsReviewActivity::render(RenderLock&&) {
  renderer.clearScreen();

  renderCard();
  drawButtonHints();

  renderer.displayBuffer();
}

void FlashcardsReviewActivity::renderCard() {
  const auto pageHeight = renderer.getScreenHeight();

  std::string text;
  switch (side) {
    case FRONT:
      text = card.front;
      break;
    case BACK:
      text = card.back;
      break;
  }

  renderer.drawCenteredText(SMALL_FONT_ID, pageHeight / 2, text.c_str());
}

void FlashcardsReviewActivity::loop() {
  const int pressedButton = mappedInput.getPressedFrontButton();

  if (side == FRONT) {
    switch (pressedButton) {
      case HalGPIO::BTN_BACK:
        onGoHome();
        return;
      case HalGPIO::BTN_RIGHT:
        side = BACK;
        requestUpdateAndWait();
        break;
      default:
        return;
    }
  } else if (side == BACK) {
    switch (pressedButton) {
      case HalGPIO::BTN_BACK:
        rate(AGAIN);
        break;
      case HalGPIO::BTN_CONFIRM:
        rate(HARD);
        break;
      case HalGPIO::BTN_LEFT:
        rate(AGAIN);
        break;
      case HalGPIO::BTN_RIGHT:
        rate(EASY);
        break;
      default:
        return;
    }
  }
}

void FlashcardsReviewActivity::rate(Rating rating) {
  LOG_DBG("FLASHCARDS", "Card rated %d", rating);
  side = FRONT;
  requestUpdateAndWait();
}
