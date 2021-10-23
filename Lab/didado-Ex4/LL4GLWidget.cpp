#include "LL4GLWidget.h"
#include <iostream>

LL4GLWidget::LL4GLWidget (QWidget* parent) : QOpenGLWidget(parent), program(NULL)
{
  setFocusPolicy(Qt::StrongFocus);  // per rebre events de teclat
}

LL4GLWidget::~LL4GLWidget ()
{
  if (program != NULL)
    delete program;
}

void LL4GLWidget::initializeGL ()
{
  eixX = glm::vec3(1, 0, 0);
  eixY = glm::vec3(0, 1, 0);


  // Cal inicialitzar l'ús de les funcions d'OpenGL
  initializeOpenGLFunctions();  
  glEnable(GL_DEPTH_TEST);
  
  glClearColor(0.5, 0.7, 1.0, 1.0); // defineix color de fons (d'esborrat)
  carregaShaders();
  iniEscena();
  iniCamera();
}

void LL4GLWidget::iniEscena()
{
  creaBuffersTerra();
  creaBuffersModel(cotxe, "models/porsche.obj", &VAO_Cotxe);
  creaBuffersModel(lego,  "models/legoman.obj", &VAO_Lego );
  calculaCapsaModel (cotxe, &escalaCotxe, baseCotxe);
  escalaCotxe    = 1.75 * escalaCotxe;
  calculaCapsaModel (lego, &escalaLego, baseLego);
  escalaLego = 2 * escalaLego;
  
  radiEscena   = sqrt(132)/2;
  centreEscena = glm::vec3(0.0, 1.0, 0.0);

  posLego    = glm::vec3(-3.50, 0.0, 3.50); //coordenades d'escena
  angleCotxe = float(M_PI)/4.0f; 
  angleLego  = 3*float(M_PI)/4.0f;

  posFocusZenital = glm::vec3(0, 10, 10);   // coordenades d'escena
  colFocusZenital = glm::vec3(1, 1, 1);

  xClick = yClick  = 0.0;
  DoingInteractive = NONE;
}

void LL4GLWidget::iniCamera()
{
  vrp = centreEscena;
  vup = glm::vec3(0, 1, 0);
  fov = float(M_PI)/3.0;
  ra  = 1.0;
  znear =  radiEscena;
  zfar  = 3 * radiEscena;

  viewTransform();
  projectTransform();
}

void LL4GLWidget::paintGL ()
{
  glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  // Terra
  terraTransform();
  glBindVertexArray (VAO_Terra);
  glDrawArrays(GL_TRIANGLES, 0, 6);
  
  // Cotxe
  cotxeTransform();
  glBindVertexArray (VAO_Cotxe);
  glDrawArrays(GL_TRIANGLES, 0, cotxe.faces().size()*3);

  // Lego
  legoTransform();
  glBindVertexArray (VAO_Lego);
  glDrawArrays(GL_TRIANGLES, 0, lego.faces().size()*3);

  glBindVertexArray (0);
}

void LL4GLWidget::terraTransform ()
{
  glm::mat4 TG(1.0f);
  TG = glm::translate(TG, centreEscena);
  //TG = glm::scale(TG, glm::vec3(escalaTerra, 1.0, escalaTerra));
  glUniformMatrix4fv (transLoc, 1, GL_FALSE, &TG[0][0]);
}

void LL4GLWidget::cotxeTransform () 
{
  glm::mat4 TG(1.0f);
  TG = glm::translate(TG, centreEscena);
  TG = glm::rotate(TG, angleCotxe, eixY);
  TG = glm::scale(TG, glm::vec3(escalaCotxe));
  TG = glm::translate(TG, -baseCotxe);
  glUniformMatrix4fv(transLoc, 1, GL_FALSE, &TG[0][0]);
}

void LL4GLWidget::legoTransform () 
{
  glm::mat4 TG(1.0f);
  TG = glm::translate(TG, posLego);
  TG = glm::translate(TG, centreEscena);
  TG = glm::rotate(TG, angleLego, eixY);
  TG = glm::scale(TG, glm::vec3(escalaLego));
  TG = glm::translate(TG, -baseLego);
  glUniformMatrix4fv(transLoc, 1, GL_FALSE, &TG[0][0]);
}

