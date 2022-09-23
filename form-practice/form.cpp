#include "form.h"
#include "ui_form.h"

Form::Form(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Form)
{
    ui->setupUi(this);

    QImage image("/home/kotetsu/Pictures/note150.png");
    ui->label->setPixmap(QPixmap::fromImage(QImage(image)));
}

Form::~Form()
{
    delete ui;
}
