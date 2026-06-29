#include "MemoryCardStore.h"

#include <FSRS.h>

void MemoryCardStore::open() {
  cards.push_back(CardContents{"Who founded Hack Club?", "Zach Latta"});
  cards.push_back(CardContents{"4 * 8 = ?", "32"});
  cards.push_back(CardContents{"How many moons does Earth have?", "1"});

  currentCard = cards[i];
}

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

void MemoryCardStore::add(std::string question, std::string answer) { cards.push_back(CardContents{question, answer}); }
