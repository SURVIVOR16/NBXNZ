#include <U8glib.h>
#define INTERVAL_LCD             20             //定义OLED刷新时间间隔 
unsigned long lcd_time = millis();                 //OLED刷新时间计时器
U8GLIB_SSD1306_128X64 u8g(U8G_I2C_OPT_NONE);     //设置OLED型号  
#define setFont_L u8g.setFont(u8g_font_7x13)   //字体大小

#define buzzer_pin 6//定义蜂鸣器的引脚
#define PIN_IRQ 10//定义触摸开关的引脚
#include <Microduino_RTC.h>
RTC rtc;
int Ye=2017,Mo=1,Da=1,We=1,ho=1,mi=1,se=1,hour=18,minute=5,on=1,ze=0,ae=1;
DateTime dateTime = {Ye, Mo,We, Da, ho,mi , se};//设置RTC启动时间，年, 月, 星期, 日, 时, 分, 秒

//wifi
#include <ESP8266.h>

#ifdef ESP32
#error "This code is not recommended to run on the ESP32 platform! Please check your Tools->Board setting."
#endif
#if defined(__AVR_ATmega32U4__) || defined(__AVR_ATmega1284P__) || defined (__AVR_ATmega644P__) || defined(__AVR_ATmega128RFA1__)
#define EspSerial Serial1
#define UARTSPEED  115200
#endif

#if defined (__AVR_ATmega168__) || defined (__AVR_ATmega328__) || defined (__AVR_ATmega328P__)
#include <SoftwareSerial.h>
SoftwareSerial mySerial(2, 3); 

#define EspSerial mySerial
ESP8266 wifi(&EspSerial);
#define UARTSPEED  9600
#endif

#define SSID        F("iPhone")//WIFI名
#define PASSWORD    F("exoweare1")//WIFI密码
#define HOST_NAME   F("api.heclouds.com")//域名
#define DEVICEID    "502965435" //OneNet上的设备ID
#define PROJECTID   "183320" //OneNet上的产品ID
#define HOST_PORT   (80)
String apiKey="51g747=BYDNYa9=8CV0dQmeT120=";//与你的设备绑定的APIKey

//由于core的内存限制，所以要设置三个从onenet请求数据的GETDATA[]
//要修改的部分：502965435为上的OneNet上的设备ID，id=和&limit之间的是所要请求的数据流，api-key:+自己的api-key
static const byte  GETDATA1[]  PROGMEM = {
  "GET https://api.heclouds.com/devices/502965435/datapoints?datastream_id=time&limit=1 HTTP/1.1\r\nHost:api.heclouds.com\r\napi-key:51g747=BYDNYa9=8CV0dQmeT120=\r\nConnection: close\r\n\r\n"
};
static const byte  GETDATA2[]  PROGMEM = {
  "GET https://api.heclouds.com/devices/502965435/datapoints?datastream_id=hour,on,&limit=1 HTTP/1.1\r\nHost:api.heclouds.com\r\napi-key:51g747=BYDNYa9=8CV0dQmeT120=\r\nConnection: close\r\n\r\n"
};
static const byte  GETDATA3[]  PROGMEM = {
  "GET https://api.heclouds.com/devices/502965435/datapoints?datastream_id=minute,We&limit=1 HTTP/1.1\r\nHost:api.heclouds.com\r\napi-key:51g747=BYDNYa9=8CV0dQmeT120=\r\nConnection: close\r\n\r\n"
};

#define INTERVAL_SENSOR   17000             //定义传感器采样时间间隔  597000
#define INTERVAL_NET      17000             //定义发送时间
//传感器部分================================   
#include <Wire.h>                                  //调用库  
#include <I2Cdev.h>                                //调用库  
#include <Microduino_SHT2x.h>
#define  sensorPin_1  A0    //传感器引脚
#define IDLE_TIMEOUT_MS  3000      // Amount of time to wait (in milliseconds) with no data 

//WEBSITE     
char buf[10];

#define INTERVAL_sensor 2000
unsigned long sensorlastTime = millis();
float tempOLED, humiOLED, lightnessOLED;
#define INTERVAL_OLED 1000
String mCottenData;
String jsonToSend;
float sensor_tem, sensor_hum;       //传感器温度、湿度
char  sensor_tem_c[7], sensor_hum_c[7] ;    //换成char数组传输


unsigned long net_time1 = millis();                          //数据上传服务器时间
unsigned long sensor_time = millis();                        //传感器采样时间计时器
String postString;                                //用于存储发送数据的字符串
Tem_Hum_S2 TempMonitor;

