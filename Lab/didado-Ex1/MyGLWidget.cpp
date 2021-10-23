#include "MyGLWidget.h"

#include <QOpenGLShader>
#include "glm/gtc/matrix_transform.hpp"
#include <iostream>

MyGLWidget::MyGLWidget (QWidget* parent) : QOpenGLWidget(parent), program(NULL)
{
  setFocusPolicy(Qt::StrongFocus);  // per rebre events de teclat
}

MyGLWidget::~MyGLWidget ()
{
  if (program != NULL)
    delete program;
}

void MyGLWidget::initializeGL ()
{
  // Cal inicialitzar l'ús de les funcions d'OpenGL
  initializeOpenGLFunctions();

  glClearColor(0.5, 0.7, 1.0, 1.0); // defineix color de fons (d'esborrat)

  valorsInicials();
  carregaShaders();
  creaBuffers();
}

void MyGLWidget::valorsInicials()
{
  groc  = glm::vec3(1.0, 1.0, 0.0);
  sentido = 0;
  iRoig = 1;
  iBlau = 2;
  iVerd = 3;
  eixZ    = glm::vec3(0.0, 0.0, 1.0);

  deltaTriangle    = 6;            // increment discretit. cercles

  centrePinyo     = glm::vec3(0, -0.2, 0);    // centre
  rPinyo          = .15;                      // radi
  anglePinyo      = 0;                        // angle de gir
  deltaAnglePinyo = 6;                        // increment rotacio en graus sexa

  centreEngraA     = glm::vec3(-0.3, 0.2, 0);    // centre
  rEngraA          = .35;                      // radi
  angleEngraA      = 0;                        // angle de gir

  centreEngraB     = glm::vec3(0.4, -0.2, 0);    // centre
  rEngraB           = .25;                      // radi
  angleEngraB     = 0;                        // angle de gir
}

void MyGLWidget::pintaEngranatge(glm::vec3 centre, float angleRot, float escala, int color)
{
  float alfa;

  glUniform1i(canviLoc, 0);
  for (int angle = 0; angle < 360; angle += deltaTriangle){
    alfa = -angle*float(M_PI)/180;
    modelTransform (centre, alfa, escala);
    glDrawArrays(GL_TRIANGLES, 0, 3);
  }
  glUniform1i(canviLoc, color);
  alfa = -angleRot * float(M_PI)/180;
  modelTransform (centre, alfa, escala);
  glDrawArrays(GL_TRIANGLES, 0, 3);
}

void MyGLWidget::paintGL ()
{
// Aquest codi és necessari únicament per a MACs amb pantalla retina.
#ifdef __APPLE__
  GLint vp[4];
  glGetIntegerv (GL_VIEWPORT, vp);
  ample = vp[2];
  alt = vp[3];
#endif

// En cas de voler canviar els paràmetres del viewport, descomenteu la crida següent i
// useu els paràmetres que considereu (els que hi ha són els de per defecte)
//  glViewport (0, 0, ample, alt);

  glClear (GL_COLOR_BUFFER_BIT);  // Esborrem el frame-buffer

  // Activem l'Array a pintar
  glBindVertexArray (VAO);

  // Dibuix del pinyó
  pintaEngranatge(centrePinyo, anglePinyo, rPinyo, iRoig);
  pintaEngranatge(centreEngraA, angleEngraA, rEngraA, iVerd);
  pintaEngranatge(centreEngraB, angleEngraB, rEngraB, iBlau);
  // Desactivem el VAO
  glBindVertexArray (0);
}

void MyGLWidget::modelTransform(glm::vec3 centre, float angle, float escala)
{
  // Matriu de transformació de model
  glm::mat4 transform (1.0f);
  transform = glm::translate(transform, centre);
  transform = glm::scale(transform, glm::vec3(escala, escala, 1));
  transform = glm::rotate(transform, -angle, eixZ);

  glUniformMatrix4fv(transLoc, 1, GL_FALSE, &transform[0][0]);
}

