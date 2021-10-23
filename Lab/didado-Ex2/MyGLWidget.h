#include "LL2GLWidget.h"

#include "model.h"

class MyGLWidget : public LL2GLWidget {
  Q_OBJECT

  public:
    MyGLWidget(QWidget *parent=0) : LL2GLWidget(parent) {}
    ~MyGLWidget();

  private:
    int printOglError(const char file[], int line, const char func[]);
    void initializeGL();
    void paintGL();
    void iniCamera();
    void keyPressEvent(QKeyEvent* event);
    void homerTransform(int x, int z);
    void cambiarangulo(int xH, int zH, int &i);
    void iniEscena();
    void CameraUP();
};
