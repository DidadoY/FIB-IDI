// MyGLWidget.h
#include "LL4GLWidget.h"

class MyGLWidget : public LL4GLWidget {
  Q_OBJECT

  public:
    MyGLWidget(QWidget *parent=0) : LL4GLWidget(parent) {}
    ~MyGLWidget();

  public slots:
    virtual void semueve(int i);
  protected:
    virtual void cotxeTransform();
    virtual void iniEscena();
    virtual void setupLights();
    virtual void carregaShaders();
    virtual void initializeGL();
    glm::vec3 colFocus1, posFocus1, colFocus2, posFocus2, colFocus3, posFocus3;
    int b;
    GLuint colFocusLoc1, posFocusLoc1, colFocusLoc2, posFocusLoc2, colFocusLoc3, posFocusLoc3, boolLoc;
  private:
    int printOglError(const char file[], int line, const char func[]);
    virtual void keyPressEvent (QKeyEvent *);
};
