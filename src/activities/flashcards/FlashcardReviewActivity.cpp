#include "FlashcardReviewActivity.h"

#include <GfxRenderer.h>
#include <HalGPIO.h>
#include <I18n.h>

#include "CardType.h"
#include "components/UITheme.h"
#include "fontIds.h"

void FlashcardReviewActivity::drawButtonHints() {
  if (scheduler.finished) {
    GUI.drawButtonHints(renderer, tr(STR_BACK), "", "", "", true);
    return;
  }

  switch (side) {
    case FRONT:
      GUI.drawButtonHints(renderer, tr(STR_BACK), tr(STR_SHOW), "", "", true);
      break;
    case BACK:
      GUI.drawButtonHints(renderer, tr(STR_AGAIN), tr(STR_HARD), tr(STR_GOOD), tr(STR_EASY), true);
      break;
  }
}

void FlashcardReviewActivity::onEnter() {
  Activity::onEnter();

  requestUpdate();
}

void FlashcardReviewActivity::onExit() {
  Activity::onExit();

  cards.close();
};

void FlashcardReviewActivity::render(RenderLock&&) {
  renderer.clearScreen();

  if (scheduler.finished) {
    renderFinishScreen();
  } else {
    renderCard();
  }

  drawButtonHints();
  renderer.displayBuffer();
  scheduler.resetTimer();
}

void FlashcardReviewActivity::renderCard() {
  const auto pageHeight = renderer.getScreenHeight();
  const auto card = cards.cardContents(scheduler.currentCardId);

  std::string text;
  switch (side) {
    case FRONT:
      text = card.question;
      break;
    case BACK:
      text = card.answer;
      break;
  }

  renderer.drawCenteredText(UI_10_FONT_ID, pageHeight / 2, text.c_str());
}

void FlashcardReviewActivity::renderFinishScreen() {
  const auto pageHeight = renderer.getScreenHeight();

  renderer.drawCenteredText(UI_10_FONT_ID, pageHeight / 2 - 16, "No more cards left");
  renderer.drawCenteredText(UI_10_FONT_ID, pageHeight / 2 + 16, "Well done!");
}

void FlashcardReviewActivity::loop() {
  const int pressedButton = mappedInput.getPressedFrontButton();

  if (side == FRONT) {
    switch (pressedButton) {
      case HalGPIO::BTN_BACK:
        finish();
        return;
      case HalGPIO::BTN_CONFIRM:
        side = BACK;
        requestUpdate();
        break;
      default:
        return;
    }
  } else if (side == BACK) {
    switch (pressedButton) {
      case HalGPIO::BTN_BACK:
        grade(AGAIN);
        break;
      case HalGPIO::BTN_CONFIRM:
        grade(HARD);
        break;
      case HalGPIO::BTN_LEFT:
        grade(GOOD);
        break;
      case HalGPIO::BTN_RIGHT:
        grade(EASY);
        break;
      default:
        return;
    }
  }
}

void FlashcardReviewActivity::grade(Grade g) {
  scheduler.grade(g);
  side = FRONT;
  requestUpdate();
}
