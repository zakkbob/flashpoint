#pragma once

#include <FSRS.h>

#include <map>
#include <string>
#include <vector>

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
  virtual bool grade(
      Grade g) = 0;         // Grade current card, move on to next. Returns true if there's another card, false if not
  virtual void undo() = 0;  // Returns to previous flashcard
  virtual void add(std::string question, std::string answer) = 0;
};