void LL4GLWidget::viewTransform ()
{
  switch (vista) {
    case 0:
    obs = vrp + 2*radiEscena* glm::normalize(glm::vec3(0.2,0.4,1.0)); 
    break;
    case 1:
    obs = vrp + 2*radiEscena* glm::normalize(glm::vec3(-4.,10.,4.));
    break;
    case 2:
    obs = vrp + 2*radiEscena* glm::normalize(glm::vec3(0.,10.,0.5));
    break;
    default: 
    obs = vrp + 2*radiEscena* glm::normalize(glm::vec3(0.,0.,1.0));
  }
  View = glm::lookAt(obs, vrp, vup);
  glUniformMatrix4fv(viewLoc, 1, GL_FALSE, &View[0][0]);

  setupLights();
}

void LL4GLWidget::setupLights() {
  makeCurrent();
  glm::vec3 aux = glm::vec3((View * glm::vec4(posFocusZenital, 1.0f)));
  glUniform3fv(posFocusLoc, 1, &aux[0]);
  glUniform3fv(colFocusLoc, 1, &colFocusZenital[0]);
}

void LL4GLWidget::projectTransform ()
{
  glm::mat4 Proj(1.0f);
  Proj = glm::perspective (fov, ra, znear, zfar);
  glUniformMatrix4fv (projLoc, 1, GL_FALSE, &Proj[0][0]);
}

void LL4GLWidget::resizeGL (int w, int h) 
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
}

