#include "ExamGLWidget.h"

class MyGLWidget:public ExamGLWidget
{
  Q_OBJECT

  public:
    MyGLWidget(QWidget *parent=0) : ExamGLWidget(parent) {}
    ~MyGLWidget();

  public slots:
    void camara ();
    void uno ();
    void dos();
    void tres ();


  protected:
    virtual void paintGL ();
    virtual void keyPressEvent(QKeyEvent* event);
    virtual void modelTransformCub (float escala, float angle);
    virtual void modelTransformPatricio (float escala);
    virtual void projectTransform ();
    virtual void viewTransform ();
    virtual void initializeGL ();

    bool activado, groc;
    float angle2, anglecub;


  private:
    int printOglError(const char file[], int line, const char func[]);

};
