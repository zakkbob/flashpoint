#pragma once

#include <FSRS.h>
#include <sqlite3.h>

#include <map>
#include <string>
#include <vector>

struct Note {
  int id;
  std::string front;
  std::string back;
};

enum CardType : int { New = 0, Learning, Review };

struct Card {
  int id;
  int noteId;
  int due;
  int created;
  int reviews;
  int lastReview;
  CardType type;
  float stability;
  float difficulty;
  float desiredRetention;
  std::string front;
  std::string back;
};

class CardStore {
 private:
  sqlite3* DB;
  std::vector<Card> cards;
  std::map<int, Note> notes;  // indexed by id
  int i;                      // card index

  void loadNotes();
  void loadCards();
  int getLastReviewTime(int id);

 public:
  Card currentCard;

  void open();
  void close();
  void grade(FSRS::Grade g);  // Grade current card, move on to next
  void undo();                // Returns previous flashcard
};
