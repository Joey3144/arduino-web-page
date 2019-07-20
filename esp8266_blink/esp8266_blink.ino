/*********
  Rui Santos
  Complete project details at https://randomnerdtutorials.com  
*********/

// Load Wi-Fi library
#include <ESP8266WiFi.h>
#include <Servo.h>

Servo myservo;
Servo myservo2;
Servo myservo3;
Servo myservo4;
// Replace with your network credentials
const char* ssid     = "yuting";//小羊之家 iphone
const char* password = "shung1210";//09819088633
int pos = 0;
int pos2 = 0;
int pos3 = 0;
int n = 90;


// Set web server port number to 80
WiFiServer server(80);

// Variable to store the HTTP request
String header;

// Auxiliar variables to store the current output state
String output5State = "未啟動";
String output4State = "關閉";
String output2State = "關閉";
String output0State = "關閉";

// Assign output variables to GPIO pins
const int output5 = 5;
const int output4 = 4;
const int output2 = 2;
const int output0 = 0;



void setup() {
  Serial.begin(115200);
  // Initialize the output variables as outputs
  myservo.attach(D0);  // attaches the servo on pin 9 to the servo object
  myservo2.attach(D3);
  myservo3.attach(D16); //yes: 0 (2) 3 4 5 16 no: 1
  myservo4.attach(D5);
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(output5, OUTPUT);
  pinMode(output4, OUTPUT);
  pinMode(output2, OUTPUT);
  pinMode(output0, OUTPUT);

  // Set outputs to LOW
  //(有改)
  digitalWrite(output5, LOW);
  digitalWrite(output4, LOW);
  digitalWrite(output2, LOW);
  digitalWrite(output0, LOW);
  // Connect to Wi-Fi network with SSID and password
  Serial.print("正在連接至 ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while(WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  // Print local IP address and start web server
  Serial.println("");
  Serial.println("WiFi已成功連接");
  Serial.print("IP地址為: ");
  Serial.println(WiFi.localIP());
  server.begin();
}

void loop(){
  
  WiFiClient client = server.available();   // Listen for incoming clients
  
  if (client) {                             // If a new client connects,
    Serial.println("New Client.");          // print a message out in the serial port
    String currentLine = "";                // make a String to hold incoming data from the client
    while (client.connected()) {            // loop while the client's connected
      if (client.available()) {             // if there's bytes to read from the client,
        char c = client.read();             // read a byte, then
       // Serial.write("I am C:"+c);                    // print it out the serial monitor
        header += c;
        if (c == '\n') {                    // if the byte is a newline character
          // if the current line is blank, you got two newline characters in a row.
          // that's the end of the client HTTP request, so send a response:
          if (currentLine.length() == 0) {
            // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
            // and a content-type so the client knows what's coming, then a blank line:
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println("Connection: close");
            client.println();
            myservo.write(n);
            // turns the GPIOs on and off
            //if (header.indexOf("GET /5/right") >= 0) {
            //test++;
            //Serial.println(test);
            if((header.indexOf("GET /5/right") >= 0)){   
              Serial.println("GPIO 5 右轉");
              
              for (pos = n; pos >= 30; pos -= 1) { // goes from 180 degrees to 0 degrees
                myservo.write(pos);//pos+10
                n=pos;
                //myservo2.write(210-pos);// tell servo to go to position in variable 'pos'
                delay(15);                       // waits 15ms for the servo to reach the position
              }
              digitalWrite(LED_BUILTIN, HIGH);
            } 
            else if ((header.indexOf("GET /5/left") >= 0)){ //&&(ifhold==true)  //&&(ifhold==true)
              Serial.println("GPIO 5 左轉");
              
              for (pos = n; pos <= 150; pos += 1) { // goes from 0 degrees to 180 degrees
                // in steps of 1 degree
                myservo.write(pos);
                n=pos;
                //myservo2.write(210-pos);// tell servo to go to position in variable 'pos'
                delay(15);                       // waits 15ms for the servo to reach the position
              }
              digitalWrite(LED_BUILTIN, LOW);
            }  
            else if (header.indexOf("GET /4/off") >= 0) {
              Serial.println("GPIO 4 關閉");
              output4State = "關閉";
              for (pos2 = 70; pos2 >= 0; pos2 -= 1) { // goes from 180 degrees to 0 degrees
                myservo3.write(pos2);
                //myservo2.write(210-pos);// tell servo to go to position in variable 'pos'
                delay(15);                       // waits 15ms for the servo to reach the position
              }
              
              digitalWrite(LED_BUILTIN, HIGH);
            } else if (header.indexOf("GET /4/on") >= 0) {
              Serial.println("GPIO 4 開啟");
              output4State = "開啟";
              for (pos2 = 0; pos2 <= 70; pos2 += 1) { // goes from 0 degrees to 180 degrees
                // in steps of 1 degree
                myservo3.write(pos2);
                //myservo2.write(210-pos);// tell servo to go to position in variable 'pos'
                delay(15);                       // waits 15ms for the servo to reach the position
              }
              
              digitalWrite(LED_BUILTIN, LOW);
            }else if (header.indexOf("GET /2/off") >= 0) {
              Serial.println("GPIO 2 關閉");
              output2State = "關閉";
              for (pos3 = 70; pos3 >= 0; pos3 -= 1) { // goes from 180 degrees to 0 degrees
                myservo4.write(pos3);
                //myservo2.write(210-pos);// tell servo to go to position in variable 'pos'
                delay(15);                       // waits 15ms for the servo to reach the position
              }
              
              digitalWrite(LED_BUILTIN, HIGH);
            } else if (header.indexOf("GET /2/on") >= 0) {
              Serial.println("GPIO 2 開啟");
              output2State = "開啟";
              for (pos3 = 0; pos3 <= 70; pos3 += 1) { // goes from 0 degrees to 180 degrees
                // in steps of 1 degree
                myservo4.write(pos3);
                //myservo2.write(210-pos);// tell servo to go to position in variable 'pos'
                delay(15);                       // waits 15ms for the servo to reach the position
              }
              
              digitalWrite(LED_BUILTIN, LOW);
            }else if (header.indexOf("GET /0/off") >= 0) {
              Serial.println("GPIO 0 關閉");
              output0State = "關閉";
              for (pos2 = 70; pos2 >= 0; pos2 -= 1) { // goes from 180 degrees to 0 degrees
                myservo3.write(pos2);
                myservo4.write(pos2);
                //myservo2.write(210-pos);// tell servo to go to position in variable 'pos'
                delay(15);                       // waits 15ms for the servo to reach the position
              }
              digitalWrite(LED_BUILTIN, HIGH);
            } else if (header.indexOf("GET /0/on") >= 0) {
              Serial.println("GPIO 0 開啟");
              output0State = "開啟";

              for (pos2 = 0; pos2 <= 70; pos2 += 1) { // goes from 0 degrees to 180 degrees
                // in steps of 1 degree
                myservo3.write(pos2);
                myservo4.write(pos2);
                //myservo2.write(210-pos);// tell servo to go to position in variable 'pos'
                delay(15);                       // waits 15ms for the servo to reach the position
              }
              digitalWrite(LED_BUILTIN, LOW);
            }
            
            // Display the HTML web page
            client.println("<!DOCTYPE html><html>");
            client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
            client.println("<title>ESPDUINO網頁伺服器</title>");
            client.println(F("<meta charset=\"UTF-8\"></meta>")); //UTF-8
            client.println("<link rel=\"icon\" href=\"data:,\">");
            // CSS to style the on/off buttons 
            // Feel free to change the background-color and font-size attributes to fit your preferences

            client.println("<style>html { font-family:   Helvetica; display: inline-block; margin: 0px auto; text-align: center;}");
            client.println(".button {width:150px;height:80px; background-color: #ddd;border-radius: 15px;all 0.5s ease;box-shadow: 1px 1px 1px black; color: white; padding: 16px 40px;");   //#195B6A
            client.println("text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer;}");
            client.println("body {max-width:100%;height:auto; text-align:center; line-height:50px; font-size:35px; color:#000000; padding:30px 60px; font-weight:bold; background-color:#C6C6C6;}");
            client.println("table {height:auto;border:none;margin-right:auto; margin-left:auto;padding:15px 50px;  background-color:#838385;}");
            client.println("tr {height:auto;border:none;vertical-align:bottom;text-align:center;padding:15px 30px;}");
            client.println(".table2 {height:auto;border:none; margin-right:auto; margin-left:auto; background-color:#979599;}");
            client.println(".table3 {height:auto;border:none; margin-right:auto; margin-left:auto; background-color:#716E73;}");
            client.println(".table4 {height:auto;border:none; margin-right:auto; margin-left:auto; background-color:#5D5A5E;}");
            client.println(".button2 {background-color: #00FF00;}");
            client.println(".button3 {background-color: #E63F00;}</style></head>");
            // Web Page Heading

            
            client.println("<body><h1>ESPDUINO網頁伺服器</h1><br/>");
            client.println("<table><tr><td>");
            // Display current state, and ON/OFF buttons for GPIO 5
            client.println("GPIO 5 - 前輪 : &nbsp;");
            // If the output5State is off, it displays the ON button       
            client.println("<a href=\"/5/left\"><button   class=\"button button3\">左轉</button></a>&nbsp;&nbsp;&nbsp;"); // <a href=\"/5/left\"> //onmousedown=\"hold()\" onmouseup=\"nhold()\"  onclick=\"turnleft()\"
            client.println("<a href=\"/5/right\"><button   class=\"button button3\">右轉</button></a>");   //<a href=\"/5/right\">
            //   
            //}
            client.println("</td></tr></table>");
            // Display current state, and ON/OFF buttons for GPIO 4  
            client.println("<table  class=\"table2 \" ><tr><td>");
            if(output4State=="關閉") { 
              client.println("GPIO 4 - 狀態(橋前) : <font color=\"red\">關閉</font>&nbsp;&nbsp;&nbsp;&nbsp;");
            } else{
              client.println("GPIO 4 - 狀態(橋前) : <font color=\"#00FF00\">開啟</font>&nbsp;&nbsp;&nbsp;&nbsp;");
            }

            
            // If the output4State is off, it displays the ON button       
            if (output4State=="關閉") {
              client.println("<a href=\"/4/on\"><button class=\"button\">OFF</button></a>");
            } else {
              client.println("<a href=\"/4/off\"><button class=\"button button2\">ON</button></a>");
            }
            client.println("</td></tr></table>");
            // Display current state, and ON/OFF buttons for GPIO 2
            client.println("<table  class=\"table3 \" ><tr><td>");   
            if(output2State=="關閉") { 
              client.println("GPIO 2 - 狀態(橋後) : <font color=\"red\">關閉</font>&nbsp;&nbsp;&nbsp;&nbsp;");
            } else{
              client.println("GPIO 2 - 狀態(橋後) : <font color=\"#00FF00\">開啟</font>&nbsp;&nbsp;&nbsp;&nbsp;");
            }

            
            // If the output2State is off, it displays the ON button       
            if (output2State=="關閉") {
              client.println("<a href=\"/2/on\"><button class=\"button\">OFF</button></a>");
            } else {
              client.println("<a href=\"/2/off\"><button class=\"button button2\">ON</button></a>");
            }
            client.println("</td></tr></table>");
            // Display current state, and ON/OFF buttons for GPIO 0
            client.println("<table  class=\"table4 \" ><tr><td>");  
            if(output0State=="關閉") { 
              client.println("GPIO 0 - 狀態(橋同時) : <font color=\"red\">關閉</font>&nbsp;&nbsp;&nbsp;&nbsp;");
            } else{
              client.println("GPIO 0 - 狀態(橋同時) : <font color=\"#00FF00\">開啟</font>&nbsp;&nbsp;&nbsp;&nbsp;");
            }

            
            // If the output0State is off, it displays the ON button       
            if (output0State=="關閉") {
              client.println("<a href=\"/0/on\"><button class=\"button\">OFF</button></a>");
            } else {
              client.println("<a href=\"/0/off\"><button class=\"button button2\">ON</button></a>");
            }
            client.println("</td></tr></table>");
            client.println("</body></html>");
            
            // The HTTP response ends with another blank line
            client.println();
            // Break out of the while loop
            break;
          } else { // if you got a newline, then clear currentLine
            currentLine = "";
          }
        } else if (c != '\r') {  // if you got anything else but a carriage return character,
          currentLine += c;      // add it to the end of the currentLine
        }
      }
    }
    // Clear the header variable
    header = "";
    // Close the connection
    client.stop();
    Serial.println("Client disconnected.");
    Serial.println("");
  }
}
