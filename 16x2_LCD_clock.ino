#include <SoftwareSerial.h>
#include <Button.h>

SoftwareSerial lcd(2, 3);  // This is required, to start an instance of an LCD
int year = 14;  // Enter the current year here, 11 = 2011
int month = 9;  // Enter the current month here, 6 = June
int date = 27;  // Enter the current date here
int day = 6;  // 0 = Sunday, 6 = Saturday
int hours = 20;  // Enter the hours here
int minutes = 00;  // Enter the minutes here
int seconds = 07;  // Enter the seconds here
int buttonMode = 1;  // Default buttonMode

//boolean s1 = false;
//boolean s2 = false;
//boolean s3 = false;
//boolean s4 = false;

//different way for stiwtch states
int state[3];
int i;


// Setting the two buttons
Button buttonOne = Button(8, PULLDOWN);
Button buttonTwo = Button(9, PULLDOWN);

//Setting the four button mode switches
//Button modeSwitchA = Button(4, PULLDOWN);
//Button modeSwitchB = Button(5, PULLDOWN);
//Button modeSwitchC = Butotn(6, PULLDOWN);
//button modeSwitchD = Button(7, PULLDOWN);

void setup()
{
	// special shit
	for (i = 7; i >= 4; i--)
	{
		pinMode(i, INPUT);
	}
	// end special shit
	
  lcd.begin(9600);  // Start the LCD at 9600 baud
  clearDisplay();  // Clear the display
  setLCDCursor(2);  // Set cursor to the 3rd spot, 1st line
  lcd.print("Hello, world");
  setLCDCursor(16);  // Set the cursor to the beginning of the 2nd line
  lcd.print("Running clock...");
  
  // Flash the backlight:
  for (int i=0; i<3; i++)
  {
    setBacklight(0);
    delay(250);
    setBacklight(255);
    delay(250);
  }  
}

void loop()
{
	// special shit
	for (i = 0, j = 7; i < 6, j >= 2; i++, j--) {
		state[i] = digitalRead(j);
    }
	
	// end special shit
	
	
  manualAdjust();
  if (!(millis() % 1000))  // If it's been 1 second
  {
    checkTime();  // this function increases the time and date if necessary
    clearDisplay();  // Clear the display
    setLCDCursor(1);  // set cursor to 2nd spot, 1st row
    printDay();  // print the day
	printDate();

    timeBrightness();  //my bit to adjust brightness based on time of day
	
    setLCDCursor(20);  // set the cursor to the 5th spot, 2nd row
	printTime();
  }
}

void manualAdjust()
{
	if(buttonMode == 1) // This mode sets the buttons to change the time incrementing hours and minutes up only
	{
	  if(buttonOne.uniquePress())
	  {
		hours++;
		if (hours == 24)
		{
		  hours = 0;
		}
	  }
	  if(buttonTwo.uniquePress())
	  {
		minutes++;
		if (minutes == 60)
		{
		  minutes = 0;
		}
	  }
	}
	else if(buttonMode == 2) // This mode sets the buttons to change the Day up and down
	{
		if(buttonOne.uniquePress())
		{
			day ++;
			if(day == 7)
			{
				day = 0;
			}
		}
		if(buttonTwo.uniquePress())
		{
			day --;
			if(day == -1)
			{
				day = 6
			}
		}
	}
	else if(buttonMode == 3) // This mode sets the buttons to change the Date Up and down
	{
		if(buttonOne.uniquePress())
		{
			date ++;
			checkDate();
		}
		if(buttonTwo.uniquePress())
		{
			date --;
			checkDate();
		}
	}
	else if(buttonMode == 4) // This mode sets the buttons to increment month and year up 
	{
		if(buttonOne.uniquePress())
		{
			month ++;
			checkDate();
		}
		if(buttonTwo.uniquePress())
		{
			year ++;
			checkDate();
		}
	}
}

void setBacklight(byte brightness)
{
  lcd.write(0x80);  // send the backlight command
  lcd.write(brightness);  // send the brightness value
}

