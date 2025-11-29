//KaitMenu basic demo, Wire and hd44780 libraries required in sketch.
//This example shows how to create a menu with 4 void functions and one submenu entry, enjoy. :-)
#include <Arduino.h>
#include <AmiraEncoder.h>
#include <Wire.h>                                                           //REQUIRED!
#include <ezButton.h>
#include <hd44780.h>                                                        //REQUIRED!
#include <hd44780ioClass/hd44780_I2Cexp.h>                                  //REQUIRED!
#include <KaitMenu.h>                                                       //REQUIRED!
//-------------------------------------------------PIN DEFINITION-----------------------------------------------
#define ENTER 3               //Switch Enter. 
#define DT 4                  //Data encoder.
#define CLK 5                 //Clock encoder.
#define EXIT 11               //Switch Exit.
////////////////////FUNCTIONS DEFINITION//////////////////////
void ledOn();
void ledOff();
void ledBright();
void info();
////////////////////OBJECTS DEFINITION//////////////////////
ezButton enterButton(ENTER, EXTERNAL_PULLUP);                                //Set object ENTER switch as input with external pullup resistor.
ezButton exitButton(EXIT, EXTERNAL_PULLUP);                                  //Set object EXIT switch as input with external pullup resistor.
hd44780_I2Cexp lcd;                                                          //Set object LCD with address auto-detect.
Encoder myEncoder(DT, CLK, EXTERNAL);                                        //Set object for rotary encoder with external pullup resistors and no sensitivity.
Menu::MenuItem controllerItems[] = {                                         //Defines array for controllerItems.
///TITLE/////DESCRIPTION////FUNCTION//SUBMENU
  {"LED ON", "Turns led on.", ledOn, nullptr},                               //This is a function entry.
  {"LED OFF", "Turns led off.", ledOff, nullptr},                            //This is a function entry.
};
Menu ledSubmenu(controllerItems, 2, &lcd);                //Submenu object constructor: defines the object for controllerItems (2 is the number of elements in array).
Menu::MenuItem mainItems[] = {                                               //Defines array for mainItems.
////////TITLE////////DESCRIPTION///FUNCTION/////SUBMENU
  {"LED CONTROLLER", "on/off LED", nullptr, &ledSubmenu},                    //This is a submenu entry.
  {"LED BRIGHT", "Trim bright", ledBright, nullptr},                         //This is a function entry.
  {"INFO", "Show info", info, nullptr},                                      //This is a function entry.
};
Menu mainMenu(mainItems, 3, &lcd);                        //Main menu object constructor: defines the object for mainItems (3 is the number of elements in array).
Menu* currentMenu = &mainMenu;                                               //Keeps track of which menu is currently displayed/active.
//IMPORTANT:
//1) remember that constructor must be written after his array;
//2) submenu array and submenu constructor must be written before main menu array and constructor.
/////////////////////ARDUINO FUNCTIONS/////////////////////
void setup() {
  lcd.begin(16, 2);                                                          //Initialize the LCD display with 16 columns and 2 rows.
  lcd.setBacklight(255);                                                     //Turn on lcd backlight.
  currentMenu->paging(0);                                                    //Print first main menu page.
}

void loop() {
  int cursor = 0;
  int prevCursor = 0;
  int menuMaxI = currentMenu->getMenuSize()-1;           //First menu array element has always 0 index so -1 is necessary to define right menu max index.
  while(true){
    cursor = myEncoder.loop(currentMenu->getCurrentIndex()); //Check if someone is touching encoder and keep updated encoder cursor with menu current index.
    if(cursor < 0){
      cursor = 0;
    } else if(cursor > menuMaxI){
      cursor = menuMaxI;
    }
    if(prevCursor != cursor) {
      currentMenu->paging(cursor);
      prevCursor = cursor;
    }
    enterButton.loop();
    if (enterButton.isReleased()) {                                     //If Enter button is pressed, enter the option.
      currentMenu = currentMenu->enter(cursor);
    }
    exitButton.loop();
    if (exitButton.isReleased()) {
      currentMenu = currentMenu->exit();
    }
  }
}
////////////////////MENU FUNCTIONS//////////////////////
void ledOn(){
  lcd.clear();
  lcd.home();
  lcd.print("Now led is on!");
  lcd.setCursor(0, 1);
  lcd.print("press EXIT");
  while(true){
    exitButton.loop();
    if (exitButton.isReleased()) {                                     //If Enter button is pressed, enter the option.
      currentMenu = currentMenu->exit();
      return;
    }
  }
}

void ledOff(){
  lcd.clear();
  lcd.home();
  lcd.print("Now led is off!");
  lcd.setCursor(0, 1);
  lcd.print("press EXIT");
  while(true){
    exitButton.loop();
    if (exitButton.isReleased()) {                                     //If Enter button is pressed, enter the option.
      currentMenu = currentMenu->exit();
      return;
    }
  }
}

void ledBright(){
  lcd.clear();
  lcd.home();
  lcd.print("Do something or ");
  lcd.setCursor(0, 1);
  lcd.print("press EXIT.");
  while(true){
    exitButton.loop();
    if (exitButton.isReleased()) {                                     //If Enter button is pressed, enter the option.
      currentMenu = currentMenu->exit();
      return;
    }
  }
}

void info(){
  lcd.clear();
  lcd.home();
  lcd.print("This is for");
  lcd.setCursor(0, 1);
  lcd.print("TruKait.");
  delay(4000);
  currentMenu = currentMenu->exit();
  return;
}
///////////////////////////////////////////////////////////
