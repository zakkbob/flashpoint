#include "AnkiConnect.h"

#include <ArduinoJson.h>
#include <Logging.h>

#include "network/WifiPowerSaveGuard.h"

const int maxBufferLen = 1000;

bool AnkiConnect::init() {
  esp_http_client_config_t config = {
      .url = "http://192.168.0.112:8765",
  };

  // config.url = url;
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

  LOG_DBG("ANKI_CONNECT", "%s", buffer);

  esp_http_client_close(client);
  return true;
}

Response<DeckNames> AnkiConnect::deckNames() {
  JsonDocument doc;

  doc["version"] = 6;
  doc["action"] = "deckNames";

  const char* body;
  serializeJson(doc, body);

  char res[101];
  if (!get(body.c_str(), res, 100)) {
    return false;
  }

  deserializeJson(doc, res);

  JsonArray result = doc["result"].as<JsonArray>();
  DeckNames deckNames;

  for (JsonVariant v : result) {
    const char* deckName = v.as<const char*>();
    LOG_DBG("ANKI_CONNECT", "deck name: %s", deckName);
    deckNames.push_back(deckName);
  }

  return deckNames;
}
