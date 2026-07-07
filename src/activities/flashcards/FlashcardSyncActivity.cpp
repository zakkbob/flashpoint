#include "FlashcardSyncActivity.h"

#include <GfxRenderer.h>
#include <WiFi.h>

#include "AnkiConnect.h"
#include "activities/network/WifiSelectionActivity.h"
#include "fontIds.h"

void FlashcardSyncActivity::loop() {}

void FlashcardSyncActivity::onEnter() {
  Activity::onEnter();

  WiFi.mode(WIFI_STA);
  startActivityForResult(std::make_unique<WifiSelectionActivity>(renderer, mappedInput),
                         [this](const ActivityResult& result) { onWifiSelectionComplete(!result.isCancelled); });
}

void FlashcardSyncActivity::onExit() { Activity::onExit(); }

void FlashcardSyncActivity::render(RenderLock&&) {
  const auto pageHeight = renderer.getScreenHeight();

  renderer.drawCenteredText(UI_10_FONT_ID, pageHeight / 2, "Syncing...");  // WARN: Hard-coded text (temporary)

  // TODO: progress stuff
}

void FlashcardSyncActivity::onWifiSelectionComplete(const bool success) {
  requestUpdateAndWait();

  AnkiConnect anki("http://192.168.0.8:8765");
  anki.init();

  auto decks = anki.deckNamesAndIds();
  if (!decks) {
    LOG_ERR("ANKI", "Failed to get deck names");
    return;
  }

  for (auto deck : decks.val) {
    auto cardIds = anki.cardIdsByDeckName(deck.name);
    LOG_DBG("ANKI", "Deck %s has %d cards", deck.name.c_str(), cardIds.val.size());

    for (auto cardId : cardIds.val) {
      auto card = anki.cardById(cardId);
      LOG_DBG("ANKI", "Receieved card; id - %lld, question - %s, answer - %s", card.val.id, card.val.question.c_str(),
              card.val.answer.c_str());
      cards.addCard(card.val.id, deck.id, card.val.question, card.val.answer);
    }
  }

  finish();
}
