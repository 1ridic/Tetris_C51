#ifndef _MENU_H_
#define _MENU_H_
#include "button.h"

typedef struct _menu
{
	struct _menu* n;
  void(*f)(struct _menu* this);
}Menu;

void menuInit();

#endif