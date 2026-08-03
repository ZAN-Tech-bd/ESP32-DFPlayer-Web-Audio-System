#include <WiFi.h>
#include <WebServer.h>
#include <DFRobotDFPlayerMini.h>

// ================================
// DFPlayer
// ================================
HardwareSerial mySerial(2);
DFRobotDFPlayerMini player;

// ESP32 pins
#define DF_RX 16
#define DF_TX 17

// ================================
// WiFi Access Point
// ================================
const char* ssid = "ESP32-MP3";
const char* password = "12345678";

WebServer server(80);

// Current volume
int currentVolume = 20;


// =====================================================
// WEB PAGE
// =====================================================
String webpage = R"rawliteral(
<!DOCTYPE html>
<html>
<head>

<meta name="viewport" content="width=device-width, initial-scale=1">

<title>ESP32 MP3 Player</title>

<style>

body {
    font-family: Arial;
    text-align: center;
    background: #101820;
    color: white;
    margin: 0;
    padding: 20px;
}

.container {
    max-width: 500px;
    margin: auto;
}

h1 {
    color: #00d4ff;
}

.card {
    background: #1b2733;
    padding: 20px;
    border-radius: 15px;
    margin-top: 20px;
}

button {
    width: 130px;
    padding: 15px;
    margin: 8px;
    font-size: 17px;
    border: none;
    border-radius: 10px;
    cursor: pointer;
}

.play {
    background: #00c853;
    color: white;
}

.control {
    background: #2979ff;
    color: white;
}

.stop {
    background: #ff1744;
    color: white;
}

.volume {
    background: #ff9800;
    color: white;
}

</style>

</head>

<body>

<div class="container">

<h1>ESP32 MP3 Player</h1>

<div class="card">

<h2>Music</h2>

<button class="play" onclick="sendCommand('/play?track=1')">
Play 0001
</button>

<button class="play" onclick="sendCommand('/play?track=2')">
Play 0002
</button>

<button class="play" onclick="sendCommand('/play?track=3')">
Play 0003
</button>

</div>


<div class="card">

<h2>Playback Control</h2>

<button class="control" onclick="sendCommand('/pause')">
Pause
</button>

<button class="control" onclick="sendCommand('/resume')">
Resume
</button>

<button class="stop" onclick="sendCommand('/stop')">
Stop
</button>

</div>


<div class="card">

<h2>Volume</h2>

<button class="volume" onclick="sendCommand('/volumedown')">
Volume -
</button>

<button class="volume" onclick="sendCommand('/volumeup')">
Volume +
</button>

</div>


<div class="card">

<h3 id="status">Ready</h3>

</div>

</div>


<script>

function sendCommand(url) {

    fetch(url)
    .then(response => response.text())
    .then(data => {

        document.getElementById("status").innerHTML = data;

    });

}

</script>

</body>
</html>
)rawliteral";


// =====================================================
// Play MP3
// =====================================================
void playTrack(int track) {

    Serial.print("Playing track: ");
    Serial.println(track);

    player.playMp3Folder(track);
}


// =====================================================
// Web Server
// =====================================================
void handleRoot() {

    server.send(200, "text/html", webpage);

}


void handlePlay() {

    if (server.hasArg("track")) {

        int track = server.arg("track").toInt();

        playTrack(track);

        server.send(
            200,
            "text/plain",
            "Playing track " + String(track)
        );

    }
}


void handlePause() {

    player.pause();

    Serial.println("Audio Paused");

    server.send(
        200,
        "text/plain",
        "Audio Paused"
    );
}


void handleResume() {

    player.start();

    Serial.println("Audio Resumed");

    server.send(
        200,
        "text/plain",
        "Audio Resumed"
    );
}


void handleStop() {

    player.stop();

    Serial.println("Audio Stopped");

    server.send(
        200,
        "text/plain",
        "Audio Stopped"
    );
}


void handleVolumeUp() {

    if (currentVolume < 30)
        currentVolume++;

    player.volume(currentVolume);

    Serial.print("Volume: ");
    Serial.println(currentVolume);

    server.send(
        200,
        "text/plain",
        "Volume: " + String(currentVolume)
    );
}


void handleVolumeDown() {

    if (currentVolume > 0)
        currentVolume--;

    player.volume(currentVolume);

    Serial.print("Volume: ");
    Serial.println(currentVolume);

    server.send(
        200,
        "text/plain",
        "Volume: " + String(currentVolume)
    );
}


// =====================================================
// SETUP
// =====================================================
void setup() {

    Serial.begin(115200);

    delay(1000);

    Serial.println();
    Serial.println("==============================");
    Serial.println("ESP32 MP3 Player Starting...");
    Serial.println("==============================");


    // --------------------------------
    // DFPlayer
    // --------------------------------

    mySerial.begin(
        9600,
        SERIAL_8N1,
        DF_RX,
        DF_TX
    );

    Serial.println("Connecting DFPlayer...");

    if (!player.begin(mySerial)) {

        Serial.println("DFPlayer ERROR!");
        Serial.println("Check wiring / SD card.");

        while (true) {
            delay(1000);
        }
    }

    Serial.println("DFPlayer Connected!");

    player.volume(currentVolume);

    delay(500);


    // --------------------------------
    // ESP32 WiFi Hotspot
    // --------------------------------

    Serial.println();
    Serial.println("Starting WiFi Hotspot...");

    WiFi.softAP(
        ssid,
        password
    );

    IPAddress IP = WiFi.softAPIP();

    Serial.println();
    Serial.println("==============================");
    Serial.println("WiFi Ready!");
    Serial.print("WiFi Name: ");
    Serial.println(ssid);

    Serial.print("Password: ");
    Serial.println(password);

    Serial.print("Open Browser: http://");
    Serial.println(IP);

    Serial.println("==============================");


    // --------------------------------
    // Web routes
    // --------------------------------

    server.on("/", handleRoot);

    server.on("/play", handlePlay);

    server.on("/pause", handlePause);

    server.on("/resume", handleResume);

    server.on("/stop", handleStop);

    server.on("/volumeup", handleVolumeUp);

    server.on("/volumedown", handleVolumeDown);

    server.begin();

    Serial.println("Web Server Started!");

    Serial.println();
    Serial.println("Serial Commands:");
    Serial.println("1 = Play 0001.mp3");
    Serial.println("2 = Play 0002.mp3");
    Serial.println("3 = Play 0003.mp3");
    Serial.println("p = Pause");
    Serial.println("r = Resume");
    Serial.println("s = Stop");
    Serial.println("+ = Volume Up");
    Serial.println("- = Volume Down");
    Serial.println();
}


// =====================================================
// LOOP
// =====================================================
void loop() {

    // Web Server
    server.handleClient();


    // ================================
    // Serial Monitor Control
    // ================================

    if (Serial.available()) {

        char command = Serial.read();

        if (command == '1') {

            playTrack(1);

        }

        else if (command == '2') {

            playTrack(2);

        }

        else if (command == '3') {

            playTrack(3);

        }

        else if (command == 'p') {

            player.pause();

            Serial.println("Paused");

        }

        else if (command == 'r') {

            player.start();

            Serial.println("Resumed");

        }

        else if (command == 's') {

            player.stop();

            Serial.println("Stopped");

        }

        else if (command == '+') {

            if (currentVolume < 30)
                currentVolume++;

            player.volume(currentVolume);

            Serial.print("Volume: ");
            Serial.println(currentVolume);

        }

        else if (command == '-') {

            if (currentVolume > 0)
                currentVolume--;

            player.volume(currentVolume);

            Serial.print("Volume: ");
            Serial.println(currentVolume);

        }
    }
}
