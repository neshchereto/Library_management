#include "registrationdialog.h"
#include "qpushbutton.h"
#include "ui_registrationdialog.h"

#include <QRegExpValidator>
#include <QSqlQuery>
#include <QSqlError>
#include <QMessageBox>

RegistrationDialog::RegistrationDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::RegistrationDialog)
{
    ui->setupUi(this);

    // Set date validator for birthday lineEdit
    QRegExp R_date("(19[0-9][0-9]|20[0-9][0-9])-(0[1-9]|[1][0-2])-(0[1-9]|[12][0-9]|3[01])");
    QRegExpValidator *valida = new QRegExpValidator(R_date, this);
    ui->birthdayLineEdit->setValidator(valida);

    ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);

    connect(ui->fullNameLineEdit, SIGNAL(textChanged(QString)), this, SLOT(onTextChanged()));
    connect(ui->passportLineEdit, SIGNAL(textChanged(QString)), this, SLOT(onTextChanged()));
    connect(ui->addressLineEdit , SIGNAL(textChanged(QString)), this, SLOT(onTextChanged()));
    connect(ui->mobileLineEdit  , SIGNAL(textChanged(QString)), this, SLOT(onTextChanged()));
    connect(ui->birthdayLineEdit, SIGNAL(textChanged(QString)), this, SLOT(onTextChanged()));
    connect(ui->passwordLineEdit, SIGNAL(textChanged(QString)), this, SLOT(onTextChanged()));
}

RegistrationDialog::~RegistrationDialog()
{
    delete ui;
}

void RegistrationDialog::onTextChanged()
{
    // If all lineEdits are filled thenn set "Ok" button enabled
    ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(!ui->fullNameLineEdit->text().isEmpty()
                                                         && !ui->passportLineEdit->text().isEmpty()
                                                         && !ui->addressLineEdit ->text().isEmpty()
                                                         && !ui->mobileLineEdit  ->text().isEmpty()
                                                         && !ui->birthdayLineEdit->text().isEmpty()
                                                         && !ui->passwordLineEdit->text().isEmpty());
}

void RegistrationDialog::on_buttonBox_accepted()
{
    QSqlQuery query;
    query.prepare("INSERT INTO reader (full_name, passport, address, mobile, birthday, entry_day, password)"
                  "VALUES ('" + ui->fullNameLineEdit->text() + "', '"
                              + ui->passportLineEdit->text() + "', '"
                              + ui->addressLineEdit ->text() + "', '"
                              + ui->mobileLineEdit  ->text() + "', '"
                              + ui->birthdayLineEdit->text() + "', "
                              + "CURDATE(), '"
                              + ui->passwordLineEdit->text() + "')");
    // query.exec() returns true if inserted, false if not
    if (query.exec()) {
        // passport is unique value, so I use it to find reader_id
        QSqlQuery query2{"SELECT reader_id FROM reader "
                         "WHERE passport = '" + ui->passportLineEdit->text() + "'"};
        query2.next();
        QMessageBox::information(this, "Success", "Account was created."
                                                  "\nYour reader id is "
                                                  + query2.value(0).toString() + ".");
    } else {
        QMessageBox::critical(this, "Error", query.lastError().text());
    }
}

