#pragma once

#include <vector>

#include "CardStore.h"

class MemoryCardStore : public CardStore {
 private:
  struct Card {
    long long id;
    long long deckId;
    float stability;
    float difficulty;
    unsigned long lastReview;
    std::string question;
    std::string answer;
  };

  struct Deck {
    long long id;
    std::string name;
  };

  std::vector<Review> reviews;
  std::vector<Card> cards;
  std::vector<Deck> decks;

  void removeReviewsByCardId(long long cardId);

 public:
  void open() override {};
  void close() override {};
  bool reviewExists(long long id) override;
  void addReview(long long id, long long cardId, Grade grade, unsigned long duration) override;
  void removeReview(long long id) override;
  bool cardExists(long long id) override;
  void addCard(long long id, long long deckId, std::string question, std::string answer) override;
  void updateCardParams(long long id, float stability, float difficulty) override;
  void removeCard(long long id) override;
  bool deckExists(long long id) override;
  void addDeck(long long id, std::string name) override;
  void removeDeck(long long id) override;
  std::vector<CardParams> cardParamsByDeckId(long long deckId) override;
  std::vector<Review> reviewsByCardId(long long cardId) override;
  CardContents cardContents(long long id) override;
  CardParams cardParams(long long id) override;
};
