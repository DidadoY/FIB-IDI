#include "LL2GLWidget.h"

#include <iostream>

LL2GLWidget::LL2GLWidget (QWidget* parent) : QOpenGLWidget(parent), program(NULL)
{
  setFocusPolicy(Qt::StrongFocus);  // per rebre events de teclat
}

LL2GLWidget::~LL2GLWidget ()
{
  if (program != NULL)
    delete program;
}

void LL2GLWidget::initializeGL ()
{
  // Cal inicialitzar l'ús de les funcions d'OpenGL
  initializeOpenGLFunctions();  
  glEnable(GL_DEPTH_TEST);
  
  glClearColor(0.5, 0.7, 1.0, 1.0); // defineix color de fons (d'esborrat)
  carregaShaders();
  creaBuffersEscac();
  creaBuffersHomer();
  iniEscena();
  iniCamera();
}

void LL2GLWidget::iniEscena()
{
  angleH     = 0.0;
  xH         = zH  = 0;  // posició x i z final del Homer
}

void LL2GLWidget::iniCamera(){

  obs = glm::vec3(0.5, 2, 12);
  vrp = glm::vec3(0.5, 0, 0.5);
  up = glm::vec3(0, 1, 0);
  fov = float(M_PI)/4.0f;
  ra  = 1.0;
  znear =  6.5;
  zfar  = 15;

  viewTransform();
  projectTransform();
}

void LL2GLWidget::paintGL ()
{
  // descomentar per canviar paràmetres
  // glViewport (0, 0, ample, alt);

  // Esborrem el frame-buffer
  glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // Homer
  homerTransform(xH, zH);
  glBindVertexArray (VAO_Homer);
  glUniform1i(indexColorLoc, 2);  // color = 2 -- color per vèrtex
  glDrawArrays(GL_TRIANGLES, 0, homer.faces().size()*3);
  glBindVertexArray (0);

  // Escac
  escacTransform(0, 0);
  glBindVertexArray (VAO_Escac);
  glUniform1i(indexColorLoc, 1);  // color = 1 -- color escac blanc (0.8, 0.8, 0.8)
  glDrawArrays(GL_TRIANGLES, 0, 6);
  glBindVertexArray (0);
}

void LL2GLWidget::homerTransform (int x, int z) 
{
  // Matriu de transformació de Homer
  glm::mat4 TG(1.0f);
  TG = glm::translate(TG, glm::vec3(x, -0.5, z));
  TG = glm::rotate(TG, angleH, glm::vec3(0, 1, 0));
  TG = glm::scale(TG, glm::vec3(escalaH));
  TG = glm::translate(TG, -baseHomer);
  glUniformMatrix4fv(transLoc, 1, GL_FALSE, &TG[0][0]);
}

void LL2GLWidget::escacTransform (int x, int z)
{
  // Matriu de transformació de l'escac
  glm::mat4 TG(1.0f);
  TG = glm::translate(TG, glm::vec3(x, -0.5, z));
  glUniformMatrix4fv (transLoc, 1, GL_FALSE, &TG[0][0]);
}

void LL2GLWidget::projectTransform ()
{
  glm::mat4 Proj(1.0f);
  Proj = glm::perspective (fov, ra, znear, zfar);
  glUniformMatrix4fv (projLoc, 1, GL_FALSE, &Proj[0][0]);
}

void LL2GLWidget::viewTransform ()
{
  // Matriu de posició i orientació de l'observador
  glm::mat4 View(1.0f);
  View = glm::lookAt (obs, vrp, up);
  glUniformMatrix4fv (viewLoc, 1, GL_FALSE, &View[0][0]);
}


void LL2GLWidget::resizeGL (int w, int h) 
{
// Aquest codi és necessari únicament per a MACs amb pantalla retina.
#ifdef __APPLE__
  GLint vp[4];
  glGetIntegerv (GL_VIEWPORT, vp);
  ample = vp[2];
  alt = vp[3];
#else
  ample = w;
  alt = h;
#endif
  ra = float(ample)/float(alt);
  projectTransform();
}