void LL4GLWidget::creaBuffersTerra ()
{
  // Model del terra
  glm::vec3 geomTerra[6] = {
     glm::vec3(-4.0, 0.0, -4.0),
     glm::vec3( 4.0, 0.0, -4.0),
     glm::vec3( 4.0, 0.0,  4.0),
     glm::vec3(-4.0, 0.0, -4.0),
     glm::vec3( 4.0, 0.0,  4.0),
     glm::vec3(-4.0, 0.0,  4.0)
  };

  // VBO amb la normal de cada vèrtex
  glm::vec3 normTerra[6] = {
    eixY, eixY, eixY, eixY, eixY, eixY 
  };

  // Definim el material de l'escenari
  glm::vec3 amb (0.1, 0.5, 0.0);
  glm::vec3 diff(0.1, 0.6, 0.0);
  glm::vec3 spec(0.5, 0.5, 0.5);
  float shin = 100;

  // Tots els vèrtexs tenen el mateix material
  glm::vec3 matamb[6] = {
    amb, amb, amb, amb, amb, amb
  };
  glm::vec3 matdiff[6] = {
    diff, diff, diff, diff, diff, diff
  };
  glm::vec3 matspec[6] = {
    spec, spec, spec, spec, spec, spec
  };
  float matshin[6] = {
    shin, shin, shin, shin, shin, shin
  };
  
  glGenVertexArrays(1, &VAO_Terra);
  glBindVertexArray(VAO_Terra);

  GLuint VBO_Terra[6];
  glGenBuffers(6, VBO_Terra);

  // geometria
  glBindBuffer(GL_ARRAY_BUFFER, VBO_Terra[0]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(geomTerra), geomTerra, GL_STATIC_DRAW);
  glVertexAttribPointer(vertexLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(vertexLoc);

  // normals
  glBindBuffer(GL_ARRAY_BUFFER, VBO_Terra[1]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(normTerra), normTerra, GL_STATIC_DRAW);
  glVertexAttribPointer(normalLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(normalLoc);

  // component ambient
  glBindBuffer(GL_ARRAY_BUFFER, VBO_Terra[2]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(matamb), matamb, GL_STATIC_DRAW);
  glVertexAttribPointer(matambLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(matambLoc);

   // component difusa
  glBindBuffer(GL_ARRAY_BUFFER, VBO_Terra[3]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(matdiff), matdiff, GL_STATIC_DRAW);
  glVertexAttribPointer(matdiffLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(matdiffLoc);

  // component especular
  glBindBuffer(GL_ARRAY_BUFFER, VBO_Terra[4]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(matspec), matspec, GL_STATIC_DRAW);
  glVertexAttribPointer(matspecLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(matspecLoc);

  // component shininness
  glBindBuffer(GL_ARRAY_BUFFER, VBO_Terra[5]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(matshin), matshin, GL_STATIC_DRAW);
  glVertexAttribPointer(matshinLoc, 1, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(matshinLoc);
  
  glBindVertexArray (0);
}


void LL4GLWidget::creaBuffersModel (Model &model, const char *fileName, GLuint *VAO)
{
  // Càrrega del model abans de crear els buffers!
  model.load(fileName);

  // Creació del VAO
  glGenVertexArrays(1, VAO);
  glBindVertexArray(*VAO);

  // Creació dels VBO
  GLuint VBO[6];
  glGenBuffers(6, VBO);

  // geometria
  glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*model.faces().size()*3*3,
	       model.VBO_vertices(), GL_STATIC_DRAW);
  glVertexAttribPointer(vertexLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(vertexLoc);

  // normals
  glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*model.faces().size()*3*3,
	       model.VBO_normals(), GL_STATIC_DRAW);
  glVertexAttribPointer(normalLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(normalLoc);

  // component ambient
  glBindBuffer(GL_ARRAY_BUFFER, VBO[2]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*model.faces().size()*3*3,
	       model.VBO_matamb(), GL_STATIC_DRAW);
  glVertexAttribPointer(matambLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(matambLoc);
  
  // component difusa
  glBindBuffer(GL_ARRAY_BUFFER, VBO[3]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*model.faces().size()*3*3,
	       model.VBO_matdiff(), GL_STATIC_DRAW);
  glVertexAttribPointer(matdiffLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(matdiffLoc);

  // component especular
  glBindBuffer(GL_ARRAY_BUFFER, VBO[4]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*model.faces().size()*3*3,
	       model.VBO_matspec(), GL_STATIC_DRAW);
  glVertexAttribPointer(matspecLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(matspecLoc);

  // Buffer de component shininness
  glBindBuffer(GL_ARRAY_BUFFER, VBO[5]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*model.faces().size()*3,
	       model.VBO_matshin(), GL_STATIC_DRAW);
  glVertexAttribPointer(matshinLoc, 1, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(matshinLoc);
  
  glBindVertexArray (0);
}


void LL4GLWidget::carregaShaders()
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

  // Identificadors per als  atributs
  vertexLoc  = glGetAttribLocation (program->programId(), "vertex");
  normalLoc  = glGetAttribLocation (program->programId(), "normal");
  matambLoc  = glGetAttribLocation (program->programId(), "matamb");
  matdiffLoc = glGetAttribLocation (program->programId(), "matdiff");
  matspecLoc = glGetAttribLocation (program->programId(), "matspec");
  matshinLoc = glGetAttribLocation (program->programId(), "matshin");
  
  // Identificadors per als uniform locations
  transLoc = glGetUniformLocation (program->programId(), "TG");
  projLoc  = glGetUniformLocation (program->programId(), "Proj");
  viewLoc  = glGetUniformLocation (program->programId(), "View");

  colFocusLoc = glGetUniformLocation (program->programId(), "colFocus");
  posFocusLoc = glGetUniformLocation (program->programId(), "posFocus");
}

void LL4GLWidget::calculaCapsaModel (Model &model, float *escala, glm::vec3 &centreBase)
{
  // Càlcul capsa contenidora i valors transformacions inicials
  float minx, miny, minz, maxx, maxy, maxz;
  minx = maxx = model.vertices()[0];
  miny = maxy = model.vertices()[1];
  minz = maxz = model.vertices()[2];
  for (unsigned int i = 3; i < model.vertices().size(); i+=3)
  {
    if (model.vertices()[i+0] < minx)
      minx = model.vertices()[i+0];
    if (model.vertices()[i+0] > maxx)
      maxx = model.vertices()[i+0];
    if (model.vertices()[i+1] < miny)
      miny = model.vertices()[i+1];
    if (model.vertices()[i+1] > maxy)
      maxy = model.vertices()[i+1];
    if (model.vertices()[i+2] < minz)
      minz = model.vertices()[i+2];
    if (model.vertices()[i+2] > maxz)
      maxz = model.vertices()[i+2];
  }
  
  *escala   = 1.0/(maxy - miny);
  centreBase[0] = (minx + maxx)/2.0;
  centreBase[1] = miny;
  centreBase[2] = (minz + maxz)/2.0;
}

void LL4GLWidget::mouseMoveEvent (QMouseEvent *)
{}


void LL4GLWidget::mousePressEvent (QMouseEvent *e)
{
  xClick = e->x();
  yClick = e->y();

  if (e->button() & Qt::LeftButton &&
      ! (e->modifiers() & (Qt::ShiftModifier|Qt::AltModifier|Qt::ControlModifier
)))
  {
    DoingInteractive = ROTATE;
  }
}

void LL4GLWidget::mouseReleaseEvent( QMouseEvent *)
{
  DoingInteractive = NONE;
}

void LL4GLWidget::keyPressEvent (QKeyEvent *e)
{
  switch (e->key()) {
    case Qt::Key_0:
      vista = 0;
      break;
    case Qt::Key_1:
      vista = 1;
      break;
    case Qt::Key_2:
      vista = 2;
      break;
    case Qt::Key_3:
      vista = 3;
      break;
    default:
      e->ignore();
  }
  makeCurrent();
  viewTransform();
  update();
}
