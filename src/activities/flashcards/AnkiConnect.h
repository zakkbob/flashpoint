#pragma once

#include <WiFi.h>
#include <esp_http_client.h>

#include <string>
#include <vector>

// Interface to interact with the anki-connect plugin for Anki

template <class T>
struct Response {
  bool error = false;
  T val;

  Response(bool error) : error(error) {};
  Response(T val) : val(val) {};

  explicit operator bool() const { return !error; };
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

  template <typename T, typename R>
  Response<T> performRequest(std::string action, R handleResult);
  template <typename T, typename B, typename R>
  Response<T> performRequest(std::string action, B fillParams, R handleResult);
};
