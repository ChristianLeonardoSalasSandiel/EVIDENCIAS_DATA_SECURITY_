#include <WiFi.h>
#include <WebServer.h>
#include <TOTP.h> // Biblioteca para generar códigos TOTP

const char* ssid = "IZZI-1284-5G";
const char* password = "4P4PFaLHHm3zhmPZme";
const char* usernameCorrecto = "christian";
const char* passwordCorrecto = "Camporojo";
const char* secret = "clave_secreta"; // Clave secreta compartida para la generación de códigos TOTP

WebServer server(80);
TOTP totp((uint8_t*)secret, 10); // Objeto TOTP inicializado con la clave secreta y la longitud de la clave

void handleRoot() {
  String html = "<form action='/login' method='post'>";
  html += "Usuario: <input type='text' name='username'><br>";
  html += "Contraseña: <input type='password' name='password'><br>";
  html += "Código de autenticación: <input type='text' name='otp'><br>";
  html += "<input type='submit' value='Iniciar sesión'></form>";
  server.send(200, "text/html", html);
}

void handleLogin() {
  String username = server.arg("username");
  String password = server.arg("password");
  String otp = server.arg("otp");

  // Verificar usuario y contraseña
  if (username.equals(usernameCorrecto) && password.equals(passwordCorrecto)) {
    // Generar código de autenticación TOTP
    char codigoTOTP[7]; // El código TOTP es de 6 dígitos más el carácter nulo
    totp.getCode((long int)time(nullptr));
    // Verificar código de autenticación TOTP
    if (strcmp(otp.c_str(), codigoTOTP) == 0) {
      server.send(200, "text/plain", "Autenticación exitosa");
    } else {
      server.send(403, "text/plain", "Error de autenticación (OTP incorrecto)");
    }
  } else {
    server.send(403, "text/plain", "Error de autenticación (Usuario o contraseña incorrectos)");
  }
}

void setup() {
  Serial.begin(115200);

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Conectando a la red Wi-Fi...");
  }
  Serial.println("Conectado a la red Wi-Fi");

  // Imprimir la dirección IP del ESP32 en el monitor serial
  Serial.print("Dirección IP del ESP32: ");
  Serial.println(WiFi.localIP());

  server.on("/", handleRoot);
  server.on("/login", handleLogin);
  server.begin();
  Serial.println("Servidor HTTP iniciado");
}

void loop() {
  server.handleClient();
}


