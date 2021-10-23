
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

MyGLWidget::~MyGLWidget() {}


void MyGLWidget::initializeGL ()
{
  eixX = glm::vec3(1, 0, 0);
  eixY = glm::vec3(0, 1, 0);


  // Cal inicialitzar l'ús de les funcions d'OpenGL
  initializeOpenGLFunctions();
  glEnable(GL_DEPTH_TEST);

  glClearColor(0.5, 0.7, 1.0, 1.0); // defineix color de fons (d'esborrat)


  b = 0;
  glUniform1i(boolLoc, b);
  carregaShaders();
  iniEscena();
  LL4GLWidget::iniCamera();

}

void MyGLWidget:: iniEscena(){
  LL4GLWidget::iniEscena();
  posFocus1 = glm::vec3(-3.5, 10.0, -3.5);   // coordenades d'escena
  colFocus1 = glm::vec3(0.9,0.2,0.2);

  posFocus2 = glm::vec3(3.5,10.0, -3.5);   // coordenades d'escena
  colFocus2 = glm::vec3(0.0, 1.0, 0.0);

  posFocus3 = glm::vec3(3.5,10.0, 3.5);   // coordenades d'escena
  colFocus3 = glm::vec3(0.2, 0.2, 0.9);
}

void MyGLWidget::cotxeTransform ()
{
  glm::mat4 TG(1.0f);
  TG = glm::translate(TG, centreEscena);
  TG = glm::rotate(TG, angleCotxe, eixY);
  TG = glm::scale(TG, glm::vec3(escalaCotxe));
  TG = glm::translate(TG, -baseCotxe);
  glUniformMatrix4fv(transLoc, 1, GL_FALSE, &TG[0][0]);
}



void MyGLWidget::setupLights() {
  LL4GLWidget::setupLights();
  glm::vec3 aux1 = glm::vec3((glm::vec4(posFocus1, 1.0f)));
  glUniform3fv(posFocusLoc1, 1, &aux1[0]);
  glUniform3fv(colFocusLoc1, 1, &colFocus1[0]);

  glm::vec3 aux2 = glm::vec3((glm::vec4(posFocus2, 1.0f)));
  glUniform3fv(posFocusLoc2, 1, &aux2[0]);
  glUniform3fv(colFocusLoc2, 1, &colFocus2[0]);

  glm::vec3 aux3 = glm::vec3((glm::vec4(posFocus3, 1.0f)));
  glUniform3fv(posFocusLoc3, 1, &aux3[0]);
  glUniform3fv(colFocusLoc3, 1, &colFocus3[0]);
}

void MyGLWidget::carregaShaders()
{
  LL4GLWidget::carregaShaders();
  colFocusLoc1 = glGetUniformLocation (program->programId(), "colFocus1");
  posFocusLoc1 = glGetUniformLocation (program->programId(), "posFocus1");
  colFocusLoc2 = glGetUniformLocation (program->programId(), "colFocus2");
  posFocusLoc2 = glGetUniformLocation (program->programId(), "posFocus2");
  colFocusLoc3 = glGetUniformLocation (program->programId(), "colFocus3");
  posFocusLoc3 = glGetUniformLocation (program->programId(), "posFocus3");
  boolLoc = glGetUniformLocation (program->programId(), "b");
}

void MyGLWidget::semueve(int i){
  angleCotxe = float(180*i)/M_PI;
  cotxeTransform();
  update();
}

void MyGLWidget::keyPressEvent(QKeyEvent *e) {
    switch (e->key()) {

      case Qt::Key_Right:
        angleCotxe += M_PI/4;
        cotxeTransform();
        break;

      case Qt::Key_Left:
        angleCotxe -= M_PI/4;
        cotxeTransform();
        break;

      case Qt::Key_F:

        if (b == 0) b = 1;
        else b = 0;
        setupLights();
        glUniform1i(boolLoc, b);

        break;

      case Qt::Key_R:
        angleCotxe = M_PI/4;
        b = 0;
        setupLights();
        glUniform1i(boolLoc, b);
        break;

        default:
            LL4GLWidget::keyPressEvent(e);
    }

    makeCurrent();
    update();
}
