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
  renderer.clearScreen();

  const auto pageHeight = renderer.getScreenHeight();
  renderer.drawCenteredText(UI_10_FONT_ID, pageHeight / 2, "Syncing...");  // WARN: Hard-coded text (temporary)

  renderer.displayBuffer();

  // TODO: progress stuff
}

std::string truncate(std::string s, int maxLen) {  // NOTE: where to put this
  if (s.length() > maxLen) {
    return s.substr(0, maxLen - 3) + "...";
  }
  return s;
}

void FlashcardSyncActivity::onWifiSelectionComplete(const bool success) {
  requestUpdateAndWait();

  LOG_INF("FlashcardSync", "Beginning sync");

  AnkiConnect anki("http://192.168.0.112:8765");  // FIXME: hard-coded
  anki.init();

  auto decks = anki.deckNamesAndIds();
  if (!decks) {
    LOG_ERR("FlashcardSync", "Failed to get decks");
    return;
  }

  for (auto deck : decks.val) {
    cards.addDeck(deck.id, deck.name);

    auto cardIds = anki.cardIdsByDeckName(deck.name);
    if (!cardIds) {
      LOG_DBG("FlashcardSync", "New deck; (%lld) %s", deck.id, truncate(deck.name, 50).c_str());
      LOG_ERR("FlashcardSync", "Failed to get cards in deck %s (%lld)", truncate(deck.name, 50).c_str(), deck.id);
      continue;
    }

    LOG_DBG("FlashcardSync", "New deck; (%lld) %s, %d cards", deck.id, truncate(deck.name, 50).c_str(),
            cardIds.val.size());

    for (auto cardId : cardIds.val) {
      auto card = anki.cardById(cardId);
      if (!card) {
        LOG_ERR("FlashcardSync", "Failed to get card %lld", cardId);
        continue;
      }
      LOG_DBG("FlashcardSync", "New card; (%lld) %s | %s", card.val.id, truncate(card.val.question, 50).c_str(),
              truncate(card.val.answer, 50).c_str());
      cards.addCard(card.val.id, deck.id, card.val.question, card.val.answer);
    }
  }

  LOG_INF("FlashcardSync", "Sync complete");

  finish();
}
