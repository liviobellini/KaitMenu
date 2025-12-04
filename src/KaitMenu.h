#ifndef KAITMENU_H
#define KAITMENU_H

#include <Wire.h>
#include <hd44780.h>
#include <hd44780ioClass/hd44780_I2Cexp.h>

class Menu {
  public:
    struct MenuItem {
      const char* title;
      const char* description;
      void (*function)();
      Menu* submenu;
    };
    Menu(MenuItem* _items, int _size, hd44780_I2Cexp* _lcd, Menu* _parent = nullptr);
    Menu* enter(int index);
    Menu* exit();
    void render(int index);
    int getMenuSize();
    int getCurrentIndex();
  private:
    MenuItem* menuItems;
    int menuSize;
    hd44780_I2Cexp* lcd;
    Menu* parent;
    int currentIndex;
    int parentIndex;
    bool inFunction = false;
};

#endif
