#include "MemoryCardStore.h"

#include <FSRS.h>

void MemoryCardStore::open() {
  cards.push_back(CardContents{"How many legs does a cat have?", "4"});
  cards.push_back(CardContents{"How many legs does a human have?", "2"});

  currentCard = cards[i];
}

void MemoryCardStore::grade(Grade g) {
  i++;
  i %= cards.size();
  currentCard = cards[i];
}

void MemoryCardStore::undo() {
  i--;
  i %= cards.size();
  currentCard = cards[i];
}