void clearDisplay()
{
  lcd.write(0xFE);  // send the special command
  lcd.write(0x01);  // send the clear screen command
}

void setLCDCursor(byte cursor_position)
{
  lcd.write(0xFE);  // send the special command
  lcd.write(0x80);  // send the set cursor command
  lcd.write(cursor_position);  // send the cursor position
}

void printDay()
{
  switch(day)
  {
    case 0:
      lcd.print("Sun.");
      break;
    case 1:
      lcd.print("Mon.");
      break;
    case 2:
      lcd.print("Tue.");
      break;
    case 3:
      lcd.print("Wed.");
      break;
    case 4:
      lcd.print("Thur.");
      break;
    case 5:
      lcd.print("Fri.");
      break;
    case 6:
      lcd.print("Sat.");
      break;
  }
}
      
void checkTime()
{
  seconds++;  // increase seconds
  if (seconds == 60)  // If it's been a minute
  {
    seconds = 0;  // start over seconds
    minutes++;  // Increase minutes
    if (minutes == 60)  // If it's been an hour
    {
      minutes = 0;  // start over minutes
      hours++;  // increase hours
      if (hours == 24)  // If it's been a day
      {
        hours = 0;  // start the day over
        day++;  // increase the day
        if (day == 7)  // if it's been a week
          day = 0;  // start the week over
        date++;  // increase the date
        checkDate();  // this function increases the date/month/year if necessary
      }
    }
  }
}

void checkDate()
{
  // 30 days has sept. apr. jun. and nov.
  if (((month == 9)||(month == 4)||(month == 6)||(month == 11))&&
      (date > 30))
  {
    date = 1;
    month++;
  }
  else if ((month == 2)&&(date > 28))
  {
    date = 1;
    month++;
  }
  else if (date > 31)
  {
    date = 1;
    month++;
    if (month > 12)
    {
      month = 1;
      year++;  // happy new year!
      clearDisplay();
      lcd.print("Happy New Year!");
      delay(5000);
      seconds+=5;
    }
  }
}

void printTime()
{
    lcd.print(hours);  // print the time:
    lcd.print(":");
	if (minutes < 10)
	{
	  lcd.print("0");
	}
	lcd.print(minutes);
    lcd.print(":");
	if (seconds < 10)
	{
	  lcd.print("0");
	}
	lcd.print(seconds);
}

void printDate()
{
    lcd.print(" ");
    lcd.print(month);  // print the date:
    lcd.print("/");
    lcd.print(date);
    lcd.print("/");
    lcd.print(year); 
}


// Function for setting backlight brightness based on time of day
void timeBrightness()
{
  if ((hours >= 0 && hours <= 3) || (hours >= 21 && hours <= 24))  //Dark
    {
      setBacklight(64);
    }
    else if ((hours > 3 && hours <= 6) || (hours >= 18 && hours < 21))  //Semi-dark
    { 
      setBacklight(127);
    }
    else if ((hours > 6 && hours <= 9) || (hours >= 15 && hours < 18))  //Semi-bright
    {
      setBacklight(191);
    }
    else //(hours > 9 && hours < 15)  //Bright
    {
      setBacklight(255);
    }
  }

// Functionsfor button mode switch states. Using a four DIP switch package
// Switch 1 to Arduino port 4,5,6,9
void modeSwitch()
{
	// Setting swith states booleans
	if(modeSwitchA.isPressed())
	{
		s1 = True;
		else
		{
			s1 = False;
		}
	}
	
	if(modeSwitchB.isPressed())
	{
		s2 = True;
		else
		{
			s2 = False;
		}
	}
	
	if(modeSwitchC.isPressed())
	{
		s3 = True;
		else
		{
			s3 = False;
		}
	}
	
	if(modeSwitchD.isPressed())
	{
		s4 = True;
		else
		{
			s4 = False;
		}
	}
	
	// setting buttonMode variable
	
	if(s1)
	{
	
	}
	
}