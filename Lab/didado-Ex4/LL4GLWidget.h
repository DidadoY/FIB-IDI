#define GLM_FORCE_RADIANS
#include <QOpenGLFunctions_3_3_Core>
#include <QOpenGLWidget>
#include <QOpenGLShader>
#include <QOpenGLShaderProgram>
#include <QKeyEvent>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "model.h"

class LL4GLWidget : public QOpenGLWidget, protected QOpenGLFunctions_3_3_Core 
{
  Q_OBJECT

  public:
    LL4GLWidget (QWidget *parent=0);
    ~LL4GLWidget ();

  protected:
    // initializeGL - Aqui incluim les inicialitzacions del contexte grafic.
    virtual void initializeGL( );
    // paintGL - Mètode cridat cada cop que cal refrescar la finestra.
    // Tot el que es dibuixa es dibuixa aqui.
    virtual void paintGL( );
    // resizeGL - És cridat quan canvia la mida del widget
    virtual void resizeGL(int width, int height);
    // keyPressEvent - Es cridat quan es prem una tecla
    virtual void keyPressEvent(QKeyEvent *e);
    // Interacció amb ratolí
    virtual void mouseMoveEvent(QMouseEvent *e);
    virtual void mouseReleaseEvent(QMouseEvent *e);
    virtual void mousePressEvent(QMouseEvent *e);

    virtual void setupLights();

    void iniEscena();
    void iniCamera();
    void creaBuffersTerra();
    void creaBuffersModel(Model &model, const char *fileName, GLuint *VAO);
    void carregaShaders();
    void terraTransform();
    void cotxeTransform();
    void legoTransform();
    void viewTransform();
    void projectTransform();
    void calculaCapsaModel(Model &model, float *escala, glm::vec3 &base);
    
    // attribute locations
    GLuint vertexLoc, normalLoc;
    GLuint matambLoc, matdiffLoc, matspecLoc, matshinLoc;
      
    // uniform locations
    GLuint transLoc, viewLoc, projLoc;
    GLuint colFocusLoc, posFocusLoc;
  
    // VAO i VBO names
    GLuint VAO_Terra, VAO_Cotxe, VAO_Lego;

    // Program
    QOpenGLShaderProgram *program;

    // View transformation
    glm::mat4 View;

    // Viewport
    GLint ample, alt;

    // Internal vars
    Model cotxe, lego;

    float radiEscena;
    float fov, ra, znear, zfar;
    float escalaCotxe, angleCotxe;
    float escalaLego, angleLego;

    glm::vec3 centreEscena;
    glm::vec3 obs, vrp, vup;
    glm::vec3 baseCotxe, baseLego;
    glm::vec3 posLego;
    glm::vec3 colFocusZenital, posFocusZenital;
    
    typedef enum {NONE, ROTATE} InteractiveAction;
    InteractiveAction DoingInteractive;
    int   xClick, yClick;
    glm::vec3 eixX, eixY;

  private:
    int vista = 0; 
};

