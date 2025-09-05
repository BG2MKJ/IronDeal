#include "registerate.h"
#include "ui_registerate.h"

registerate::registerate(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::registerate)
{
    ui->setupUi(this);
}

registerate::~registerate()
{
    delete ui;
}
