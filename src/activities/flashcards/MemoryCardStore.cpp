#include "MemoryCardStore.h"

#include <FSRS.h>

void MemoryCardStore::open() { currentCard = CardContents{"ERROR: No cards loaded", "ERROR: No cards loaded"}; }

bool MemoryCardStore::empty() { return cards.size() == 0; }

bool MemoryCardStore::grade(Grade g) {
  if (i >= cards.size() - 1) {
    return false;
  }
  currentCard = cards[++i];
  return true;
}

void MemoryCardStore::undo() {
  i = --i % cards.size();
  currentCard = cards[i];
}

void MemoryCardStore::add(std::string question, std::string answer) {
  cards.push_back(CardContents{question, answer});
  currentCard = cards[i];
}