void MyGLWidget::resizeGL (int w, int h)
{
  ample = w;
  alt = h;
}

void MyGLWidget::keyPressEvent(QKeyEvent* event)
{
  makeCurrent();
  switch (event->key()) {
    case Qt::Key_M:

      if (sentido == 0){
        anglePinyo += deltaAnglePinyo;
        angleEngraB -= (deltaAnglePinyo*rPinyo)/rEngraB;
        angleEngraA -= (deltaAnglePinyo*rPinyo)/rEngraA;
      }

      else {
        anglePinyo -= deltaAnglePinyo;
        angleEngraB += (deltaAnglePinyo*rPinyo)/rEngraB;
        angleEngraA += (deltaAnglePinyo*rPinyo)/rEngraA;
      }

      modelTransform(centrePinyo, anglePinyo, escalaPinyo);
      modelTransform(centreEngraA, angleEngraA, escalaEngraA);
      modelTransform(centreEngraB, angleEngraB, escalaEngraB);
      break;

    case Qt::Key_I:
      if (sentido == 1) sentido = 0;
      else sentido = 1;
      break;

    case Qt::Key_R:
      anglePinyo = 0;
      angleEngraA = 0;
      angleEngraB = 0;
      modelTransform(centrePinyo, anglePinyo, escalaPinyo);
      modelTransform(centreEngraA, angleEngraA, escalaEngraA);
      modelTransform(centreEngraB, angleEngraB, escalaEngraB);

      break;

    default:
      event->ignore();
      break;
  }
  update();
}

void MyGLWidget::creaBuffers ()
{
  // Triangle per a la discretització del cercle de radi unitari

  float cosDelta = cos(deltaTriangle * M_PI/180.0);
  float sinDelta = sin(deltaTriangle * M_PI/180.0);

  // engranatge
  glm::vec3 triangle[3] = {
			   glm::vec3(0.0, 0.0, 0.0),
			   glm::vec3(1.0, 0.0, 0.0),
			   glm::vec3(cosDelta, sinDelta, 0.0)
  };
  glm::vec3 triangleColor[3] = {
    groc, groc, groc
  };

  // Creació del VAO del triangle
  glGenVertexArrays(1, &VAO);
  glBindVertexArray(VAO);

  // Creació dels Vertex Buffers
  GLuint VBO[2];
  glGenBuffers(2, &VBO[0]);

  // Buffer de geometria
  glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(triangle), triangle, GL_STATIC_DRAW);
  glVertexAttribPointer(vertexLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(vertexLoc);

  // Buffer de color
  glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(triangleColor), triangleColor, GL_STATIC_DRAW);
  glVertexAttribPointer(colorLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(colorLoc);

  // Desactivem el VAO
   glBindVertexArray(0);
}


void MyGLWidget::carregaShaders()
{
  // Creem els shaders per al fragment shader i el vertex shader
  QOpenGLShader fs (QOpenGLShader::Fragment, this);
  QOpenGLShader vs (QOpenGLShader::Vertex, this);
  // Carreguem el codi dels fitxers i els compilem
  fs.compileSourceFile("shaders/basicShader.frag");
  vs.compileSourceFile("shaders/basicShader.vert");
  // Creem el program
  program = new QOpenGLShaderProgram(this);
  // Li fegim els shaders corresponents
  program->addShader(&fs);
  program->addShader(&vs);
  // Linkem el program
  program->link();
  // Indiquem que aquest és el program que volem usar
  program->bind();

  // Identificadors pels atributs
  vertexLoc = glGetAttribLocation(program->programId(), "vertex");
  colorLoc  = glGetAttribLocation(program->programId(), "color");

  // Uniform locations
  transLoc = glGetUniformLocation(program->programId(), "TG");
  canviLoc = glGetUniformLocation(program->programId(), "canviColor");
}
