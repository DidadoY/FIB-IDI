// MyGLWidget.cpp
#include "MyGLWidget.h"
#include <iostream>
#include <stdio.h>

#define printOpenGLError() printOglError(__FILE__, __LINE__)
#define CHECK() printOglError(__FILE__, __LINE__,__FUNCTION__)
#define DEBUG() std::cout << __FILE__ << " " << __LINE__ << " " << __FUNCTION__ << std::endl;

int MyGLWidget::printOglError(const char file[], int line, const char func[])
{
    GLenum glErr;
    int    retCode = 0;

    glErr = glGetError();
    const char * error = 0;
    switch (glErr)
    {
        case 0x0500:
            error = "GL_INVALID_ENUM";
            break;
        case 0x501:
            error = "GL_INVALID_VALUE";
            break;
        case 0x502:
            error = "GL_INVALID_OPERATION";
            break;
        case 0x503:
            error = "GL_STACK_OVERFLOW";
            break;
        case 0x504:
            error = "GL_STACK_UNDERFLOW";
            break;
        case 0x505:
            error = "GL_OUT_OF_MEMORY";
            break;
        default:
            error = "unknown error!";
    }
    if (glErr != GL_NO_ERROR)
    {
        printf("glError in file %s @ line %d: %s function: %s\n",
                             file, line, error, func);
        retCode = 1;
    }
    return retCode;
}

MyGLWidget::~MyGLWidget() {
}

void MyGLWidget::initializeGL ()
{
  ExamGLWidget::initializeGL();
  activado = true;
  angle2 = 0;
  anglecub = 0;
  groc = false;
}


void MyGLWidget::paintGL ()   // Mètode que has de modificar
{

  glBindVertexArray (VAO_Terra);
  modelTransformTerra ();
  glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

  if(activado){
  glBindVertexArray(VAO_Cub);
  modelTransformCub (4.0, anglecub);
  glDrawArrays(GL_TRIANGLES, 0, 36);

  glBindVertexArray(VAO_Cub);
  modelTransformCub (5.0, anglecub+2*M_PI/3);
  glDrawArrays(GL_TRIANGLES, 0, 36);

  glBindVertexArray(VAO_Cub);
  modelTransformCub (6.0, anglecub+4*M_PI/3);
  glDrawArrays(GL_TRIANGLES, 0, 36);
}

  glBindVertexArray (VAO_Patr);
  modelTransformPatricio (angle2);
  glDrawArrays(GL_TRIANGLES, 0, patr.faces().size()*3);
}

void MyGLWidget::modelTransformCub (float escala, float angle)
{
  TG = glm::mat4(1.f);
  TG = glm::rotate(TG, angle, glm::vec3(0, 1, 0));
  TG = glm::translate(TG, glm::vec3(5, 0, 0));
  TG = glm::scale(TG, glm::vec3 (escala, escala, escala));
  glUniformMatrix4fv (transLoc, 1, GL_FALSE, &TG[0][0]);

  // En aquest mètode has de substituir aquest codi per construir la
  // transformació geomètrica (TG) del cub usant els paràmetres adientment
}

void MyGLWidget::modelTransformPatricio (float angle2)    // Mètode que has de modificar
{
  float angle = -M_PI/2;
  TG = glm::mat4(1.f);
  TG = glm::rotate(TG, angle2, glm::vec3(0, 1, 0));
  TG = glm::translate(TG, glm::vec3(5, 0, 0));
  TG = glm::rotate(TG, angle, glm::vec3(0, 1, 0));
  TG = glm::scale(TG, glm::vec3 (2*escala, 2*escala, 2*escala));
  TG = glm::translate(TG, -centreBasePat);

  glUniformMatrix4fv (transLoc, 1, GL_FALSE, &TG[0][0]);
}

void MyGLWidget::viewTransform ()    // Mètode que has de modificar
{
  if (!camPlanta)
    ExamGLWidget::viewTransform();
  else
  {
    View = glm::translate(glm::mat4(1.f), glm::vec3(0, 0, -2*radiEsc));
    View = glm::rotate(View, float(M_PI/2), glm::vec3(1, 0, 0));
    View = glm::rotate(View, float(M_PI/2), glm::vec3(0, 1, 0));
    View = glm::translate(View, -centreEsc);

    glUniformMatrix4fv (viewLoc, 1, GL_FALSE, &View[0][0]);
  }
}

void MyGLWidget::projectTransform ()
{
  if (!camPlanta)
    ExamGLWidget::projectTransform();
  else
  {
    glm::mat4 Proj;  // Matriu de projecció
    Proj = glm::ortho(-radiEsc, radiEsc, -radiEsc, radiEsc, zn, zf);

    glUniformMatrix4fv (projLoc, 1, GL_FALSE, &Proj[0][0]);
  }
}

void MyGLWidget::camara () {
  if (camPlanta) camPlanta = false;
  else camPlanta = true;
  viewTransform();
  projectTransform();
}

void MyGLWidget::uno() {
  angle2 = anglecub;
}

void MyGLWidget::dos() {
  angle2 = anglecub+2*M_PI/3;
}

void MyGLWidget::tres() {
  angle2 = anglecub+4*M_PI/3;
}

void MyGLWidget::keyPressEvent(QKeyEvent* event) {
  makeCurrent();
  switch (event->key()) {
  case Qt::Key_V: {
      if(activado) activado = false;
      else activado = true;
    break;
	}
  case Qt::Key_1: {
    angle2 = anglecub;

    break;
	}
  case Qt::Key_2: {
    angle2 = anglecub+2*M_PI/3;

    break;
	}
  case Qt::Key_3: {
    angle2 = anglecub+4*M_PI/3;

    break;
	}
  case Qt::Key_F: {
      if (groc){
        colFoc = glm::vec3(1,1,1);
        groc = false;
      }
      else {
        colFoc = glm::vec3(1,1,0);
        groc = true;
      }

      ExamGLWidget::enviaColFocus();
    break;
	}
  case Qt::Key_C: {
      if (camPlanta) camPlanta = false;
      else camPlanta = true;
      viewTransform();
      projectTransform();
    break;
	}
  case Qt::Key_Right: {
      anglecub += float(2*M_PI/3);
      angle2 += float(2*M_PI/3);
      paintGL();
    break;
	}
  case Qt::Key_Left: {
      anglecub -= float(2*M_PI/3);
      angle2 -= float(2*M_PI/3);
      paintGL();
    break;
	}
  case Qt::Key_R: {
      initializeGL();
    break;
	}
  default: ExamGLWidget::keyPressEvent(event); break;
  }
  update();
}
