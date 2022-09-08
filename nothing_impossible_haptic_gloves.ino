#include <WiFi.h>
#include <M5StickC.h>
#include <PubSubClient.h>
 
float accuracy_x = 0.0F;
float accuracy_y = 0.0F;
float accuracy_z = 0.0F;
 
WiFiClient espClient;
PubSubClient client(espClient);
 
void setup() {
    Serial.begin(115200);
    M5.begin();
    M5.IMU.Init();
    M5.Lcd.print("Nothing's Impossible");
 
    char wirelessMessage[256];
    sprintf(wirelessMessage, "%s Connecting to %s", "[M5Stack]", "iPhone7");
    Serial.println(wirelessMessage);
 
    WiFi.mode(WIFI_STA);
    WiFi.begin("iPhone7", "12345678");
 
    while (WiFi.status() != WL_CONNECTED) {
        delay(1024);
        Serial.println("[M5Stack] Let's try to connecting the wifi...");
    }
 
    client.setServer("45.130.141.210", 1883);
    client.setBufferSize(4096);
 
    while (!client.connected()) {
        Serial.println("[M5Stack] Attempting MQTT broker connection...");
 
        if (client.connect("hardware_8ad968f8", "nothing", "nothing1413")) {
        Serial.println("[M5Stack] MQTT broker has been connected.");
 
        } else {
            char failedBrokerMessage[128];
            sprintf(failedBrokerMessage, "[M5Stack] Failed to connection, RC=%d", client.state());
            Serial.println(failedBrokerMessage);
            Serial.println("[M5Stack] Will try again in 5 seconds.");
            delay(5120);
        }
    }
}
 
void loop() {
    M5.IMU.getAccelData(&accuracy_x, &accuracy_y, &accuracy_z);
 
    char payload_accuracy_axis[256];
    sprintf(payload_accuracy_axis, "{\"gyro_x\":\"%f\",\"gyro_y\":\"%f\",\"gyro_z\":\"%f\"}", accuracy_x, accuracy_y, accuracy_z);
 
    Serial.println(payload_accuracy_axis);
 
    client.publish("nothing/hardware/cartesian/axis/gloves", payload_accuracy_axis);
}