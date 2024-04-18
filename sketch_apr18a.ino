#include <WiFi.h>
#include <WebServer.h>

const char* ssid = "BatiESP32";
const char* password = "bettylafeaeslamejornoveladelmundo";
WebServer server(80);

// Lista de identificadores con claves únicas
const char* allowed_ids[] = {"id1", "id2", "id3", "id4", "id5"};
const char* correct_username = "warriorwithin";
const char* correct_password = "caricachupas";

void handleRoot() {
  String html = "<h1>Bienvenido al Portal Cautivo ESP32</h1>";
  html += "<form action='/login' method='post'>";
  html += "Usuario: <input type='text' name='username'><br>";
  html += "Contraseña: <input type='password' name='password'><br>";
  html += "<input type='submit' value='Iniciar sesión'></form>";
  server.send(200, "text/html", html);
}

void handleLogin() {
  String username = server.arg("username");
  String password = server.arg("password");
  
  if (username.equals(correct_username) && password.equals(correct_password)) {
    server.send(200, "text/html", "Inicio de sesión exitoso. ¡Disfruta tu conexión!");
  } else {
    server.send(403, "text/html", "Error de inicio de sesión. Usuario o contraseña incorrectos.");
  }
}

void handleNotFound() {
  server.send(404, "text/plain", "Página no encontrada");
}

void setup() {
  Serial.begin(115200);
  delay(1000);

  // Configuración del modo de acceso del ESP32
  WiFi.softAP(ssid, password);

  IPAddress IP = WiFi.softAPIP();
  Serial.print("Dirección IP del ESP32: ");
  Serial.println(IP);

  server.on("/", handleRoot);
  server.on("/login", handleLogin);
  server.onNotFound(handleNotFound);
  server.begin();
}

void loop() {
  server.handleClient();
}



