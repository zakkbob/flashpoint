#pragma once

#include "CardStore.h"
#include "activities/Activity.h"

class FlashcardSyncActivity final : public Activity {
 private:
  CardStore& cards;

  void onWifiSelectionComplete(const bool success);

 public:
  FlashcardSyncActivity(GfxRenderer& renderer, MappedInputManager& mappedInput, CardStore& cards)
      : Activity("FlashcardSync", renderer, mappedInput), cards(cards) {};

  void onEnter() override;
  void onExit() override;
  void render(RenderLock&&) override;
  void loop() override;
};
