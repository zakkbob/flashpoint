#include "MemoryCardStore.h"

#include <FSRS.h>
#include <Logging.h>

#include <string>

bool MemoryCardStore::reviewExists(long long id) {
  for (int i = 0; i < reviews.size(); i++) {
    if (reviews[i].id == id) {
      return true;
    }
  }
  return false;
}

void MemoryCardStore::addReview(long long id, long long cardId, Grade grade, unsigned long duration) {
  reviews.push_back({.id = id, .cardId = cardId, .grade = grade, .duration = duration});
  for (int i = 0; i < cards.size(); i++) {
    if (cards[i].id == cardId) {
      cards[i].lastReview = id;  // id is timestamp
      return;
    }
  }
}

void MemoryCardStore::removeReview(long long id) {
  for (int i = 0; i < reviews.size(); i++) {
    if (reviews[i].id == id) {
      reviews.erase(reviews.begin() + i);
      return;
    }
  }
}

void MemoryCardStore::addCard(long long id, long long deckId, std::string question, std::string answer) {
  cards.push_back({.id = id, .deckId = deckId, .question = question, .answer = answer});
}

bool MemoryCardStore::cardExists(long long id) {
  for (int i = 0; i < cards.size(); i++) {
    if (cards[i].id == id) {
      return true;
    }
  }
  return false;
}

void MemoryCardStore::removeCard(long long id) {
  for (int i = 0; i < cards.size(); i++) {
    if (cards[i].id == id) {
      cards.erase(cards.begin() + i);
      removeReviewsByCardId(id);
      return;
    }
  }
}

void MemoryCardStore::removeReviewsByCardId(long long id) {
  for (int i = reviews.size(); i >= 0; i--) {
    if (reviews[i].cardId == id) {
      reviews.erase(reviews.begin() + i);
      return;
    }
  }
}

void MemoryCardStore::updateCardParams(long long id, float stability, float difficulty) {
  for (int i = 0; i < cards.size(); i++) {
    if (cards[i].id == id) {
      cards[i].stability = stability;
      cards[i].difficulty = difficulty;
      return;
    }
  }
}

bool MemoryCardStore::deckExists(long long id) {
  for (int i = 0; i < decks.size(); i++) {
    if (decks[i].id == id) {
      return true;
    }
  }
  return false;
}

void MemoryCardStore::addDeck(long long id, std::string name) { decks.push_back({.id = id, .name = name}); }

void MemoryCardStore::removeDeck(long long id) {
  for (int i = 0; i < decks.size(); i++) {
    if (decks[i].id == id) {
      decks.erase(decks.begin() + i);
      return;
    }
  }

  for (int i = cards.size() - 1; i >= 0; i--) {
    if (cards[i].deckId == id) {
      cards.erase(cards.begin() + i);
      removeReviewsByCardId(cards[i].id);
    }
  }
}

std::vector<CardParams> MemoryCardStore::cardParamsByDeckId(long long deckId) {
  std::vector<CardParams> params;

  for (const auto& card : cards) {
    if (card.deckId == deckId) {
      params.push_back({
          .id = card.id,
          .stability = card.stability,
          .difficulty = card.difficulty,
          .lastReview = card.lastReview,
      });
    }
  }

  return params;
}

std::vector<Review> MemoryCardStore::reviewsByCardId(long long cardId) {
  std::vector<Review> cardReviews;

  for (const auto& review : reviews) {
    if (review.cardId == cardId) {
      cardReviews.push_back(review);
    }
  }

  return cardReviews;
}

CardContents MemoryCardStore::cardContents(long long id) {
  for (int i = 0; i < cards.size(); i++) {
    auto card = cards[i];
    if (card.id == id) {
      return CardContents{.id = card.id, .question = card.question, .answer = card.answer};
    }
  }

  LOG_ERR("FLASHCARDS", "Attempted to access non-existent card with ID %lld", id);
  assert(false);  // FIXME: Implement proper fallback
}

CardParams MemoryCardStore::cardParams(long long id) {
  for (int i = 0; i < cards.size(); i++) {
    auto card = cards[i];
    if (card.id == id) {
      return CardParams{
          .id = card.id, .stability = card.stability, .difficulty = card.difficulty, .lastReview = card.lastReview};
    }
  }

  LOG_ERR("FLASHCARDS", "Attempted to access non-existent card with ID %lld", id);
  assert(false);  // FIXME: Implement proper fallback
}
