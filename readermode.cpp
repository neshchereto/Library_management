#include "readermode.h"
#include "ui_readermode.h"

#include "registrationdialog.h"

ReaderMode::ReaderMode(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ReaderMode)
{
    ui->setupUi(this);
}

ReaderMode::~ReaderMode()
{
    delete ui;
}

void ReaderMode::on_pushButton_clicked()
{
    RegistrationDialog dialog;
    dialog.exec();
}


void ReaderMode::on_pushButton_2_clicked()
{

}


void ReaderMode::on_pushButton_3_clicked()
{

}


void ReaderMode::on_pushButton_4_clicked()
{

}


void ReaderMode::on_pushButton_5_clicked()
{

}


void ReaderMode::on_pushButton_6_clicked()
{

}


void ReaderMode::on_pushButton_7_clicked()
{

}


void ReaderMode::on_pushButton_8_clicked()
{

}

