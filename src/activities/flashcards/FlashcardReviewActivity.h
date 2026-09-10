#pragma once

#include <string>

#include "CardStore.h"
#include "CardType.h"
#include "Scheduler.h"
#include "activities/Activity.h"

enum Side : int { FRONT, BACK };

class FlashcardReviewActivity final : public Activity {
 private:
  CardStore& cards;
  Scheduler scheduler;
  Side side = FRONT;

  void grade(Grade);
  void renderCard();
  void renderFinishScreen();
  void drawButtonHints();

 public:
  FlashcardReviewActivity(GfxRenderer& renderer, MappedInputManager& mappedInput, CardStore& cards, long long deckId)
      : Activity("FlashcardReview", renderer, mappedInput), cards(cards), scheduler(Scheduler(cards, deckId)) {};

  void onEnter() override;
  void onExit() override;
  void render(RenderLock&&) override;
  void loop() override;
};
