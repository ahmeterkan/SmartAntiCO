//***********************************************
//  MQ7 GAZ SENSÖRÜ VE LCD EKRAN
#include <LiquidCrystal.h>
LiquidCrystal lcd(9, 8, 6, 5, 4, 3);
int sensorPin = A0;
int sensorValue = 0;
int subValue = 0;
int divValue = 0;
int j = 0;
int buzzerPin = 2;
//**********************************************


//*****************************************************
// GPS SENSÖRÜ
#include <String.h>
char gelen[256];
char enlem[10];
char boylam[15];
char karakter;
int index = 0;
int i = 0;
bool nmea = false;
//*****************************************************


// *******************************************
//ETHERNET SHİELD
#include <SPI.h>
#include <Ethernet.h>
byte mac[] = {
  0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED
};
IPAddress ip(10, 138, 32, 140);
EthernetServer server(80);

// *******************************************

//////////////////////////////////////////////////////////////////////////////////////////SETUP
void setup() {
  //**********************************************
  //  MQ7 GAZ SENSÖRÜ VE LCD EKRAN
  Serial.begin(9600);
  pinMode(buzzerPin, OUTPUT);
  lcd.begin(16, 2);
  lcd.print("Uygun Oksijen  ");
  lcd.setCursor(0, 1);
  lcd.print("Seviyesi");
  //**********************************************

  //*****************************************************
  // GPS SENSÖRÜ
  Serial.begin(115200);
  Serial2.begin(115200);
  delay(500);
  //*****************************************************

  // *******************************************
  //ETHERNET SHİELD
  Serial.begin(9600);
  Ethernet.begin(mac, ip);
  server.begin();
  Serial.print("server is at ");
  Serial.println(Ethernet.localIP());
  // *******************************************
  // *******************************************
  //GSM SHİELD
  Serial.begin(19200);
  Serial1.begin(19200);
  delay(500);
  //********************************************
}
////////////////////////////////////////////////////////////////////////////////////////////
void loop() {
  mq7_1();
  konum();
  baglan(divValue);

}
////////////////////////////////////////////////////////////////////////////////////////////

//**********************************************
//  MQ7 GAZ SENSÖRÜ VE LCD EKRAN FONKSİYONU
void mq7_1() {

  sensorValue = analogRead(sensorPin);
  subValue = subValue + sensorValue;
  if (j == 10)
  {
    j = 0;
    divValue = subValue / 10;
    Serial.println(divValue);
    delay(1000);
    subValue = 0;

    if (divValue > 250 && divValue < 500)
    {
      lcd.begin(16, 2);
      lcd.print("LUTFEN ODAYI ");
      lcd.setCursor(0, 1);
      lcd.print("HAVALANDIRIN");
      digitalWrite(buzzerPin, HIGH);
      delay(100);
      digitalWrite(buzzerPin, LOW);
      delay(10);
    }
    else if (divValue > 1000) {
      SendTextMessage();
    }
  }
  else {
    lcd.begin(16, 2);
    lcd.print("Uygun Oksijen  ");
    lcd.setCursor(0, 1);
    lcd.print("Seviyesi");
  }
  j++;
}
//**********************************************


//*****************************************************
// GPS SENSÖRÜ FONKSİYONU
void kesme() {
  if (gelen[i + 5] == 'C' )
  {
    Serial.write("Global saat:");
    Serial.print(gelen[i + 7]);
    Serial.print(gelen[i + 8]);
    Serial.print(gelen[i + 9]);
    Serial.print(gelen[i + 10]);
    Serial.print(gelen[i + 11]);
    Serial.print(gelen[i + 12]);
    Serial.print(gelen[i + 13]);
    Serial.print(gelen[i + 14]);
    Serial.println(gelen[i + 15]);


    Serial.print("Uydu Baglantisi:");
    Serial.println(gelen[i + 18]);
    Serial.print("Enlem Bilgisi:");
    Serial.print(gelen[i + 20]);
    enlem[0] = gelen[i + 20];
    Serial.print(gelen[i + 21]);
    enlem[1] = gelen[i + 21];
    Serial.print(".");
    enlem[2] = ".";
    Serial.print(gelen[i + 22]);
    enlem[3] = gelen[i + 22];
    Serial.print(gelen[i + 23]);
    enlem[4] = gelen[i + 23];
    Serial.print(gelen[i + 25]);
    enlem[4] = gelen[i + 25];
    Serial.print(gelen[i + 26]);
    enlem[5] = gelen[i + 26];
    Serial.print(gelen[i + 27]);
    enlem[6] = gelen[i + 27];
    Serial.print(gelen[i + 28]);
    enlem[7] = gelen[i + 28];
    Serial.print(",");
    enlem[8] = ",";
    Serial.println(gelen[i + 30]);
    enlem[9] = gelen[i + 30];


    Serial.print("Boylam Bilgisi:");
    Serial.print(gelen[i + 32]);
    boylam[0] = gelen[i + 32];
    Serial.print(gelen[i + 33]);
    boylam[1] = gelen[i + 33];
    Serial.print(gelen[i + 34]);
    boylam[2] = gelen[i + 34];
    Serial.print(".");
    boylam[3] = ".";
    Serial.print(gelen[i + 35]);
    boylam[4] = gelen[i + 35];
    Serial.print(gelen[i + 36]);
    boylam[5] = gelen[i + 36];
    Serial.print(gelen[i + 38]);
    boylam[6] = gelen[i + 38];
    Serial.print(gelen[i + 39]);
    boylam[7] = gelen[i + 39];
    Serial.print(gelen[i + 40]);
    boylam[8] = gelen[i + 40];
    Serial.print(gelen[i + 41]);
    boylam[9] = gelen[i + 41];
    Serial.print(",");
    boylam[10] = ",";
    Serial.println(gelen[i + 43]);
    boylam[11] = gelen[i + 43];

    Serial.println("");
    Serial.println("");

  }
}
void konum() {
  if (Serial2.available()) {
    karakter = (Serial2.read());
    gelen[index] = karakter;
    if (index > 5) {
      if (gelen[index - 1] == 'M' && gelen[index] == 'C' )
      {
        i = index - 5;
        nmea = true;

      }
      if (index == i + 70 && nmea)
      {
        kesme();
        nmea = false;
        delay(500);
      }
    }
    index++;
    if (index == 256)
    {
      index = 0;
    }
  }
}
//*****************************************************

