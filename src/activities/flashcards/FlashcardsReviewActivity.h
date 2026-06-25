#pragma once

#include <FSRS.h>

#include <string>

#include "CardStore.h"
#include "activities/Activity.h"

enum Side : int { FRONT, BACK };

class FlashcardsReviewActivity final : public Activity {
 private:
  Side side = FRONT;
  CardStore cards;

  void grade(FSRS::Grade);

  void renderCard();
  void drawButtonHints();

 public:
  FlashcardsReviewActivity(GfxRenderer& renderer, MappedInputManager& mappedInput)
      : Activity("Flashcards", renderer, mappedInput) {};

  void onEnter() override;
  void onExit() override;
  void render(RenderLock&&);
  void loop();
};
