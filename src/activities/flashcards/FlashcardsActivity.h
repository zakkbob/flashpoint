#pragma once

#include <GfxRenderer.h>
#include <I18n.h>

#include "MemoryCardStore.h"
#include "activities/Activity.h"
#include "components/themes/BaseTheme.h"

const int tabCount = 2;

enum { DecksTab = 0, SyncTab };

class FlashcardsActivity final : public Activity {
 private:
  bool tabBarSelected = false;
  int currentTab = DecksTab;
  std::vector<TabInfo> tabs;
  MemoryCardStore cards;

  void enterSelectedActivity();

 public:
  FlashcardsActivity(GfxRenderer& renderer, MappedInputManager& mappedInput)
      : Activity("Flashcards", renderer, mappedInput) {
    tabs.reserve(tabCount);
    tabs.push_back({tr(STR_FLASHCARDS_DECKS), false});
    tabs.push_back({tr(STR_SYNC), false});
  };

  void onEnter() override;
  void onExit() override;
  void render(RenderLock&&) override;
  void loop() override;
};
