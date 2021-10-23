TEMPLATE    = app
QT         += opengl

INCLUDEPATH +=  /usr/include/glm
INCLUDEPATH += ../Model

FORMS += MyForm.ui

HEADERS += MyForm.h Bl3GLWidget.h MyGLWidget.h

SOURCES += main.cpp  MyForm.cpp \
        Bl3GLWidget.cpp MyGLWidget.cpp ../Model/model.cpp
