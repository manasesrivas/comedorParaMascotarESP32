#include <Arduino.h> // incluyendo libreria de el framework de arduino
#include <WiFiManager.h> // libreria para conectar el esp32 a diferentes redes wifi
#include <CTBot.h> // libreria para contectar el esp32 a telegram bot

// creanto objeto de diferenetes librerias
CTBot bot;
CTBotInlineKeyboard teclado;



// token de acceso que nos prevee botFather de telegram
const char* token=""; 


// Servo motor pin
const int apin=13;

// cantidad de comidad 
int32_t cantidadComida=800;


// funcion para mover el servo motor
void moveServo(int pulseWidth) {
  int pulso = (pulseWidth*10)+600;
  digitalWrite(apin, HIGH);
  delayMicroseconds(pulso);
  digitalWrite(apin, LOW); 
}



void setup(){
  // creado un boton para que el usuario administra la comida con un boton
  teclado.addButton("Alimentar", "alimentar", CTBotKeyboardButtonQuery);

  // iniciando la comunicacion serial
  Serial.begin(115200);
  //inicializando el modo del pin del servo motor como salida
  pinMode(apin, OUTPUT);

  Serial.println("conectando a una red wifi");
  WiFiManager wifimanager; // creando un objeto de WiFiManager

  wifimanager.autoConnect("comedor ESP32"); // creando un punto de acceso con el nombre de comedor ESP32 si el esp no se puede conectar a un red anterior
  
  bot.setTelegramToken(token); // enviandole el token de acceso 
  Serial.println("probando conexion");
  if(bot.testConnection()){ // provando coneccion de internet
    Serial.println("se ha conectado a una red wifi correctamente");
  }else{
    while(true){
      Serial.println("no se puedo conectar a una red wifi");
      delay(500);
    }
  }
}

void loop(){
  TBMessage msg;
  // verica si hay un nuevo mensaje
  if(bot.getNewMessage(msg)){
    if(msg.messageType==CTBotMessageText){ // verifica si el mensaje es de tipo texto
      if(msg.text.equalsIgnoreCase("/start")){ // si el mensaje es igual a /start entonces me enviara el teclado con el boton
        bot.sendMessage(msg.sender.id,"Acciones", teclado);
      }else{
        Serial.println("mensaje ->" + String(msg.text)+" id: "+String(msg.sender.id)); // sino me inprimira por el monitor lo que recibot de telegram
      }
    }
    if(msg.messageType==CTBotMessageQuery){ // verifico si el mensaje es una consulta
      if(msg.callbackQueryData.equals("alimentar")){ // si el mensaje es una consulta 
        for (int angle = 0; angle <= 90; angle++) { // habre el paso a 90 grados
          moveServo(angle);
          Serial.println();
        }
        delay(cantidadComida);

        for (int angle = 90; angle >= 0; angle--) { // cierra el paso a 0 grados
          moveServo(angle);
        }

        bot.endQuery(msg.callbackQueryID, "se ha dado de comer", true); // me manda al chat que ya realizo la accion de darle de comer a la mascota
      }
    }
  }
  moveServo(0); // cierra el paso de comida



}
