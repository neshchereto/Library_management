#include "registrationdialog.h"
#include "qpushbutton.h"
#include "ui_registrationdialog.h"

#include <QSqlQuery>
#include <QSqlError>
#include <QMessageBox>

RegistrationDialog::RegistrationDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::RegistrationDialog)
{
    ui->setupUi(this);

    ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);

    connect(ui->fullNameLineEdit, SIGNAL(textChanged(QString)), this, SLOT(onTextChanged()));
    connect(ui->passportLineEdit, SIGNAL(textChanged(QString)), this, SLOT(onTextChanged()));
    connect(ui->addressLineEdit , SIGNAL(textChanged(QString)), this, SLOT(onTextChanged()));
    connect(ui->mobileLineEdit  , SIGNAL(textChanged(QString)), this, SLOT(onTextChanged()));
    connect(ui->birthdayLineEdit, SIGNAL(textChanged(QString)), this, SLOT(onTextChanged()));
}

RegistrationDialog::~RegistrationDialog()
{
    delete ui;
}

void RegistrationDialog::onTextChanged()
{
    // IF ALL LINE EDITS ARE FILLED THAN SET "Ok" BUTTON ENABLED
    ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(!ui->fullNameLineEdit->text().isEmpty()
                                                         && !ui->passportLineEdit->text().isEmpty()
                                                         && !ui->addressLineEdit ->text().isEmpty()
                                                         && !ui->mobileLineEdit  ->text().isEmpty()
                                                         && !ui->birthdayLineEdit->text().isEmpty());
}

void RegistrationDialog::on_buttonBox_accepted()
{
    QSqlQuery query;
    query.prepare("INSERT INTO reader (full_name, passport, address, mobile, birthday, entry_day)"
                  "VALUES ('" + ui->fullNameLineEdit->text() + "', '"
                              + ui->passportLineEdit->text() + "', '"
                              + ui->addressLineEdit ->text() + "', '"
                              + ui->mobileLineEdit  ->text() + "', '"
                              + ui->birthdayLineEdit->text() + "', "
                              + "CURDATE())");

    // query.exec() returns true if inserted, false if not
    if (query.exec()) {
        QMessageBox::information(this, "Success", "Account was created.");
    } else {
        QMessageBox::critical(this, "Error", query.lastError().text());
    }
}

