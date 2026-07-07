#pragma once

#include <FSRS.h>

#include <string>

#include "CardStore.h"
#include "activities/Activity.h"

enum Side : int { FRONT, BACK };

class FlashcardReviewActivity final : public Activity {
 private:
  CardStore& cards;
  Side side = FRONT;
  bool finished = false;

  void grade(Grade);
  void renderCard();
  void renderFinishScreen();
  void drawButtonHints();

 public:
  FlashcardReviewActivity(GfxRenderer& renderer, MappedInputManager& mappedInput, CardStore& cards)
      : Activity("FlashcardReview", renderer, mappedInput), cards(cards) {};

  void onEnter() override;
  void onExit() override;
  void render(RenderLock&&) override;
  void loop() override;
};
