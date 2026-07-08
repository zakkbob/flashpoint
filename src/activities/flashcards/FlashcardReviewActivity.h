#pragma once

#include <FSRS.h>

#include <string>

#include "CardStore.h"
#include "activities/Activity.h"

enum Side : int { FRONT, BACK };

class FlashcardReviewActivity final : public Activity {
 private:
  CardStore& cards;
  long long deckId;
  std::vector<CardParams> due;
  int i = 0;
  bool finished = false;
  Side side = FRONT;
  Scheduler scheduler;

  void grade(Grade);
  void renderCard();
  void renderFinishScreen();
  void drawButtonHints();

 public:
  FlashcardReviewActivity(GfxRenderer& renderer, MappedInputManager& mappedInput, CardStore& cards,
                          Scheduler& scheduler, long long deckId)
      : Activity("FlashcardReview", renderer, mappedInput),
        cards(cards),
        scheduler(scheduler),
        deckId(deckId),
        due(cards.cardParamsByDeckId(deckId)),
        finished(due.size() == 0) {};

  void onEnter() override;
  void onExit() override;
  void render(RenderLock&&) override;
  void loop() override;
};
