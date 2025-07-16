#include <WiFi.h>          // For connecting ESP32 to Wi-Fi
#include <esp_wifi.h>      // For using low-level Wi-Fi features like promiscuous mode
#include <WebServer.h>     // To run a basic HTTP web server on ESP32

// Wi-Fi credentials (connects ESP32 to your network)
const char* ssid = "Your_Wi-Fi_SSID";          // Your Wi-Fi SSID
const char* password = "Your_Wi-Fi_Password";       // Your Wi-Fi password

WebServer server(80);                      // Create a web server on port 80 (default HTTP port)

// This string will store rows of HTML table containing detected device info
String devices = "";


// Convert Wi-Fi packet types to readable names
String packetTypeToString(wifi_promiscuous_pkt_type_t type) {
  switch (type) {
    case WIFI_PKT_MGMT: return "Management";  // E.g., beacon, probe
    case WIFI_PKT_CTRL: return "Control";     // E.g., RTS, CTS
    case WIFI_PKT_DATA: return "Data";        // E.g., actual data packets
    default: return "Unknown";                // Any other unknown packet
  }
}

// Format MAC address from byte array to human-readable string
void formatMac(const uint8_t *mac, char *out) {
  sprintf(out, "%02X:%02X:%02X:%02X:%02X:%02X",
          mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
}

// This function is called every time the ESP32 captures a Wi-Fi packet
void snifferCallback(void* buf, wifi_promiscuous_pkt_type_t type) {
  const wifi_promiscuous_pkt_t *pkt = (wifi_promiscuous_pkt_t*) buf;

  // Safety check (make sure packet is valid)
  if (pkt == nullptr || pkt->payload == nullptr) return;

  char macStr[18];
  formatMac(pkt->payload + 10, macStr);  // Get source MAC address (offset 10 in frame)

  int rssi = pkt->rx_ctrl.rssi;                  // Received Signal Strength Indicator
  String pktType = packetTypeToString(type);     // Convert packet type to readable string

  // Append device info to the HTML table string
  devices += "<tr><td>" + String(macStr) + "</td><td>" + pktType + "</td><td>" + String(rssi) + " dBm</td></tr>\n";
}

// This function handles the main web dashboard page
void handleRoot() {
  String html = R"rawliteral(
  <!DOCTYPE html>
  <html>
  <head>
    <title>ESP32 Wi-Fi Sniffer Dashboard</title>
    <meta http-equiv="refresh" content="3"/> <!-- Auto-refresh every 3 seconds -->
    <style>
      * { margin: 0; padding: 0; box-sizing: border-box; }

      body {
        background-color: #000000;
        color: #e0e0e0;
        font-family: 'Segoe UI', sans-serif;
        line-height: 1.5;
      }

      header {
        padding: 20px;
        background: rgba(20, 20, 20, 0.7);
        text-align: center;
        border-bottom: 1px solid #333;
        backdrop-filter: blur(8px);
      }

      h2 {
        font-size: 26px;
        color: #ffffff;
      }

      .subtitle {
        font-size: 14px;
        color: #bbbbbb;
        margin-top: 5px;
      }

      .disclaimer {
        width: 90%;
        margin: 20px auto;
        padding: 12px 20px;
        background: rgba(255, 255, 255, 0.05);
        border-left: 4px solid #f39c12;
        color: #cccccc;
        border-radius: 8px;
        font-size: 13px;
        backdrop-filter: blur(10px);
      }

      table {
        width: 90%;
        margin: 30px auto;
        border-collapse: collapse;
        background-color: rgba(255, 255, 255, 0.02);
        border-radius: 10px;
        overflow: hidden;
        backdrop-filter: blur(8px);
      }

      th, td {
        padding: 12px 15px;
        text-align: left;
        border-bottom: 1px solid rgba(255,255,255,0.05);
      }

      th {
        background-color: rgba(255, 255, 255, 0.05);
        color: #f0f0f0;
        font-weight: 600;
      }

      td {
        color: #b0e0e6;
      }

      tr:hover {
        background-color: rgba(255, 255, 255, 0.06);
      }

      footer {
        text-align: center;
        padding: 16px;
        font-size: 14px;
        color: #777;
        background-color: rgba(20, 20, 20, 0.7);
        border-top: 1px solid #333;
        margin-top: 30px;
        backdrop-filter: blur(8px);
      }

      a {
        color: #79baff;
        text-decoration: none;
      }

      a:hover {
        text-decoration: underline;
      }
    </style>
  </head>
  <body>
    <header>
      <h2>ESP32 Wi-Fi Sniffer Dashboard</h2>
      <div class="subtitle">Monitoring nearby Wi-Fi signals on your network</div>
      <div class="subtitle">Built by K Rajtilak</div>
    </header>

    <div class="disclaimer">
        <strong>Disclaimer:</strong> This tool is intended for ethical, educational use only on authorized networks.
    </div>

    <table>
      <tr>
        <th>MAC Address</th>
        <th>Packet Type</th>
        <th>Signal Strength (RSSI)</th>
      </tr>
  )rawliteral";

  html += devices;  // Insert rows with detected MAC addresses and RSSI

  html += R"rawliteral(
    </table>

    <footer>
      Made with <3 by <strong>K Rajtilak</strong> |
      <a href="https://github.com/rajtilak-2020" target="_blank">GitHub: rajtilak-2020</a>
    </footer>
  </body>
  </html>
  )rawliteral";

  // Send the HTML to the client's browser
  server.send(200, "text/html", html);

  // Clear device list for next refresh (avoids infinite growth)
  devices = "";
}

void setup() {
  Serial.begin(115200);          // Start serial monitor at 115200 baud
  delay(1000);                   // Give some time before starting

  WiFi.mode(WIFI_STA);           // Set Wi-Fi to station mode (ESP32 won't broadcast its own SSID)
  WiFi.begin(ssid, password);    // Connect to the specified Wi-Fi network

  Serial.print("Connecting to WiFi");

  // Keep checking until connected
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  // Wi-Fi connected
  Serial.println("\nConnected!");
  Serial.print("ESP32 IP: ");
  Serial.println(WiFi.localIP());  // Print the IP address to access the dashboard

  // Set up the web server route
  server.on("/", handleRoot);
  server.begin();
  Serial.println("Web server started.");

  // Enable promiscuous (sniffing) mode
  esp_wifi_set_promiscuous(true);

  // Set the callback function to be called for each captured packet
  esp_wifi_set_promiscuous_rx_cb(&snifferCallback);
}

void loop() {
  // Handle incoming client requests (browser page refreshes)
  server.handleClient();
}