void setup(void)
{
  Wire.begin();
  Serial.begin(115200);
  pinMode(buzzer_pin,OUTPUT);//蜂鸣器
   //清除所有寄存器
  rtc.begin();
  rtc.clearAll();
  //设置启动时间
  rtc.setDateTime(dateTime);
  while (!Serial); // wait for Leonardo enumeration, others continue immediately
  Serial.print(F("setup begin\r\n"));
  WifiInit(EspSerial, UARTSPEED);
  if (wifi.setOprToStationSoftAP()) {} else {}

  if (wifi.joinAP(SSID, PASSWORD)) {} else {}

 if (wifi.disableMUX()) {} else {}
 
Serial.print(F("setup end\r\n"));
}



void loop(void)     //循环函数  
{    if (wifi.createTCP(HOST_NAME, HOST_PORT)) {//如果TCP连接建立成功，运行online函数即从onenet上获取数据
    Serial.print(F("create tcp ok\r\n"));  
     online();
    } 
//以下为温湿传感器	
  if (sensor_time > millis())  sensor_time = millis();  
    
  if(millis() - sensor_time > INTERVAL_SENSOR)              //传感器采样时间间隔  
  {  
    getSensorData();                                        //读串口中的传感器数据
    sensor_time = millis();
  }  

  if (net_time1 > millis())  net_time1 = millis();
  
  if (millis() - net_time1 > INTERVAL_NET)                  //发送数据时间间隔
  {                
    updateSensorData(); //将数据上传到服务器的函数
    net_time1 = millis();
  }
//触摸开关
pinMode(PIN_IRQ,INPUT);
kaiguan=digitalRead(PIN_IRQ);
  Serial.print(digitalRead(PIN_IRQ));
  //当时间达到指定时间时OLED让蜂鸣器响起
      if (on==1&&dateTime.hour==hour&&dateTime.minute>=minute&&digitalRead(PIN_IRQ)==1)
  {   

for(int i=400;i>=200;i--)  //蜂鸣器频率
  { 
    tone(buzzer_pin,i);    //Output frequency in the port. 
    delay(5);      //The frequency lasts for 5ms.  
    if(digitalRead(PIN_IRQ)==0)//按下触摸开关，蜂鸣器停止鸣叫
    {tone(buzzer_pin,NULL);
    delay(50000);
    on=0;
    break;}
  }
  }

}
//温湿传感器获取温湿度数据，以及OLED屏幕的显示设置
void getSensorData(){  
    sensor_tem = TempMonitor.getTemperature();  
    sensor_hum = TempMonitor.getHumidity();   
      u8g.firstPage();  
    do {
      rtc.getDateTime(&dateTime);
        setFont_L;
        u8g.setPrintPos(50, 30);
        u8g.print("t ");
        u8g.print(sensor_tem);//温度
        u8g.setPrintPos(50, 45);
        u8g.print("h ");
        u8g.print(sensor_hum);//湿度
        u8g.setPrintPos(1, 45);
        u8g.print(hour);//闹钟
        u8g.print(":");
        u8g.print(minute);
        u8g.setPrintPos(1, 60);
        u8g.print("on/off ");
        u8g.print(on);//闹钟开关与否
        u8g.setPrintPos(1, 10);
        u8g.print(dateTime.year);//实时日期及时间
        u8g.print("/");
        u8g.print(dateTime.month);
        u8g.print("/");
        u8g.print(dateTime.day);
        u8g.setPrintPos(100, 10);
        u8g.print(dateTime.weekday);
        u8g.setPrintPos(1, 30);
        u8g.print(dateTime.hour);
        u8g.print(":");
        u8g.print(dateTime.minute);
        }while( u8g.nextPage() );
    delay(1000);
    dtostrf(sensor_tem, 2, 1, sensor_tem_c);
    dtostrf(sensor_hum, 2, 1, sensor_hum_c);
}

