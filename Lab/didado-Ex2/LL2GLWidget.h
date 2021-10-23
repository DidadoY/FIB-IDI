#define GLM_FORCE_RADIANS
#include <QOpenGLFunctions_3_3_Core>
#include <QOpenGLWidget>
#include <QOpenGLShader>
#include <QOpenGLShaderProgram>
#include <QKeyEvent>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "model.h"

class LL2GLWidget : public QOpenGLWidget, protected QOpenGLFunctions_3_3_Core 
{
  Q_OBJECT

  public:
    LL2GLWidget (QWidget *parent=0);
    ~LL2GLWidget ();

  protected:
    // initializeGL - Aqui incluim les inicialitzacions del contexte grafic.
    virtual void initializeGL ( );
    // paintGL - Mètode cridat cada cop que cal refrescar la finestra.
    // Tot el que es dibuixa es dibuixa aqui.
    virtual void paintGL ( );
    // resizeGL - És cridat quan canvia la mida del widget
    virtual void resizeGL (int width, int height);
    // keyPressEvent - Es cridat quan es prem una tecla
    virtual void keyPressEvent (QKeyEvent *event);

    virtual void iniEscena ();
    virtual void iniCamera ();
    virtual void creaBuffersHomer ();
    virtual void creaBuffersEscac ();
    virtual void carregaShaders ();
    virtual void homerTransform (int i, int j);
    virtual void escacTransform (int i, int j);
    virtual void viewTransform ();
    virtual void projectTransform ();
    virtual void calculaCapsaModel ();
    
    // attribute locations
    GLuint vertexLoc, colorLoc;

    // uniform locations
    GLuint transLoc, viewLoc, projLoc, indexColorLoc;

    // VAO i VBO names
    GLuint VAO_Homer, VAO_Escac;

    // Program
    QOpenGLShaderProgram *program;

    // Viewport
    GLint ample, alt;

    // Internal vars
    float escalaH, angleH;
    int   xH, zH;
    float radiEscena;
    float fov, ra, znear, zfar;
    glm::vec3 obs, vrp, up;
    glm::vec3 baseHomer;

    Model homer;     

    int deltai[8] = {-1, -2, -2, -1,  1,  2,  2,  1};
    int deltaj[8] = {-2, -1,  1,  2,  2,  1, -1, -2};
};

