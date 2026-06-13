#pragma once
#include "activities/Activity.h"

enum Rating : int { AGAIN, HARD, GOOD, EASY };
enum Side : int { FRONT, BACK };

struct Card {
 public:
  std::string front;
  std::string back;

  explicit Card(std::string front, std::string back) : front(front), back(back) {};
};

class FlashcardsReviewActivity final : public Activity {
 private:
  Side side = FRONT;

  Card card = Card("Is it flashcarding time already?", "Yes!");

  void rate(Rating);

  void renderCard();
  void drawButtonHints();

 public:
  explicit FlashcardsReviewActivity(GfxRenderer& renderer, MappedInputManager& mappedInput)
      : Activity("Flashcards", renderer, mappedInput) {};

  void onEnter() override;
  void render(RenderLock&&);
  void loop();
};
