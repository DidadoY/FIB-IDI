#include "abacus.h"
#include "ui_abacus.h"

Abacus::Abacus(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Abacus)
{
    ui->setupUi(this);
}

Abacus::~Abacus()
{
    delete ui;
}