// *******************************************
//ETHERNET SHİELD
void baglan(int gasValue) {
  EthernetClient client = server.available();
  if (client) {
    boolean currentLineIsBlank = true;
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        if (c == '\n' && currentLineIsBlank) {
          client.println("HTTP/1.1 200 OK");
          client.println("Content-Type: text/html");
          client.println("Connection: close");
          client.println("Refresh: 5");
          client.println();
          client.println("<!DOCTYPE HTML>");
          client.println("<html>");
          client.println("<head>");
          client.println("<title>Smart Anti CO</title>");
          client.println("<meta charset=\"utf-8\">");
          client.println("<meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
          client.println("<link rel=\"stylesheet\" href=\"http://maxcdn.bootstrapcdn.com/bootstrap/3.3.6/css/bootstrap.min.css\">");
          client.println("<script src=\"https://ajax.googleapis.com/ajax/libs/jquery/1.12.0/jquery.min.js\"></script>");
          client.println("<script src=\"http://maxcdn.bootstrapcdn.com/bootstrap/3.3.6/js/bootstrap.min.js\"></script>");
          client.println("</head>");
          client.println("<body style=\"margin: auto; background:paleturquoise;\">");
          client.println("<div class=\"row\" style=\"background:turquoise; margin: auto;\">");
          client.println("<div class=\"col-xs-2 col-sm-2 col-md-2 \"> </div>");
          client.println("<div class=\"col-xs-8 col-sm-8 col-md-8\">");
          client.println("<h1>Smart Anti CO</h1> </div>");
          client.println("<div class=\"col-xs-2 col-sm-2 col-md-2\"> </div>  </div>");
          client.println("<div class=\"row\" style=\"margin: auto; background:paleturquoise;\"><br>");
          client.println("</div>");
          client.println("<div class=\"row\" style=\"margin: auto; background:paleturquoise;\">");
          client.println("<div class=\"col-xs-2 col-sm-2 col-md-2 \"> </div>");
          client.println("<div class=\"col-xs-8 col-sm-8 col-md-8 \" margin=\"auto\">");
          client.println("<div class=\"row\">");
          client.println("<div class=\"col-xs-6 col-sm-6 col-md-6 \">Ortamdaki Gaz Miktarı = </div>");
          client.println("<div class=\"col-xs-6 col-sm-6 col-md-6 \">");
          client.print(gasValue);
          client.println("</div> </div>");
          client.println("<div class=\"row\">");
          client.println("<div class=\"col-xs-6 col-sm-6 col-md-6 \">Enlem = </div>");
          client.println("<div class=\"col-xs-6 col-sm-6 col-md-6 \">");
          client.print(enlem);
          client.println("</div> </div>");
          client.println("<div class=\"row\">");
          client.println("<div class=\"col-xs-6 col-sm-6 col-md-6 \">Boylam = </div>");
          client.print("<div class=\"col-xs-6 col-sm-6 col-md-6 \">");
          client.print(boylam);
          client.println("</div></div></div>");
          client.print("<div class=\"col-xs-2 col-sm-2 col-md-2\"> </div>");
          client.println("</div>");
          client.println("</body> </html>");
          break;
        }
        if (c == '\n') {
          currentLineIsBlank = true;
        } else if (c != '\r') {
          currentLineIsBlank = false;
        }
      }
    }
    delay(1);
    client.stop();
  }
}
// *******************************************
//GSM SHİELD
void SendTextMessage()
{
  Serial.println("Sending Text...");
  Serial1.print("AT+CMGF=1\r");
  delay(100);
  Serial1.println("AT+CMGS=\"+905xxxxxxxxx\""); // TELEFON NUMARASINI BURAYA GİRECEGİZ
  delay(100);
  Serial1.println("Asagıdaki konumda gaz kacagı vardır."); //the content of the message
  delay(100);
  Serial1.print("Enlem:"); //the content of the message
  delay(100);
  Serial1.println(enlem); //the content of the message
  delay(100);
  Serial1.print("Boylam :"); //the content of the message
  delay(100);
  Serial1.println(boylam); //the content of the message
  delay(100);
  Serial1.print((char)26);//the ASCII code of the ctrl+z is 26 (required according to the datasheet)
  delay(100);
  Serial1.println();
  Serial.println("Text Sent.");
}
