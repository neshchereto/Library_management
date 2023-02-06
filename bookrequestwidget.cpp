#include "bookrequestwidget.h"
#include "ui_bookrequestwidget.h"

BookRequestWidget::BookRequestWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::BookRequestWidget)
{
    ui->setupUi(this);
}

BookRequestWidget::~BookRequestWidget()
{
    delete ui;
}
