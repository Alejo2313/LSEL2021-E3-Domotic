#include "server.h"

char wifi_ssid[32];
char wifi_password[64];
char mqtt_broker_url[64];

static const char *TAG = "Server";

uint32_t flags;
httpd_handle_t server = NULL;


static esp_err_t start_get_handler(httpd_req_t *req);
static esp_err_t config_get_handler(httpd_req_t *req);


static const httpd_uri_t configure =
{
    .uri    = "/",
    .method = HTTP_GET,
    .handler = start_get_handler,
};

static const httpd_uri_t configureP =
{
    .uri    = "/config",
    .method = HTTP_GET,
    .handler = config_get_handler,
};



static esp_err_t start_get_handler(httpd_req_t *req)
{
    ESP_LOGI(TAG, "GET");
    httpd_resp_send(req, configHTML, strlen(configHTML));
    return ESP_OK;
}

static esp_err_t config_get_handler(httpd_req_t *req)
{
    char*  buf;
    size_t buf_len;

    buf_len = httpd_req_get_url_query_len(req) + 1;

    if (buf_len > 1) {
        buf = malloc(buf_len);
        if (httpd_req_get_url_query_str(req, buf, buf_len) == ESP_OK) {
            ESP_LOGI(TAG, "Found URL query => %s", buf);
            char param[32];
            /* Get value of expected key from query string */
            if (httpd_query_key_value(buf, "ssid", param, sizeof(param)) == ESP_OK) {
                if(strlen(param) > 0)
                {
                    ESP_LOGI(TAG, "Found URL query parameter => ssid=%s", param);
                    strcpy(wifi_ssid, param);
                }
            }
            if (httpd_query_key_value(buf, "pass", param, sizeof(param)) == ESP_OK) {
                if(strlen(param) > 8)
                {
                    ESP_LOGI(TAG, "Found URL query parameter => pass=%s", param);
                    strcpy(wifi_password, param);
                }

            }   
            if (httpd_query_key_value(buf, "mqtt", param, sizeof(param)) == ESP_OK) {
                if(strlen(param) > 0)
                {
                    ESP_LOGI(TAG, "Found URL query parameter => mqtt=%s", param);
                    strcpy(mqtt_broker_url, param);
                }
            }
        }
        free(buf);
        CLEAR_FLAGS(&flags,FLAGS_ALL);
        save_data();
        stop_webserver();

        
    }

    httpd_resp_send(req, configHTML, strlen(configHTML));

    return ESP_OK;
}


void start_webserver(void)
{
    
    mqtt_stop();
    httpd_config_t config = HTTPD_DEFAULT_CONFIG();
    wifi_init_softap();
    // Start the httpd server
    ESP_LOGI(TAG, "Starting server on port: '%d'", config.server_port);
    if (httpd_start(&server, &config) == ESP_OK) {
        // Set URI handlers
        ESP_LOGI(TAG, "Registering URI handlers");
        httpd_register_uri_handler(server, &configure);
        httpd_register_uri_handler(server, &configureP);

        return;
    }

    ESP_LOGI(TAG, "Error starting server!");
    return;
}

void stop_webserver()
{
    // Stop the httpd server
    httpd_stop(server);
}


