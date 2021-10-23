#ifndef ABACUS_H
#define ABACUS_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class Abacus; }
QT_END_NAMESPACE

class Abacus : public QMainWindow
{
    Q_OBJECT

public:
    Abacus(QWidget *parent = nullptr);
    ~Abacus();

private:
    Ui::Abacus *ui;
};
#endif // ABACUS_H
