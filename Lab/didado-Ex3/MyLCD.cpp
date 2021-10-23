#include "MyLCD.h"

MyLCD::MyLCD(QWidget *parent=0):QLCDNumber(parent)
{
	num = 0;
}

void MyLCD::incrementar(){
	if (num < 77) ++num;
	else num = 0;
  display(num);
}

void MyLCD::resetear(){
	num = 0;
	display(num);
}
