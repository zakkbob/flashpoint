#include "CardStore.h"

#include <ArduinoJson.h>
#include <HalStorage.h>
#include <Logging.h>
#include <SD.h>
#include <SPI.h>
#include <sqlite3.h>

#include <cstdio>

constexpr char COLLECTION_FILE[] = "/sd/.flashpoint/collection.sqlite";

void CardStore::open() {
  bool ok = SD.begin(12);
  LOG_INF("FLASHCARDS", "ok=%d", ok);

  /*
  LOG_INF("FLASHCARDS", "exists=%d", Storage.exists(COLLECTION_FILE));

  FILE* fp = std::fopen(COLLECTION_FILE, "rb");
  LOG_INF("FLASHCARDS", "exists2 %p, %d", fp, errno);

  if (fp) {
    fclose(fp);
  }
  */

  if (!sqlite3_initialize()) {
    LOG_ERR("FLASHCARDS", "Failed to initialize");
  }

  const int code = sqlite3_open(COLLECTION_FILE, &DB);
  if (code != SQLITE_OK) {
    LOG_ERR("FLASHCARDS", "Failed to create sqlite connection - %d", code);
    return;
  }

  LOG_INF("FLASHCARDS", "Sqlite suces", code);

  loadNotes();
  loadCards();

  i = 0;
  currentCard = cards[i];
}

void CardStore::loadNotes() {
  sqlite3_stmt* stmt;
  const int code = sqlite3_prepare_v2(DB, "SELECT id, flds FROM notes;", -1, &stmt, NULL);
  if (code != SQLITE_OK) {
    LOG_ERR("FLASHCARDS", "SQLITE ERROR, notes - %d", code);
    return;
  }

  int i;
  const char delim = 0x1f;  // front-back delimeter in flds column
  Note note;
  std::string flds;

  sqlite3_step(stmt);
  while (code == SQLITE_ROW) {
    note.id = sqlite3_column_int(stmt, 0);
    const char* fldsC = (char*)sqlite3_column_text(stmt, 1);
    flds = std::string(fldsC);

    i = flds.find(delim);
    note.front = flds.substr(0, i);
    note.back = flds.substr(i);

    notes[note.id] = note;

    sqlite3_step(stmt);
  }

  if (code != SQLITE_DONE) {
    LOG_ERR("FLASHCARDS", "SQLITE ERROR 2");
  }
}

void CardStore::loadCards() {
  sqlite3_stmt* stmt;
  const int code = sqlite3_prepare_v2(DB, "SELECT id, nid, mod, reps, data, type, due FROM cards;", -1, &stmt, NULL);
  if (code != SQLITE_OK) {
    LOG_ERR("FLASHCARDS", "SQLITE ERROR - %d", code);
    return;
  }

  Card card;
  int type;
  int due;
  std::string data;
  JsonDocument doc;

  sqlite3_step(stmt);
  while (code == SQLITE_ROW) {
    card.id = sqlite3_column_int(stmt, 0);
    card.noteId = sqlite3_column_int(stmt, 1);
    card.created = sqlite3_column_int(stmt, 2);
    card.reviews = sqlite3_column_int(stmt, 3);

    const char* dataC = (char*)sqlite3_column_text(stmt, 4);
    data = std::string(dataC);
    deserializeJson(doc, data);
    card.stability = doc["s"];
    card.difficulty = doc["d"];
    card.desiredRetention = doc["dr"];

    type = sqlite3_column_int(stmt, 5);
    switch (type) {
      case 0:
        card.type = New;
        break;
      case 1:
        card.type = Learning;
        break;
      case 2:
        card.type = Review;
        break;
      default:
        LOG_ERR("FLASHCARDS", "Unknown Card Type - %d", type);
    }

    if (card.type != New) {
      card.lastReview = getLastReviewTime(card.id);
    }

    if (card.type == Learning) {
      card.due = sqlite3_column_int(stmt, 6);
    } else {
      // TODO: calculate
    }

    card.front = notes[card.noteId].front;
    card.back = notes[card.noteId].back;

    sqlite3_step(stmt);
  }
}

int CardStore::getLastReviewTime(int id) {
  sqlite3_stmt* stmt;
  const int code =
      sqlite3_prepare_v2(DB, "SELECT id FROM revlog WHERE cid = $ ORDER BY id DESC LIMIT 1", -1, &stmt, NULL);

  if (code != SQLITE_OK) {
    LOG_ERR("FLASHCARDS", "SQLITE ERROR");
    return -1;
  }

  sqlite3_bind_int(stmt, 0, id);
  sqlite3_step(stmt);
  int time = sqlite3_column_int(stmt, 0);
  return time / 1000;  // id is a unix timestamp in milliseconds of when the review happened
}

void CardStore::grade(FSRS::Grade g) {
  // TODO: implement grading

  i = (i + 1) % cards.size();
  currentCard = cards[i];
}

void CardStore::close() { sqlite3_close(DB); }
