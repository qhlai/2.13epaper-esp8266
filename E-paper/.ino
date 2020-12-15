/**
 *  @filename   :   epd2in13-demo.ino
 *  @brief      :   2.13inch e-paper display demo
 *  @author     :   Yehui from Waveshare
 *
 *  Copyright (C) Waveshare     September 9 2017
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documnetation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to  whom the Software is
 * furished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS OR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */


#include <SPI.h>
#include "epd2in13.h"
#include "epdpaint.h"
#include "imagedata.h"
#include "Config.h"
#define COLORED     0
#define UNCOLORED   1


/**
  * Due to RAM not enough in Arduino UNO, a frame buffer is not allowed.
  * In this case, a smaller image buffer is allocated and you have to 
  * update a partial display several times.
  * 1 byte = 8 pixels, therefore you have to set 8*N pixels at a time.
  */
unsigned char image[4000];
Paint paint(image, 0, 0);
Epd epd;
unsigned long time_start_ms;
unsigned long time_now_s;
String SN;
String tempStr;
DATE Date_Time;

//char display_cache[EPD_HEIGHT*EPD_WIDTH];
//void Fast(char* a, unsigned char* b)
//{
//	for (int j = 0; j < EPD_HEIGHT; j++) {
//		for (int i = 0; i < EPD_WIDTH / 8; i++) {
//			a[i + j * (EPD_WIDTH / 8)] = b[i + j * (EPD_WIDTH / 8)];
//		}
//	}
//}
void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);

  Serial.println("");
  Serial.print("SN:");
  SN = (String)system_get_chip_id();
  Serial.println(SN);

  Serial.println("boot");
  if (epd.Init(lut_full_update) != 0) {
      Serial.print("e-Paper full init failed");
      return;
  }
  //Serial.println("e-Paper init full clear ok");
  //Serial.println("e-Paper init ok ---- 1");
 //Serial.println("e-Paper init ok");
  epd.ClearFrameMemory(0xFF);   // bit set = white, bit reset = black
  epd.DisplayFrame();
  epd.ClearFrameMemory(0xFF);

  if (epd.Init(lut_partial_update) != 0) {
	  Serial.print("e-Paper partial init failed");
	  return;
  }
  paint.SetRotate(ROTATE_0);
  paint.SetWidth(128);    // width should be the multiple of 8 
  paint.SetHeight(12);

  /* For simplicity, the arguments are explicit numerical coordinates */
  paint.Clear(COLORED);
  paint.DrawStringAt(0, 1, "Testing", &Font12, UNCOLORED);
  epd.SetFrameMemory(paint.GetImage(), 0, 0, paint.GetWidth(), paint.GetHeight());
  epd.DisplayFrame();
  epd.SetFrameMemory(paint.GetImage(), 0, 0, paint.GetWidth(), paint.GetHeight());

  paint.SetWidth(128);    // width should be the multiple of 8 
  paint.SetHeight(230);
  paint.Clear(UNCOLORED);


  paint.DrawStringAt(0, 0, "WiFi Connecting", &Font12, COLORED);
  //epd.SetFrameMemory(paint.GetImage(), 0, 12, paint.GetWidth(), paint.GetHeight());

  //paint.Clear(UNCOLORED);
  paint.DrawStringAt(0, 12, "WIFI_SSID:", &Font12, COLORED);
  //epd.SetFrameMemory(paint.GetImage(), 0, 12, paint.GetWidth(), paint.GetHeight());

  //paint.Clear(UNCOLORED);
  paint.DrawStringAt(0, 24, (String("") + WIFI_SSID).c_str(), &Font12, COLORED);
  //epd.SetFrameMemory(paint.GetImage(), 0, 12, paint.GetWidth(), paint.GetHeight());
  epd.DisplayFrame();
  //Fast(display_cache, paint.GetImage())
  //epd.DisplayFrame();

  
  //epd.ClearFrameMemory(0xFF);
  //paint.SetWidth(64);
  //paint.SetHeight(64);
  //ConnectWiFi();
  //if (WiFi.status() != WL_CONNECTED)
  //{

  //}
  //paint.Clear(UNCOLORED);
  paint.DrawStringAt(0, 36, ConnectWiFi().c_str(), &Font12, COLORED);
  //paint.DrawStringAt(0, 2, IpAddress2String(WiFi.localIP()).c_str(), &Font12, COLORED);
  //paint.DrawStringAt(30, 4, ConnectWiFi(), &Font8, COLORED);
  epd.SetFrameMemory(paint.GetImage(), 0, 12, paint.GetWidth(), paint.GetHeight());
  epd.DisplayFrame();


  //paint.Clear(UNCOLORED);
  Date_Time=DateHandle();
  if (Date_Time.GetNetDate == true)
  {
	  paint.DrawStringAt(0, 48, "GetNetDate OK", &Font12, COLORED);
  }
  else
  {
	  paint.DrawStringAt(0, 48, "GetNetDate Failed", &Font12, COLORED);
  }
  epd.SetFrameMemory(paint.GetImage(), 0, 12, paint.GetWidth(), paint.GetHeight());
  epd.DisplayFrame();
  //epd.SetFrameMemory(paint.GetImage(), 0, 12, paint.GetWidth(), paint.GetHeight());
  //epd.DisplayFrame();
  //epd.DisplayFrame();
 // epd.SetFrameMemory(paint.GetImage(), 0, 48, paint.GetWidth(), paint.GetHeight());


  //epd.SetFrameMemory(paint.GetImage(), 0, 48, paint.GetWidth(), paint.GetHeight());

  //GetOpenwrtData();
  //epd.SetFrameMemory(paint.GetImage(), 0, 40, paint.GetWidth(), paint.GetHeight());
  //epd.DisplayFrame();
  //paint.Clear(UNCOLORED);
  //paint.DrawRectangle(0, 0, 40, 50, COLORED);
  //paint.DrawLine(0, 0, 40, 50, COLORED);
  //paint.DrawLine(40, 0, 0, 50, COLORED);
  //epd.SetFrameMemory(paint.GetImage(), 16, 60, paint.GetWidth(), paint.GetHeight());

  //paint.Clear(UNCOLORED);
  //paint.DrawCircle(32, 32, 30, COLORED);
  //epd.SetFrameMemory(paint.GetImage(), 72, 60, paint.GetWidth(), paint.GetHeight());

  //paint.Clear(UNCOLORED);
  //paint.DrawFilledRectangle(0, 0, 40, 50, COLORED);
  //epd.SetFrameMemory(paint.GetImage(), 16, 130, paint.GetWidth(), paint.GetHeight());

  //paint.Clear(UNCOLORED);
  //paint.DrawFilledCircle(32, 32, 30, COLORED);
  //epd.SetFrameMemory(paint.GetImage(), 72, 130, paint.GetWidth(), paint.GetHeight());

   Serial.println("e-Paper init done");

  delay(3000);



  /** 
   *  there are 2 memory areas embedded in the e-paper display
   *  and once the display is refreshed, the memory area will be auto-toggled,
   *  i.e. the next action of SetFrameMemory will set the other memory area
   *  therefore you have to set the frame memory and refresh the display twice.
   */ 
   //epd.SetFrameMemory(IMAGE_DATA);
 // epd.SetFrameMemory(IMAGE_DATA);
  epd.ClearFrameMemory(0xFF);
  epd.DisplayFrame();
  delay(5);
  epd.ClearFrameMemory(0xFF);
  epd.DisplayFrame();
  delay(5);
  epd.ClearFrameMemory(0xFF);
  epd.DisplayFrame();
  delay(5);
  //epd.SetFrameMemory(IMAGE_DATA);
  //epd.DisplayFrame();

  time_start_ms = millis();

}

