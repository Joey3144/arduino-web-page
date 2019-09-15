
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
const char* ssid     = "yuting99";
const char* password = "shung1210";
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
String output2State = "前進";
String output3State = "關閉";
String output4State = "降下";


// Assign output variables to GPIO pins
const int output4 = 5;
const int output3 = 4;
const int output2 = 2;
const int output1 = 0;




void setup() {
  Serial.begin(115200);
  
  // 伺服馬達腳位設定
  //腳位備註: avail: 0 (2) 3 4 5 16 unavail: 1
  myservo1.attach(D0);  //前輪
  myservo2.attach(D3);  //後輪
  myservo3.attach(D5);
  myservo4.attach(D16); //後輪
  

  
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

            /*if(header.indexOf("GET /1/up") >= 0)
            {
                LeftButtonDown=false;
                RightButtonDown=false;
            }*/
            
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
                servo1pos-=1;
                Serial.println(servo1pos);
                myservo1.write(servo1pos);
                n=servo1pos;
                delay(2);
              } 
            }
            
            else if((header.indexOf("GET /1/right")>=0) || (RightButtonDown==true))
            {
              if(n<150)
              {
                servo1pos=n;
                servo1pos+=1;
                Serial.println(servo1pos);
                myservo1.write(servo1pos);
                n=servo1pos;
                delay(2); 
              }
            }
            
            else if (header.indexOf("GET /2/back") >= 0) 
            {
              Serial.println("後退");
              output2State = "後退"; //好像不用了，再測測看
              myservo2.attach(D3);  
              myservo4.attach(D16);
              myservo2.write(360);
              myservo4.write(-360);
              delay(1);                       
            } 
            
            else if (header.indexOf("GET /2/go") >= 0) 
            {
              Serial.println("前進");
              output2State = "前進";//好像不用了，再測測看
              myservo2.attach(D3);  
              myservo4.attach(D16);
              myservo2.write(-360);
              myservo4.write(360);
              delay(100);                       
            }

            else if (header.indexOf("GET /2/stop") >= 0) 
            {
              Serial.println("停止");
              output2State = "停止";//好像不用了，再測測看
              myservo2.detach();  
              myservo4.detach();
              delay(100);                       
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

            //降下
            else if (header.indexOf("GET /4/down") >= 0) {
              Serial.println("降下");
              output4State = "降下";
              for (servo2pos = 70; servo2pos >= 0; servo2pos -= 1) 
                { 
                myservo4.write(servo2pos);
                myservo3.write(servo2pos);
                delay(15);                      
                }
              digitalWrite(LED_BUILTIN, HIGH);
            } 

            //升起
            else if (header.indexOf("GET /4/up") >= 0) 
            {
              Serial.println("升起");
              output4State = "升起";
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
            client.println("<style>html { font-family:   Helvetica; display: inline-block; margin: 0px auto; }");
            client.println(".button {width:150px;height:80px; background-color: #ddd;border-radius: 15px;all 0.5s ease;box-shadow: 1px 1px 1px black; color: white; padding: 16px 40px;");   //#195B6A
            client.println("text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer;}");
            client.println("body {max-width:100%; height:auto;  line-height:50px; font-size:35px; color:#000000; padding:30px 60px; font-weight:bold; background-color:#C6C6C6;}");
            client.println("table {width:100%; max-width:800px; height:120px; border:none; padding:15px 15px 15px 25px; margin-left:auto; margin-right:auto; background-color:#838385;}");
            client.println("tr { border:none; }");
            client.println(".tdhead { width:22%; text-align:center; }");
            client.println(".tdsign { width:3%; }");
            client.println(".tdbutton { text-align:center; }");
            client.println(".table2 { background-color:#979599;}");
            client.println(".table3 { background-color:#716E73;}");
            client.println(".table4 { background-color:#5D5A5E;}");
            client.println(".button2 {background-color: #00FF00;}");
            client.println(".button3 {background-color: #E63F00;}</style></head>");


            
            // JavaScript部分
            client.println("<script>");
            client.println("function turnleft(){location.href=\"/1/left\"}");
            client.println("function turnright(){location.href=\"/1/right\"}");
            client.println("function mouseup(){location.href=\"/1/up\"}");        
            client.println("</script>");


            //Body部分
            client.println("<body><div style=text-align:center><h1>ESPDUINO網頁伺服器</h1></div>");

            // 車子方向 (左轉、右轉)
            client.println("<table><tr>");   
            client.println("<td class=\"tdhead\">");          
            client.println("前輪控制 ");
            client.println("</td>");

            client.println("<td class=\"tdsign\">");          
            client.println(":");
            client.println("</td>");
            
            client.println("<td class=\"tdbutton\">");
            client.println("<button  onmousedown=\"turnleft()\"     onmouseup=\"mouseup()\"  class=\"button button3\">左轉</button>"); 
            client.println("</td>");
            
            client.println("<td class=\"tdbutton\">");
            client.println("<button  onmousedown=\"turnright()\"    onmouseup=\"mouseup()\"  class=\"button button3\">右轉</button>");   
            client.println("</td>");
            client.println("</tr></table>");
            
            // 車子前進、後退
            client.println("<table  class=\"table2 \" ><tr>"); 
            client.println("<td class=\"tdhead\">"); 
            client.println("後輪控制 ");
            client.println("</td>");

            client.println("<td class=\"tdsign\">");          
            client.println(":");
            client.println("</td>");
            
            client.println("<td class=\"tdbutton\">");
            client.println("<a href= \"/2/go\" >   <button   class=\"button button3\">前進</button> </a>"); 
            client.println("</td>");
            
            client.println("<td class=\"tdbutton\">");
            client.println("<a href= \"/2/back\" > <button   class=\"button button3\">後退</button> </a>");
            client.println("</td>");
            
            client.println("<td class=\"tdbutton\">");
            client.println("<a href= \"/2/stop\" > <button   class=\"button button3\">停止</button> </a>");      
            client.println("</td>");
            client.println("</tr></table>");
            
            // 半橋的升降
            client.println("<table  class=\"table3 \" ><tr>");   
            client.println("<td class=\"tdhead\">");   
            if(output3State=="關閉") 
            { 
              client.println("無作用 <font color=\"red\"></font>");
            } else{
              client.println("無作用 <font color=\"#00FF00\"></font>");
            }
            client.println("</td>");

            client.println("<td class=\"tdsign\">");          
            client.println(":");
            client.println("</td>");
            
            client.println("<td class=\"tdbutton\">");
            if (output3State=="關閉") 
            {
              client.println("<a href=\"/3/on\"><button class=\"button\">OFF</button></a>");
            } 
            else 
            {
              client.println("<a href=\"/3/off\"><button class=\"button button2\">ON</button></a>");
            }
            client.println("</td>");
            client.println("</tr></table>");

            // 兩橋的升降
            client.println("<table  class=\"table4 \" ><tr>");  
            client.println("<td class=\"tdhead\">");  
            if(output4State=="降下") 
            { 
              client.println("橋控制<font color=\"red\"></font>");
            } 
            else
            {
              client.println("橋控制<font color=\"#00FF00\"></font>");
            }
            client.println("</td>");

            client.println("<td class=\"tdsign\">");          
            client.println(":");
            client.println("</td>");
            
            client.println("<td class=\"tdbutton\">");
            if (output4State=="降下") 
            {
              client.println("<a href=\"/4/up\"><button class=\"button\">降下</button></a>");
            } 
            else 
            {
              client.println("<a href=\"/4/down\"><button class=\"button button2\">升起</button></a>");
            }
            client.println("</td>");
            client.println("</tr></table>");
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
    delay(5);
  }

}
