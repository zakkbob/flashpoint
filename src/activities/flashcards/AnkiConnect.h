#pragma once

#include <WiFi.h>
#include <esp_http_client.h>

#include <string>
#include <vector>

#include "CardType.h"

// Interface to interact with the anki-connect plugin for Anki

template <class T>
struct Response {
  bool error = false;
  T val;

  Response(bool error) : error(error) {};
  Response(T val) : val(val) {};

  explicit operator bool() const { return !error; };
};

struct CardInfo {
  long long id;
  CardType type;
  std::string question;
  std::string answer;
};

struct Deck {
  int id;
  std::string name;
};

class AnkiConnect {
 private:
  const char* url;
  esp_http_client_handle_t client;
  esp_http_client_config_t config;

  bool get(const char* body, char* buffer, size_t bufferSize);

 public:
  AnkiConnect(const char* url) : url(url) {
    config = {
        .url = url,
    };
  };

  bool init();  // Initialise http client handle

  Response<std::vector<std::string>> deckNames();
  Response<std::vector<Deck>> deckNamesAndIds();
  Response<std::vector<CardInfo>> cardsByIds(std::vector<long long int> ids);
  Response<CardInfo> cardById(long long id);
  Response<std::vector<long long>> cardIdsByDeckName(std::string deckName);

  template <typename T, typename R>
  Response<T> performRequest(std::string action, R handleResult);
  template <typename T, typename B, typename R>
  Response<T> performRequest(std::string action, B fillParams, R handleResult);
};
