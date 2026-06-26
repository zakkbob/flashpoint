#pragma once

#include <vector>

#include "CardStore.h"

class MemoryCardStore : public CardStore {
 private:
  std::vector<CardContents> cards;
  int i = 0;

 public:
  void open() override;
  void close() override {};
  void grade(Grade g) override;  // Grade current card, move on to next
  void undo() override;          // Returns to previous flashcard
};
