#include "labels.h"


labels::labels(QWidget *parent=0):QLabel(parent)
{
	num = 0;
  setNum(num);
  setStyleSheet("background-color: green;");
}

void labels::incrementar(){

  if (num < 77) ++num;
  else num = 0;
  unitats = num%10;
  desenes = (num/10)%10;

  //unitats
  if (objectName() == "label_6"){
      setNum(unitats%2);
      if (unitats%2 == 0) setStyleSheet("background-color: green;");
      else setStyleSheet("background-color: yellow;");
  }

  if (objectName() == "label_5"){
      setNum((unitats/2)%2);
      if ((unitats/2)%2 == 0) setStyleSheet("background-color: green;");
      else setStyleSheet("background-color: yellow;");
  }

  if (objectName() == "label_4"){
      setNum((unitats/4)%2);
      if ((unitats/4)%2 == 0) setStyleSheet("background-color: green;");
      else setStyleSheet("background-color: yellow;");
  }

  if (objectName() == "label_3"){
      setNum(desenes%2);
      if (desenes%2 == 0) setStyleSheet("background-color: green;");
      else setStyleSheet("background-color: yellow;");
  }

  if (objectName() == "label_2"){
      setNum((desenes/2)%2);
      if ((desenes/2)%2 == 0) setStyleSheet("background-color: green;");
      else setStyleSheet("background-color: yellow;");
  }

  if (objectName() == "label"){
      setNum((desenes/4)%2);
      if ((desenes/4)%2 == 0) setStyleSheet("background-color: green;");
      else setStyleSheet("background-color: yellow;");
  }
}

void labels::resetear(){
	num = 0;
  setNum(num);
  setStyleSheet("background-color: green;");
}