void LL2GLWidget::creaBuffersEscac ()
{
  // Model de l'escac
  glm::vec3 geomEscac[6] = {
     glm::vec3(-0.5, 0.0, -0.5),
     glm::vec3( 0.5, 0.0, -0.5),
     glm::vec3( 0.5, 0.0,  0.5),
     glm::vec3(-0.5, 0.0, -0.5),
     glm::vec3( 0.5, 0.0,  0.5),
     glm::vec3(-0.5, 0.0,  0.5)
  };

  glm::vec3 blanc(0.8, 0.8, 0.8);
  glm::vec3 colEscac[6] = {
     blanc, blanc, blanc, blanc, blanc, blanc
  };

  // Escac
  glGenVertexArrays(1, &VAO_Escac);
  glBindVertexArray(VAO_Escac);

  GLuint VBO_Escac[2];
  glGenBuffers(2, VBO_Escac);

  // geometria
  glBindBuffer(GL_ARRAY_BUFFER, VBO_Escac[0]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(geomEscac), geomEscac, GL_STATIC_DRAW);
  glVertexAttribPointer(vertexLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(vertexLoc);

  // color
  glBindBuffer(GL_ARRAY_BUFFER, VBO_Escac[1]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(colEscac), colEscac, GL_STATIC_DRAW);
  glVertexAttribPointer(colorLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(colorLoc);
  
  glBindVertexArray (0);
}

void LL2GLWidget::creaBuffersHomer ()
{
  // Càrrega del model
  homer.load("./models/HomerProves.obj");

  // Calculem la capsa contenidora del model
  calculaCapsaModel ();
  
  // Creació de VAOs i VBOs per pintar
  // Homer
  glGenVertexArrays(1, &VAO_Homer);
  glBindVertexArray(VAO_Homer);

  GLuint VBO_Homer[2];
  glGenBuffers(2, VBO_Homer);

  // geometria
  glBindBuffer(GL_ARRAY_BUFFER, VBO_Homer[0]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*homer.faces().size()*3*3,
	       homer.VBO_vertices(), GL_STATIC_DRAW);
  glVertexAttribPointer(vertexLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(vertexLoc);

  // color
  glBindBuffer(GL_ARRAY_BUFFER, VBO_Homer[1]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*homer.faces().size()*3*3,
	       homer.VBO_matdiff(), GL_STATIC_DRAW);
  glVertexAttribPointer(colorLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(colorLoc);
  
  glBindVertexArray (0);
}

void LL2GLWidget::carregaShaders()
{
  // Creem els shaders per al fragment shader i el vertex shader
  QOpenGLShader fs (QOpenGLShader::Fragment, this);
  QOpenGLShader vs (QOpenGLShader::Vertex, this);
  // Carreguem el codi dels fitxers i els compilem
  fs.compileSourceFile("shaders/basicShader.frag");
  vs.compileSourceFile("shaders/basicShader.vert");
  // Creem el program
  program = new QOpenGLShaderProgram(this);
  // Li afegim els shaders corresponents
  program->addShader(&fs);
  program->addShader(&vs);
  // Linkem el program
  program->link();
  // Indiquem que aquest és el program que volem usar
  program->bind();

  // Identificador per als  atributs
  vertexLoc = glGetAttribLocation (program->programId(), "vertex");
  colorLoc = glGetAttribLocation (program->programId(), "color");

  // Identificadors dels uniform locations
  transLoc = glGetUniformLocation(program->programId(), "TG");
  projLoc  = glGetUniformLocation (program->programId(), "Proj");
  viewLoc  = glGetUniformLocation (program->programId(), "View");
  indexColorLoc = glGetUniformLocation (program->programId(), "indexColor");
}

void LL2GLWidget::calculaCapsaModel ()
{
  // Càlcul capsa contenidora i valors transformacions inicials
  float minx, miny, minz, maxx, maxy, maxz;
  minx = maxx = homer.vertices()[0];
  miny = maxy = homer.vertices()[1];
  minz = maxz = homer.vertices()[2];
  for (unsigned int i = 3; i < homer.vertices().size(); i+=3)
  {
    if (homer.vertices()[i+0] < minx)
      minx = homer.vertices()[i+0];
    if (homer.vertices()[i+0] > maxx)
      maxx = homer.vertices()[i+0];
    if (homer.vertices()[i+1] < miny)
      miny = homer.vertices()[i+1];
    if (homer.vertices()[i+1] > maxy)
      maxy = homer.vertices()[i+1];
    if (homer.vertices()[i+2] < minz)
      minz = homer.vertices()[i+2];
    if (homer.vertices()[i+2] > maxz)
      maxz = homer.vertices()[i+2];
  }
  escalaH        = 1.0/(maxy-miny);
  baseHomer[0]   = (maxx+minx)/2.0;
  baseHomer[1]   = miny;
  baseHomer[2]   = (maxz+minz)/2.0;
}

void LL2GLWidget::keyPressEvent(QKeyEvent* event) 
{
  makeCurrent();
  switch (event->key()) {
    default: event->ignore(); break;
  }
  update();
}

