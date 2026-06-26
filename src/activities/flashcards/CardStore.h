#pragma once

#include <FSRS.h>

#include <map>
#include <string>
#include <vector>

enum CardType : int { New = 0, Learning, Review };

struct CardContents {
  std::string front;
  std::string back;
};

class CardStore {
 public:
  CardContents currentCard;

  virtual ~CardStore() = default;

  virtual void open() = 0;
  virtual void close() = 0;
  virtual void grade(Grade g) = 0;  // Grade current card, move on to next
  virtual void undo() = 0;          // Returns to previous flashcard
};
