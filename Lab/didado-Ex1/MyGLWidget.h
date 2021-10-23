#include <QOpenGLFunctions_3_3_Core>
#include <QOpenGLWidget>
#include <QOpenGLShaderProgram>
#include <QKeyEvent>
#define GLM_FORCE_RADIANS
#include "glm/glm.hpp"
#include <stdio.h>

class MyGLWidget : public QOpenGLWidget, protected QOpenGLFunctions_3_3_Core 
{
  Q_OBJECT

  public:
    MyGLWidget (QWidget *parent=0);
    ~MyGLWidget ();

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

  private:
    void creaBuffers ();
    void carregaShaders ();
    void modelTransform(glm::vec3 centre, float angle, float escala);
    void valorsInicials ();
    void pintaEngranatge(glm::vec3 centre, float angleRot, float escala, int color);

    // attribute locations
    GLuint vertexLoc, colorLoc;

    // uniform locations
    GLuint transLoc, canviLoc, canviColor;

    // VAO i VBO names
    GLuint VAO;
    GLint ample, alt;

    // Program
    QOpenGLShaderProgram *program;

    // Internal vars
    int iRoig, iVerd, iBlau, sentido;
    glm::vec3 groc;
    glm::vec3 eixZ;
    
    glm::vec3 centrePinyo, centreEngraA, centreEngraB;
    float rPinyo, rEngraA, rEngraB, relacioPEA, relacioPEB;
    float anglePinyo, angleEngraA, angleEngraB;
    float deltaTriangle, deltaAnglePinyo;
    float escalaPinyo, escalaEngraA, escalaEngraB;
};

