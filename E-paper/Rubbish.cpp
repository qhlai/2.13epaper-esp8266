#if 0

if (Date_Time.Minute % 300 == 0)
{
	Date_Time = DateHandle();
	TimeFlush = true;
	NetTimeFlush = true;
	/*DayFlush = true;*/
	HourFlush = true;
	MinuteFlush = true;
	SecondFlush = true;
}
else
{
	if (time_now_s > 300)
	{
		time_start_ms = millis();
		Date_Time.Second += time_now_s;
		SecondFlush = true;
		if (Date_Time.Second >= 60)
		{
			Date_Time.Second = int(Date_Time.Second % 60);
			Date_Time.Minute++;
			MinuteFlush = true;
			/*YearFlush = true;*/
			if (Date_Time.Minute >= 60)
			{
				Date_Time.Hour++;
				if (Date_Time.Hour == 0 && Date_Time.Minute == 0)
				{
					Date_Time.Day++;
					/*DayFlush = true;*/
					if (Date_Time.Day >= 31)
					{
						Date_Time.Month++;
						Date_Time.Day = 0;
					}
				}
			}

		}

		TimeFlush = true;

	}
	//Dat			
	display.println(String(Date_Time.Second));
	display.println(String(Date_Time.Minute));
	display.println("123");


	void TimePartialUpdate()
	{
		static bool TimeFlush = true;
		static bool NetTimeFlush = true;
		static bool DayFlush = true;
		static bool HourFlush = true;
		static bool MinuteFlush = true;
		static bool SecondFlush = true;
		time_now_s = (millis() - time_start_ms) / 1000;

		//if (Date_Time.Minute % 300 == 0)

		TimeFlush = true;
		NetTimeFlush = true;
		DATE Date_Time_tmp;
		Date_Time_tmp = DateHandle();
		if (Date_Time_tmp.Day != Date_Time.Day)
		{
			DayFlush = true;
		}
		if (Date_Time_tmp.Minute != Date_Time.Minute)
		{
			MinuteFlush = true;
		}
		Date_Time = Date_Time_tmp;
		//SecondFlush = true;

		//Date_Time.Minute = Date_Time.Minute + int(time_now_s / 60 / 10)*10 + int(time_now_s / 60 % 10);//h
		////Date_Time.Second = int(time_now_s % 60)*10  + int(time_now_s % 60 % 10);


		if (TimeFlush || NetTimeFlush)
		{
			// some useful background
		   //helloWorld();
			// use asymmetric values for test
			display.setRotation(1);
			//display.setFont(&DSEG7_Classic_Mini_Bold_40);
			display.setTextColor(GxEPD_BLACK);
			// show where the update box is
			if (NetTimeFlush)
			{
				//YearFlush = false;
				display.setFont(&DejaVu_Serif_Bold_12);
				String Year_Show = String(Date_Time.Year);
				display.firstPage();
				do
				{
					//display.fillRect(100, 20, 48, 12, GxEPD_WHITE);
					display.setCursor(100, 20);
					display.println(Year_Show);
				} while (display.nextPage());
			}
			if (DayFlush)
			{
				display.setFont(&DejaVu_Serif_Bold_12);
				String Mou_DayShow = toStringAddZero2(Date_Time.Month) + "/" + toStringAddZero2(Date_Time.Day);
				//display.firstPage();
				do
				{
					//display.fillRect(140, 20, 64, 12, GxEPD_WHITE);
					display.setCursor(140, 20);
					display.println(Mou_DayShow);
				} while (display.nextPage());
			}
			if (MinuteFlush)
			{
				MinuteFlush = false;
				display.setFont(&DSEG7_Classic_Mini_Bold_40);
				String Hour_MinShow = toStringAddZero2(Date_Time.Hour) + ":" + toStringAddZero2(Date_Time.Minute);
				//display.firstPage();
				do
				{
					//display.fillRect(100, 80, 130, 40, GxEPD_WHITE);
					display.setCursor(100, 80);
					display.println(Hour_MinShow);
					/*display.setCursor(100, 80);
					display.println(".");*/
				} while (display.nextPage());
			}
			//if (SecondFlush)
			//{
			//	SecondFlush = false;
			//	display.setFont(&DSEG7_Classic_Mini_Bold_30);
			//	String Second_Show = toStringAddZero2(Date_Time.Second);
			//	//display.firstPage();
			//	do
			//	{
			//		//display.fillRect(140, 120, 60, 40, GxEPD_WHITE);
			//		display.setCursor(190, 120);
			//		display.println(Second_Show);
			//		/*display.setCursor(100, 80);
			//		display.println(".");*/
			//	} while (display.nextPage());
			//}
			if (NetTimeFlush)NetTimeFlush = false;
			if (TimeFlush)TimeFlush = false;

		}
		// note for partial update window and setPartialWindow() method:
		// partial update window size and position is on byte boundary in physical x direction
		// the size is increased in setPartialWindow() if x or w are not multiple of 8 for even rotation, y or h for odd rotation
		// see also comment in GxEPD2_BW.h, GxEPD2_3C.h or GxEPD2_GFX.h for method setPartialWindow()
		// showPartialUpdate() purposely uses values that are not multiples of 8 to test this
	}

#endif