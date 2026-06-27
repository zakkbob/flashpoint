#pragma once

// Interface to interact with the anki-connect plugin for Anki

template <class T>
struct Response {
  bool error = false;
  T val;

  Response(bool error) : error(error) {};
  Response(T val) : val(val) {};

  explicit operator bool() const { return !error };
};

using DeckNames = const std::vector<std::string>

    class AnkiConnect {
 private:
  const char* url;
  esp_http_client_handle_t client;
  esp_http_client_config_t config;

  bool get(const char* body, char* buffer);

 public:
  AnkiConnect(const char* url) : url(url) {};

  bool init()  // Initialise http client handle

      Response<DeckNames> deckNames();
};

/*
void FlashcardsReviewActivity::onWifiSelectionComplete(const bool success) {
  const char* TAG = "HTTP";
  const int MAX_HTTP_OUTPUT_BUFFER = 100;

  // Declare local_response_buffer with size (MAX_HTTP_OUTPUT_BUFFER + 1) to prevent out of bound access when
  // it is used by functions like strlen(). The buffer should only be used upto size MAX_HTTP_OUTPUT_BUFFER
  char output_buffer[MAX_HTTP_OUTPUT_BUFFER + 1] = {0};  // Buffer to store response of http request
  int content_length = 0;
  esp_http_client_config_t config = {
      .url = "http://192.168.0.112:8765",
  };
  LOG_DBG(TAG, "HTTP native request =>");
  esp_http_client_handle_t client = esp_http_client_init(&config);

  if (!client) {
    LOG_ERR(TAG, "Client failed to initialise");
  }

  WifiPowerSaveGuard wifiPowerSaveGuard;
  (void)wifiPowerSaveGuard;

  const char *contents = "{\"action\": \"deckNames\", \"version\": 6}";

  // GET Request
  esp_http_client_set_method(client, HTTP_METHOD_GET);
  esp_err_t err = esp_http_client_open(client, strlen(contents));
  if (err != ESP_OK) {
    LOG_DBG(TAG, "Failed to open HTTP connection: %s", esp_err_to_name(err));
  } else {
  int len = esp_http_client_write(client, contents, strlen(contents));
    if (len == -1) {
      LOG_ERR(TAG, "Failed to write contents");
    }
    content_length = esp_http_client_fetch_headers(client);
    if (content_length < 0) {
      LOG_DBG(TAG, "HTTP client fetch headers failed");
    } else {
      int data_read = esp_http_client_read_response(client, output_buffer, MAX_HTTP_OUTPUT_BUFFER);
      if (data_read >= 0) {
        LOG_DBG(TAG, "HTTP GET Status = %d, content_length = %d", esp_http_client_get_status_code(client),
                esp_http_client_get_content_length(client));
        LOG_DBG(TAG, "%s", (const char *)output_buffer);
      } else {
        LOG_DBG(TAG, "Failed to read response");
      }
    }
  }
  esp_http_client_close(client);
}
*/
