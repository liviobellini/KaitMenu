#include "KaitMenu.h"

Menu::Menu(Menu::MenuItem* _items, int _size, hd44780_I2Cexp* _lcd, Menu* _parent) {
  menuItems = _items;
  menuSize = _size;
  lcd = _lcd;
  parent = _parent;
  currentIndex = 0;
  parentIndex = 0;
}

Menu* Menu::enter(int index) {
  if (index < 0 || index >= menuSize) {
    return this;
  }
  currentIndex = index;
  if (menuItems[index].function != nullptr) {
	inFunction = true;
    menuItems[index].function();
	inFunction = false;
    return this;
  } else if (menuItems[index].submenu != nullptr) {
    Menu* sub = menuItems[index].submenu;
    sub->parent = this;
    sub->parentIndex = index;
    sub->paging(0);
    return sub;
  }
  return this;
}

Menu* Menu::exit() {
  if(inFunction || parent == nullptr) {
    paging(currentIndex);
    return this;
  } else if(parent != nullptr) {
    parent->paging(parent->parentIndex);
    return parent;
  }
}

void Menu::paging(int index) {
  if (index < 0 || index >= menuSize) {
    return;
  } 
  currentIndex = index;
  lcd->clear();
  lcd->home();
  lcd->print(menuItems[index].title);
  lcd->setCursor(0, 1);
  lcd->print(menuItems[index].description);
}

int Menu::getMenuSize() {
  return menuSize;
}

int Menu::getCurrentIndex() {
  return currentIndex;
}
