#include "FlashcardsReviewActivity.h"

#include <FSRS.h>
#include <GfxRenderer.h>
#include <HalGPIO.h>
#include <I18n.h>
#include <WiFi.h>
#include <esp_http_client.h>
#include <network/WifiPowerSaveGuard.h>

#include "AnkiConnect.h"
#include "activities/network/WifiSelectionActivity.h"
#include "components/UITheme.h"
#include "fontIds.h"

void FlashcardsReviewActivity::drawButtonHints() {
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

void FlashcardsReviewActivity::onEnter() {
  Activity::onEnter();

  cards.open();
  requestUpdate();

  WiFi.mode(WIFI_STA);
  startActivityForResult(std::make_unique<WifiSelectionActivity>(renderer, mappedInput),
                         [this](const ActivityResult& result) { onWifiSelectionComplete(!result.isCancelled); });
}

void FlashcardsReviewActivity::onWifiSelectionComplete(const bool success) {
  AnkiConnect anki("http://192.168.0.112:8765");
  anki.init();

  auto deckNames = anki.deckNames();
  if (!deckNames) {
    LOG_ERR("ANKI", "Failed to get deck names");
    return;
  }
}

void FlashcardsReviewActivity::onExit() {
  Activity::onExit();

  cards.close();
};

void FlashcardsReviewActivity::render(RenderLock&&) {
  renderer.clearScreen();

  if (finished) {
    renderFinishScreen();
  } else {
    renderCard();
  }

  drawButtonHints();

  renderer.displayBuffer();
}

void FlashcardsReviewActivity::renderCard() {
  const auto pageHeight = renderer.getScreenHeight();

  std::string text;
  switch (side) {
    case FRONT:
      text = cards.currentCard.front;
      break;
    case BACK:
      text = cards.currentCard.back;
      break;
  }

  renderer.drawCenteredText(UI_10_FONT_ID, pageHeight / 2, text.c_str());
}

void FlashcardsReviewActivity::renderFinishScreen() {
  const auto pageHeight = renderer.getScreenHeight();

  renderer.drawCenteredText(UI_10_FONT_ID, pageHeight / 2 - 16, "No more cards left");
  renderer.drawCenteredText(UI_10_FONT_ID, pageHeight / 2 + 16, "Well done!");
}

void FlashcardsReviewActivity::loop() {
  const int pressedButton = mappedInput.getPressedFrontButton();

  if (side == FRONT) {
    switch (pressedButton) {
      case HalGPIO::BTN_BACK:
        onGoHome();
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

void FlashcardsReviewActivity::grade(Grade g) {
  LOG_DBG("FLASHCARDS", "Card graded %d", g);
  finished = !cards.grade(g);
  side = FRONT;
  requestUpdateAndWait();
}
