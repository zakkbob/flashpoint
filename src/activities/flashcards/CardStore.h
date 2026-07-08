#pragma once

#include <string>
#include <vector>

#include "CardType.h"

struct Review {
  long long id;
  long long cardId;
  Grade grade;
  unsigned long duration;
};

struct CardParams {
  long long id;
  float stability;
  float difficulty;
  unsigned long lastReview;
};

struct CardContents {
  long long id;
  std::string question;
  std::string answer;
};

class CardStore {
 public:
  virtual ~CardStore() = default;

  virtual void open() = 0;
  virtual void close() = 0;
  virtual bool reviewExists(long long id) = 0;
  virtual void addReview(long long id, long long cardId, Grade grade, unsigned long duration) = 0;
  virtual void updateCardParams(long long id, float stability, float difficulty) = 0;
  virtual void removeReview(long long id) = 0;
  virtual bool cardExists(long long id) = 0;
  virtual void addCard(long long id, long long deckId, std::string question, std::string answer) = 0;
  virtual void removeCard(long long id) = 0;
  virtual bool deckExists(long long id) = 0;
  virtual void addDeck(long long id, std::string name) = 0;
  virtual void removeDeck(long long id) = 0;
  virtual std::vector<CardParams> cardParamsByDeckId(long long deckId) = 0;
  virtual std::vector<Review> reviewsByCardId(long long cardId) = 0;
  virtual CardContents cardContents(long long id) = 0;
  virtual CardParams cardParams(long long id) = 0;
};
