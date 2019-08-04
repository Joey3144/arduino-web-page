
/*********
  Rui Santos
  Complete project details at https://randomnerdtutorials.com  
*********/
// Load Wi-Fi library
#include <ESP8266WiFi.h>
#include <Servo.h>


// 伺服馬達宣告
Servo myservo1;
Servo myservo2;
Servo myservo4;
Servo myservo3;


// 網路ID與密碼
const char* ssid     = "iphone";
const char* password = "adcebkcu";
int servo1pos = 0;
int servo2pos = 0;
int servo3pos = 0;
int n = 90;  //servo1 position temp
bool LeftButtonDown=false;
bool RightButtonDown=false;


// Set web server port number to 80
WiFiServer server(80);


// Variable to store the HTTP request
String header;


// Auxiliar variables to store the current output state
String output1State = "未啟動";
String output2State = "關閉";
String output3State = "關閉";
String output4State = "關閉";


// Assign output variables to GPIO pins
const int output4 = 5;
const int output3 = 4;
const int output2 = 2;
const int output1 = 0;




void setup() {
  Serial.begin(115200);
  
  // 伺服馬達腳位設定
  myservo1.attach(D0);  
  myservo2.attach(D3);
  myservo3.attach(D5);
  myservo4.attach(D16); //avail: 0 (2) 3 4 5 16 unavail: 1

  
  // Initialize the output variables as outputs
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(output1, OUTPUT);
  pinMode(output2, OUTPUT);
  pinMode(output3, OUTPUT);
  pinMode(output4, OUTPUT);


  // Set outputs to LOW
  digitalWrite(output1, LOW);
  digitalWrite(output2, LOW);
  digitalWrite(output3, LOW);
  digitalWrite(output4, LOW);

  
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

  //myservo1.write(360);
  //myservo1.write(-360);
  
  WiFiClient client = server.available();   // Listen for incoming clients
  
  if(client)
  {
      LeftButtonDown=false;
      RightButtonDown=false;
  }
  
  if (client || (LeftButtonDown==true&&n>30) || (RightButtonDown==true&&n<150)) {             // If a new client connects   
    String currentLine = "";                                                              // make a String to hold incoming data from the client
    while (client.connected() || LeftButtonDown==true || RightButtonDown==true) {           // loop while the client's connected
      
      if (client.available() || LeftButtonDown==true || RightButtonDown==true) {             // if there's bytes to read from the client,  client.available()        
        char c = client.read();             // read a byte, then          
        header += c;
    
        if (c == '\n' || LeftButtonDown==true || RightButtonDown==true) {                    // if the byte is a newline character c == '\n'
          // if the current line is blank, you got two newline characters in a row.
          // that's the end of the client HTTP request, so send a response:
          if (currentLine.length() >= 0) 
            { 
              
            // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
            // and a content-type so the client knows what's coming, then a blank line:
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println("Connection: close");
            client.println();
            myservo1.write(n);

            if(client.available())
            {
                LeftButtonDown=false;
                RightButtonDown=false;
            }
            
            if((header.indexOf("GET /1/left") >= 0))
            {
              LeftButtonDown=true;
            }
            
            if((header.indexOf("GET /1/right") >= 0))
            {
              RightButtonDown=true;
            }
            
            if(((header.indexOf("GET /1/left")>=0)) || (LeftButtonDown==true))
            {
              if(n>30)
              {
                servo1pos=n;
                servo1pos--;
                Serial.println(servo1pos);
                myservo1.write(servo1pos);
                n=servo1pos;
                delay(8);
              } 
            }
            
            if((header.indexOf("GET /1/right")>=0) || (RightButtonDown==true))
            {
              if(n<150)
              {
                servo1pos=n;
                servo1pos++;
                Serial.println(servo1pos);
                myservo1.write(servo1pos);
                n=servo1pos;
                delay(8); 
              }
            }
            
            if (header.indexOf("GET /2/back") >= 0) 
            {
              Serial.println("GPIO 2 關閉");
              output2State = "關閉";
              digitalWrite(D3, HIGH);
              digitalWrite(D16, LOW);
              delay(15);                       
            } 
            
            else if (header.indexOf("GET /2/go") >= 0) 
            {
              Serial.println("GPIO 2 開啟");
              output2State = "開啟";
              digitalWrite(D3, LOW);
              digitalWrite(D16, HIGH);
              delay(15);                       
            }
            
            else if (header.indexOf("GET /3/off") >= 0) 
            {
              Serial.println("GPIO 3 關閉");
              output3State = "關閉";
              for (servo3pos = 70; servo3pos >= 0; servo3pos -= 1) 
              { 
                myservo3.write(servo3pos);
                delay(15);                       
              }              
              digitalWrite(LED_BUILTIN, HIGH);
            } 
            
            else if (header.indexOf("GET /3/on") >= 0) {
              Serial.println("GPIO 3 開啟");
              output3State = "開啟";
              for (servo3pos = 0; servo3pos <= 70; servo3pos += 1) 
                { 
                myservo3.write(servo3pos);
                delay(15);                     
                }
              digitalWrite(LED_BUILTIN, LOW);
            }
            
            else if (header.indexOf("GET /4/off") >= 0) {
              Serial.println("GPIO 4 關閉");
              output4State = "關閉";
              for (servo2pos = 70; servo2pos >= 0; servo2pos -= 1) 
                { 
                myservo4.write(servo2pos);
                myservo3.write(servo2pos);
                delay(15);                      
                }
              digitalWrite(LED_BUILTIN, HIGH);
            } 
            
            else if (header.indexOf("GET /4/on") >= 0) 
            {
              Serial.println("GPIO 4 開啟");
              output4State = "開啟";
              for (servo2pos = 0; servo2pos <= 70; servo2pos += 1) 
              {
                myservo4.write(servo2pos);
                myservo3.write(servo2pos);
                delay(15);                      
              }
              digitalWrite(LED_BUILTIN, LOW);
            }
            
            // 網頁 
            
            client.println("<!DOCTYPE html><html>");
            client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
            client.println("<title>ESPDUINO網頁伺服器</title>");
            client.println(F("<meta charset=\"UTF-8\"></meta>")); //UTF-8
            client.println("<link rel=\"icon\" href=\"data:,\">");

            
            // CSS部分 
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

            
            // JavaScript部分
            client.println("<script>");
            client.println("var uu=1");
            client.println("function turnleft(){location.href=\"http://172.20.10.3/1/left\"}");
            client.println("function turnright(){location.href=\"http://172.20.10.3/1/right\"}");
            client.println("function mouseup(){location.href=\"http://172.20.10.3/1/up\"}");
            client.println("function mousemove(){location.href=\"http://172.20.10.3/1/up\"}");           
            client.println("</script>");


            //Body部分
            client.println("<body><h1>ESPDUINO網頁伺服器</h1><br/>");

            // 車子方向 (左轉、右轉)
            client.println("<table><tr><td>");            
            client.println("車子 - 方向 : &nbsp;");
            client.println("<button  onmousedown=\"turnleft()\"     onmouseup=\"mouseup()\"  class=\"button button3\">左轉</button>&nbsp;&nbsp;&nbsp;"); 
            client.println("<button  onmousedown=\"turnright()\"    onmouseup=\"mouseup()\"  class=\"button button3\">右轉</button>");   
            client.println("</td></tr></table>");
            
            // 車子前進、後退
            client.println("<table  class=\"table2 \" ><tr><td>");
            
            if(output2State=="關閉") 
            { 
              client.println("車子 - 後退 : <font color=\"red\">後退</font>&nbsp;&nbsp;&nbsp;&nbsp;");
            } 
            else
            {
              client.println("車子 - 前進 : <font color=\"#00FF00\">前進</font>&nbsp;&nbsp;&nbsp;&nbsp;");
            }

                 
            if (output2State=="關閉") 
            {
              client.println("<a href=\"/2/go\"><button class=\"button\">後退</button></a>");
            } 
            else 
            {
              client.println("<a href=\"/2/back\"><button class=\"button button2\">前進</button></a>");
            }
            
            // 半橋的升降
            client.println("</td></tr></table>");
            client.println("<table  class=\"table3 \" ><tr><td>");   
            if(output3State=="關閉") 
            { 
              client.println("GPIO 2 - 狀態(橋後) : <font color=\"red\">關閉</font>&nbsp;&nbsp;&nbsp;&nbsp;");
            } else{
              client.println("GPIO 2 - 狀態(橋後) : <font color=\"#00FF00\">開啟</font>&nbsp;&nbsp;&nbsp;&nbsp;");
            }
     
            if (output3State=="關閉") 
            {
              client.println("<a href=\"/3/on\"><button class=\"button\">OFF</button></a>");
            } 
            else 
            {
              client.println("<a href=\"/3/off\"><button class=\"button button2\">ON</button></a>");
            }

            // 兩橋的升降
            client.println("</td></tr></table>");
            // Display current state, and ON/OFF buttons for GPIO 0
            client.println("<table  class=\"table4 \" ><tr><td>");  
            if(output4State=="關閉") 
            { 
              client.println("GPIO 0 - 狀態(橋同時) : <font color=\"red\">關閉</font>&nbsp;&nbsp;&nbsp;&nbsp;");
            } 
            else
            {
              client.println("GPIO 0 - 狀態(橋同時) : <font color=\"#00FF00\">開啟</font>&nbsp;&nbsp;&nbsp;&nbsp;");
            }
   
            if (output4State=="關閉") 
            {
              client.println("<a href=\"/4/on\"><button class=\"button\">OFF</button></a>");
            } 
            else 
            {
              client.println("<a href=\"/4/off\"><button class=\"button button2\">ON</button></a>");
            }
            client.println("</td></tr></table>");
            client.println("</body></html>");
            
            // The HTTP response ends with another blank line
            //client.println();
            
            // Break out of the while loop
            break;
            
          } 
          else 
          { 
            // if you got a newline, then clear currentLine
            currentLine = "";
          }
        } 
        else if (c != '\r') 
        {  // if you got anything else but a carriage return character,
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
    delay(30);
  }

}
