#include "readerinfowidget.h"
#include "ui_readerinfowidget.h"

ReaderInfoWidget::ReaderInfoWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ReaderInfoWidget)
{
    ui->setupUi(this);


}

ReaderInfoWidget::~ReaderInfoWidget()
{
    delete ui;
}
