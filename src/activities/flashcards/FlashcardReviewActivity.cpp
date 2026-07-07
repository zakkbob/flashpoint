#include "FlashcardReviewActivity.h"

#include <FSRS.h>
#include <GfxRenderer.h>
#include <HalGPIO.h>
#include <I18n.h>

#include "components/UITheme.h"
#include "fontIds.h"

void FlashcardReviewActivity::drawButtonHints() {
  if (finished) {
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

  if (finished) {
    renderFinishScreen();
  } else {
    renderCard();
  }

  drawButtonHints();
  renderer.displayBuffer();
}

void FlashcardReviewActivity::renderCard() {
  const auto pageHeight = renderer.getScreenHeight();
  const auto card = cards.cardContents(due[i].id);

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
        requestUpdateAndWait();
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
  LOG_DBG("FLASHCARDS", "Card graded %d", g);

  auto card = due[i];
  scheduler.review(card.stability, card.difficulty, 0, card.lastReview == 0, scheduler.getInterval(card.stability) < 0,
                   g);  // FIX: hard-coded placeholders
  cards.addReview(0, card.id, g, 0);
  cards.updateCardParams(card.id, card.stability, card.difficulty);

  finished = ++i >= due.size();
  side = FRONT;

  requestUpdateAndWait();
}
