#include "AnkiConnect.h"

#include <ArduinoJson.h>
#include <Logging.h>

#include "network/WifiPowerSaveGuard.h"

const int maxBufferLen = 1000;

bool AnkiConnect::init() {
  client = esp_http_client_init(&config);

  LOG_DBG("ANKI_CONNECT", "Using url '%s'", url);

  if (!client) {
    LOG_ERR("ANKI_CONNECT", "Failed to initialise http client");
    return false;
  }

  LOG_DBG("ANKI_CONNECT", "Successfully Initialised http client");
  return true;
}

bool AnkiConnect::get(const char* body, char* buffer, size_t bufferSize) {
  WifiPowerSaveGuard wifiPowerSaveGuard;
  (void)wifiPowerSaveGuard;

  esp_http_client_set_method(client, HTTP_METHOD_GET);
  esp_err_t err = esp_http_client_open(client, strlen(body));
  if (err != ESP_OK) {
    LOG_DBG("ANKI_CONNECT", "Failed to open HTTP connection: %s", esp_err_to_name(err));
    esp_http_client_close(client);
    return false;
  }

  int len = esp_http_client_write(client, body, strlen(body));
  if (len == -1) {
    LOG_ERR("ANKI_CONNECT", "Failed to write request body '%s'", body);
    esp_http_client_close(client);
    return false;
  }

  int contentLength = esp_http_client_fetch_headers(client);
  if (contentLength < 0) {
    LOG_DBG("ANKI_CONNECT", "Failed to fetch http headers");
    esp_http_client_close(client);
    return false;
  }

  int dataRead = esp_http_client_read_response(client, buffer, bufferSize);
  if (dataRead < 0) {
    LOG_ERR("ANKI_CONNECT", "Failed to read response");
    esp_http_client_close(client);
    return false;
  }

  LOG_DBG("ANKI_CONNECT", "HTTP request Status = %d, content_length = %d", esp_http_client_get_status_code(client),
          esp_http_client_get_content_length(client));

  esp_http_client_close(client);
  return true;
}

template <typename T, typename R>
Response<T> AnkiConnect::performRequest(std::string action, R handleResult) {
  return performRequest<T>(action, [](JsonVariant) {}, handleResult);
}

template <typename T, typename B, typename R>
Response<T> AnkiConnect::performRequest(std::string action, B fillParams, R handleResult) {
  JsonDocument doc;

  doc["version"] = 6;
  doc["action"] = action;
  fillParams(doc["params"].to<JsonObject>());

  std::string body;
  serializeJson(doc, body);

  size_t bufferSize = 1500;
  char res[bufferSize + 1];

  if (!get(body.c_str(), res, bufferSize)) {
    return false;
  }

  auto err = deserializeJson(doc, res);
  if (err != DeserializationError::Ok) {
    LOG_ERR("ANKI_CONNECT", "Failed to deserialise JSON: %d", err);
    return false;
  }

  if (doc["error"]) {
    LOG_DBG("ANKI_CONNECT", "API response contained error");
    return false;
  }

  return handleResult(doc["result"]);
}

Response<std::vector<std::string>> AnkiConnect::deckNames() {
  return performRequest<std::vector<std::string>>("deckNames", [](JsonVariant result) {
    std::vector<std::string> deckNames;

    for (JsonVariant v : result.as<JsonArray>()) {
      const char* deckName = v.as<const char*>();
      deckNames.push_back(deckName);
    }

    return deckNames;
  });
}

Response<std::vector<Deck>> AnkiConnect::deckNamesAndIds() {
  return performRequest<std::vector<Deck>>("deckNamesAndIds", [](JsonVariant result) {
    std::vector<Deck> decks;

    for (JsonPair kv : result.as<JsonObject>()) {
      decks.push_back(Deck{.id = kv.value().as<int>(), .name = kv.key().c_str()});
    }

    return decks;
  });
}

Response<std::vector<CardInfo>> AnkiConnect::cardsByIds(std::vector<long long int> ids) {
  return performRequest<std::vector<CardInfo>>(
      "cardsInfo",
      [ids](JsonVariant params) {
        auto cards = params["cards"].to<JsonArray>();

        for (auto id : ids) {
          cards.add(id);
        }
      },
      [](JsonVariant result) {
        std::vector<CardInfo> cards;

        for (JsonObject o : result.as<JsonArray>()) {
          cards.push_back(CardInfo{.id = o["cardId"],
                                   .type = o["type"],
                                   .question = o["fields"]["Front"]["value"],
                                   .answer = o["fields"]["Back"]["value"]});
        }

        return cards;
      });
}

Response<std::vector<long long>> AnkiConnect::cardIdsByDeckName(std::string deckName) {
  return performRequest<std::vector<long long>>(
      "findCards",
      [deckName](JsonVariant params) {
        std::string query = "deck:" + deckName;
        params["query"] = query;
      },
      [](JsonVariant result) {
        std::vector<long long> ids;

        for (JsonVariant v : result.as<JsonArray>()) {
          ids.push_back(v.as<long long>());
        }

        return ids;
      });
}