void updateSensorData() {
  if (wifi.createTCP(HOST_NAME, HOST_PORT)) { //建立TCP连接，如果失败，不能发送温湿度数据
    Serial.print("create tcp ok\r\n");

jsonToSend="{\"Temperature\":";
    dtostrf(sensor_tem,1,2,buf);
    jsonToSend+="\""+String(buf)+"\"";
    jsonToSend+=",\"Humidity\":";
    dtostrf(sensor_hum,1,2,buf);
    jsonToSend+="\""+String(buf)+"\"";
    jsonToSend+="}";


    postString="POST /devices/";
    postString+=DEVICEID;
    postString+="/datapoints?type=3 HTTP/1.1";
    postString+="\r\n";
    postString+="api-key:";
    postString+=apiKey;
    postString+="\r\n";
    postString+="Host:api.heclouds.com\r\n";
    postString+="Connection:close\r\n";
    postString+="Content-Length:";
    postString+=jsonToSend.length();
    postString+="\r\n";
    postString+="\r\n";
    postString+=jsonToSend;
    postString+="\r\n";
    postString+="\r\n";
    postString+="\r\n";

  const char *postArray = postString.c_str();                 //将str转化为char数组
  Serial.println(postArray);
  wifi.send((const uint8_t*)postArray, strlen(postArray));    //send发送命令，参数必须是这两种格式，尤其是(const uint8_t*)
  Serial.println("send success");   
     if (wifi.releaseTCP()) {                                 //释放TCP连接
        Serial.print("release tcp ok\r\n");
        } 
     else {
        Serial.print("release tcp err\r\n");
        }
      postArray = NULL;                                       //清空数组，等待下次传输数据
  

}
}
void online()//这个函数是接受onenet上的数据并将它显示在串口的
{
  if (wifi.createTCP(HOST_NAME, HOST_PORT)) {
    Serial.print(F("create tcp ok\r\n"));  
  if(ae%3==0)//ae对3取余  实现轮流读取不同数据流
  {wifi.sendFromFlash(GETDATA2, sizeof(GETDATA2));//从Flash读取发送内容，节约内存
  ae++;}
  else if(ae%3==1)
   {wifi.sendFromFlash(GETDATA3, sizeof(GETDATA3));//从Flash读取发送内容，节约内存
  ae++;
  }
    else 
   {wifi.sendFromFlash(GETDATA1, sizeof(GETDATA1));//从Flash读取发送内容，节约内存
  ae++;
  }
  char buffer[350];//因为这里需要申请一个较大的数组来获取onenet上发送过来的字符串，
                   // 当所要申请的数据流过多时，若仅用一个GETDATA结构，要申请的数组就会很大，
				   //从而占据极大的内存，会使程序由于内存限制无法获取数据
  uint32_t len = wifi.recv(buffer, sizeof(buffer), 20000);
   /* 在串口上打印从onenet上接收到的字符串，可根据该字符串，来编写下面使字符转换成数字
   从而得到所要传输数据的代码。
	if (len > 0)
    {    Serial.print(F("Received:["));
    for (uint32_t i = 0; i < len; i++) {
      Serial.print((char)buffer[i]);
    }
    Serial.print(F("]\r\n"));
	*/
       short k=0,temp=0;
      for (uint32_t i = 0; i < len-12; i++) {
        if((char)buffer[i]=='v'&&(char)buffer[i+1]=='a'&&(char)buffer[i+2]=='l'&&(char)buffer[i+3]=='u'&&(char)buffer[i+4]=='e')//标志位value
        { if((ae-1)%3==0)//这里对应三个GETDATA 结构
            {for (uint32_t j = 0; j<6;j++)
                {if((char)buffer[i+j+6]<='9'&&(char)buffer[i+j+6]>='0')
                  temp=((short)buffer[i+j+6]-48)+temp*10;//转换
                }
            
             Serial.println(temp);
             if(k==0)
             {
		     hour=temp;//借助temp赋值
             temp=0;
			 }  
             else if(k==1)
              on=temp;

             }

              else if((ae-1)%3==2)//这里对应三个GETDATA 结构
             {    for (uint32_t j = 0; j<26;j++)
                {
                  if((char)buffer[i+j+6]<='9'&&(char)buffer[i+j+6]>='0')
                     {
                      temp=((short)buffer[i+j+6]-48)+temp*10;
                      
                     }
       
            
                    else if((char)buffer[i+j+6]=='/')
                  {  Serial.println(temp);
                    ze++;   
                    if(ze==1)
                    Ye=temp;
                    else if(ze==2)
                    Mo=temp;
                    else if(ze==3)
                    Da=temp;
                    else if(ze==4)
                    ho=temp;
                    else if(ze==5)
                    mi=temp;
                    else if(ze==6)
                    se=temp;
                    
                    temp=0;  
                    dateTime = {Ye, Mo,We, Da, ho,mi , se};
                   rtc.setDateTime(dateTime);  //重新设定时间           

                }
       }}
       else  //这里对应三个GETDATA 结构           
       {  for (uint32_t j = 0; j<6;j++)
                {if((char)buffer[i+j+6]<='9'&&(char)buffer[i+j+6]>='0')
            temp=((short)buffer[i+j+6]-48)+temp*10;
                }
            
             Serial.println(temp); 
             if(k==0)
             We=temp;
             else if(k==1)
            minute=temp;
            dateTime = {Ye, Mo,We, Da, ho,mi , se};
            rtc.setDateTime(dateTime);}

             k++;
             temp=0;
        }    
      }
  } 
  if (wifi.releaseTCP()) {
    Serial.print(F("release tcp ok\r\n"));
  } else {
    Serial.print(F("release tcp err\r\n"));
  }
 /*在串口上打印获得的数据
 Serial.print(hour);
 Serial.print(on);
  Serial.print(minute);
 Serial.print(We);
  Serial.print(Ye);
 Serial.print(Mo);
 Serial.print(Da);
 Serial.print(ho);
 Serial.print(mi);
 Serial.print(se);
*/
  } else {
    Serial.print(F("create tcp err\r\n"));
  }


}