void loop() {
	//GetTime();
	Date_Time =DateHandle();
 // // put your main code here, to run repeatedly:
    time_now_s = (millis() - time_start_ms) / 1000;
 // char time_string[] = {'0', '0', ':', '0', '0', '\0'};
 // time_string[0] = time_now_s / 60 / 10 + '0';
 // time_string[1] = time_now_s / 60 % 10 + '0';
 // time_string[3] = time_now_s % 60 / 10 + '0';
 // time_string[4] = time_now_s % 60 % 10 + '0';
	//String DateNow = String(Date_Time.Year) + String(" ") + String(Date_Time.Month) + String(" ") + String(Date_Time.Day) + String(" ") + String(Date_Time.Hour) + String(" ") + String(Date_Time.Minute) + String(" ") + String(Date_Time.Second);
		//Serial.println( String("Year ")+String(Date_Time.Year) );
		//Serial.println(String("M ") + String(Date_Time.Month));
		//Serial.println(String("D ") + String(Date_Time.Day));
		//Serial.println(String("H ") + String(Date_Time.Hour));
		//Serial.println(String("Mi ") + String(Date_Time.Minute));
		//Serial.println(String("S ") + String(Date_Time.Second));

	paint.SetWidth(128);
	paint.SetHeight(250);
	paint.Clear(UNCOLORED);
	paint.SetRotate(ROTATE_90);

	paint.DrawStringAt(0, 6, (toStringAddZero2(Date_Time.Year)).c_str(), &Font24, COLORED);
    paint.DrawStringAt(20, 26, (toStringAddZero2(Date_Time.Month) + String("/") + String(Date_Time.Day)).c_str(), &Font20, COLORED);
	paint.DrawStringAt(40, 46, (toStringAddZero2(Date_Time.Second)).c_str(), &Font20, COLORED);
	epd.SetFrameMemory(paint.GetImage(), 0, 0, paint.GetWidth(), paint.GetHeight());


	//paint.SetWidth(32);
	//paint.SetHeight(64);
	//paint.Clear(UNCOLORED);
	//paint.SetRotate(ROTATE_90);
	//paint.DrawStringAt(0, 0, (String(Date_Time.Year) ).c_str(), &Font24, COLORED);
	//epd.SetFrameMemory(paint.GetImage(), 80, 170, paint.GetWidth(), paint.GetHeight());

	//paint.SetWidth(32);
	//paint.SetHeight(64);
	//paint.Clear(UNCOLORED);
	//paint.SetRotate(ROTATE_90);
	//paint.DrawStringAt(0, 0, (String(Date_Time.Month) + String("/") + String(Date_Time.Day)).c_str(), &Font20, COLORED);
	//epd.SetFrameMemory(paint.GetImage(), 60, 170, paint.GetWidth(), paint.GetHeight());

	//paint.SetWidth(32);
	//paint.SetHeight(80);
	//paint.Clear(UNCOLORED);
	//paint.SetRotate(ROTATE_90);
	//paint.DrawStringAt(6, 5, (String(Date_Time.Hour)+ String(":")+ String(Date_Time.Minute)).c_str(), &Font24, COLORED);
	//epd.SetFrameMemory(paint.GetImage(), 40, 170, paint.GetWidth(), paint.GetHeight());

	////paint.SetWidth(24);
	////paint.SetHeight(32);
	////paint.Clear(UNCOLORED);
	////paint.SetRotate(ROTATE_90);
	////paint.DrawStringAt(0, 5, String(Date_Time.Second).c_str(), &Font20, COLORED);
	////epd.SetFrameMemory(paint.GetImage(), 0, 170, paint.GetWidth(), paint.GetHeight());

	/*paint.SetWidth(32);
	paint.SetHeight(256);
	paint.SetRotate(ROTATE_90);

	paint.Clear(UNCOLORED);
	paint.DrawStringAt(0, 4, DateNow.c_str(), &Font12, COLORED);
	epd.SetFrameMemory(paint.GetImage(), 40, 20, paint.GetWidth(), paint.GetHeight());*/
	epd.DisplayFrame();
	 //Serial.print(".");
	 delay(1000);
  
}
