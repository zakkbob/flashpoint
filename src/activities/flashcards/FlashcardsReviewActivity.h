#pragma once

#include <FSRS.h>

#include <string>

#include "MemoryCardStore.h"
#include "activities/Activity.h"

enum Side : int { FRONT, BACK };

class FlashcardsReviewActivity final : public Activity {
 private:
  Side side = FRONT;
  MemoryCardStore cards;
  bool finished = false;

  void grade(Grade);
  void renderCard();
  void renderFinishScreen();
  void drawButtonHints();

 public:
  FlashcardsReviewActivity(GfxRenderer& renderer, MappedInputManager& mappedInput)
      : Activity("Flashcards", renderer, mappedInput) {};

  void onEnter() override;
  void onExit() override;
  void render(RenderLock&&);
  void loop();
};
