// MyGLWidget.h
#include "Bl3GLWidget.h"

class MyGLWidget : public Bl3GLWidget {
  Q_OBJECT
  public:
    MyGLWidget(QWidget *parent=0) : Bl3GLWidget(parent) {}
    ~MyGLWidget();
  protected:
    virtual void mouseMoveEvent(QMouseEvent *e);
    virtual void keyPressEvent(QKeyEvent* event);
  private:
    int printOglError(const char file[], int line, const char func[]);
};