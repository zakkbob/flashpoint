#pragma once

#include <Arduino.h>
#include <Logging.h>

#include "CardStore.h"
#include "CardType.h"

class Scheduler {
 private:
  float w[21] = {0.212,  1.2931, 2.3065, 8.2956, 6.4133, 0.8334, 3.0194, 0.001,  1.8722, 0.1666, 0.796,
                 1.4835, 0.0614, 0.2629, 1.6483, 0.6014, 1.8729, 0.5425, 0.0912, 0.0658, 0.1542};  // Parameters
  float dr = 0.9;                                                                                  // Desired Retention

  CardStore& cards;
  long long deckId;
  std::vector<CardParams> due;
  unsigned long reviewStartTime = millis();
  int i = 0;

  void review(float& s, float& d, Grade g, unsigned long millisSince, bool isFirst, bool isSameDay);
  float interval();

 public:
  long long currentCardId = 0;
  int newCount = 0;
  int learningCount = 0;
  int reviewCount = 0;
  bool finished;

  Scheduler(CardStore& cards, long long deckId) : cards(cards), deckId(deckId) {
    due = cards.cardParamsByDeckId(deckId);
    finished = due.size() == 0;

    if (!finished) {
      currentCardId = due[i].id;
    }
  };

  void grade(Grade g);
  void resetTimer();
  int cardsDue() { return due.size(); };
};
