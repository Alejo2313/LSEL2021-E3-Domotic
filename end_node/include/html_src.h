#ifndef __HTML_SRC_H__
#define __HTML_SRC_H__

__unused static char* configHTML =
        "<!DOCTYPE html>\n"
        "<html>\n"
        "<body>\n"
        "\n"
        "<h1>Network Settings</h1>\n"
        "\n"
        "<form action=\"/config\" method=\"get\" target=\"_blank\">\n"
        "  <label for=\"ssid\">WiFi Name:</label>\n"
        "  <input type=\"text\" id=\"ssid\" name=\"ssid\"><br><br>\n"
        "  <label for=\"pass\">WiFi Password:</label>\n"
        "  <input type=\"text\" id=\"pass\" name=\"pass\"><br><br>\n"
        "  <label for=\"mqtt\">MQTT Server</label>\n"
        "  <input type=\"text\" id=\"mqtt\" name=\"mqtt\"><br><br>\n"
        "  <input type=\"submit\" value=\"Submit\">\n"
        "</form>\n"
        "\n"
        "</body>\n"
        "</html>";


#endif // !__HTML_H__
