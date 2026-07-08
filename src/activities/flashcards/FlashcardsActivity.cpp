#include "FlashcardsActivity.h"

#include <GfxRenderer.h>
#include <I18n.h>

#include "CardType.h"
#include "FlashcardReviewActivity.h"
#include "FlashcardSyncActivity.h"
#include "Scheduler.h"
#include "activities/Activity.h"
#include "activities/ActivityManager.h"
#include "components/UITheme.h"
#include "fontIds.h"

void FlashcardsActivity::onEnter() {
  Activity::onEnter();

  cards.open();
  requestUpdate();
};

void FlashcardsActivity::onExit() { Activity::onExit(); };

void FlashcardsActivity::render(RenderLock&&) {
  renderer.clearScreen();

  const auto pageWidth = renderer.getScreenWidth();
  const auto pageHeight = renderer.getScreenHeight();

  const auto& metrics = UITheme::getInstance().getMetrics();

  // Navigation
  tabs[currentTab].selected = true;
  GUI.drawTabBar(renderer, Rect{0, metrics.topPadding, pageWidth, metrics.tabBarHeight}, tabs, tabBarSelected);
  tabs[currentTab].selected = false;

  // Button
  const char* btnText = [this]() {
    switch (currentTab) {
      case DecksTab:
        return tr(STR_OPEN);
        break;
      case SyncTab:
        return tr(STR_SYNC);
        break;
      default:
        return "";
    }
  }();

  const int buttonWidth = renderer.getTextWidth(UI_10_FONT_ID, btnText) + 40;
  const int textHeight = renderer.getTextHeight(UI_10_FONT_ID);
  const int buttonHeight = textHeight + 20;

  renderer.drawRoundedRect(pageWidth / 2 - buttonWidth / 2, pageHeight / 2 - buttonHeight / 2, buttonWidth,
                           buttonHeight, 1, 6, true);

  if (!tabBarSelected) {
    renderer.fillRoundedRect(pageWidth / 2 - buttonWidth / 2, pageHeight / 2 - buttonHeight / 2, buttonWidth,
                             buttonHeight, 6, Color::Black);
  }

  renderer.drawCenteredText(UI_10_FONT_ID, pageHeight / 2 - textHeight / 2, btnText, tabBarSelected);

  if (tabBarSelected) {
    auto labels =
        mappedInput.mapLabels(tr(STR_BACK), tabs[(currentTab + 1) % tabCount].label, tr(STR_DIR_UP), tr(STR_DIR_DOWN));
    GUI.drawButtonHints(renderer, labels.btn1, labels.btn2, labels.btn3, labels.btn4);
  } else {
    auto labels = mappedInput.mapLabels(tr(STR_BACK), tabs[currentTab].label, tr(STR_DIR_UP), tr(STR_DIR_DOWN));
    GUI.drawButtonHints(renderer, labels.btn1, labels.btn2, labels.btn3, labels.btn4);
  }

  renderer.displayBuffer();
}

void FlashcardsActivity::loop() {
  if (mappedInput.wasPressed(MappedInputManager::Button::Back)) {
    onGoHome();
    return;
  }

  if (mappedInput.wasPressed(MappedInputManager::Button::Left) ||
      mappedInput.wasPressed(MappedInputManager::Button::Right) ||
      mappedInput.wasPressed(MappedInputManager::Button::Up) ||
      mappedInput.wasPressed(MappedInputManager::Button::Down)) {
    tabBarSelected = !tabBarSelected;
    requestUpdate();
  }

  if (mappedInput.wasPressed(MappedInputManager::Button::Confirm)) {
    if (tabBarSelected) {
      currentTab = (++currentTab) % tabCount;
      requestUpdate();
    } else {
      enterSelectedActivity();
    }
  }
}

void FlashcardsActivity::enterSelectedActivity() {
  switch (currentTab) {
    case DecksTab: {
      Scheduler scheduler(cards, 1);
      activityManager.pushActivity(std::make_unique<FlashcardReviewActivity>(renderer, mappedInput, cards, scheduler));
      break;
    }
    case SyncTab:
      activityManager.pushActivity(std::make_unique<FlashcardSyncActivity>(renderer, mappedInput, cards));
      break;
  }
}
