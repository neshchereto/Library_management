#include "readersignindialog.h"
#include "ui_readersignindialog.h"
#include "registrationdialog.h"

#include <QSqlQuery>
#include <QMessageBox>

ReaderSignInDialog::ReaderSignInDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ReaderSignInDialog)
{
    ui->setupUi(this);

    ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);

    connect(ui->idLineEdit      , SIGNAL(textChanged(QString)), this, SLOT(onTextChanged()));
    connect(ui->passwordLineEdit, SIGNAL(textChanged(QString)), this, SLOT(onTextChanged()));
}

ReaderSignInDialog::~ReaderSignInDialog()
{
    delete ui;
}

void ReaderSignInDialog::onTextChanged()
{
    // If all lineEdits are filled thenn set "Ok" button enabled
    ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(!ui->idLineEdit      ->text().isEmpty()
                                                         && !ui->passwordLineEdit->text().isEmpty());
}

void ReaderSignInDialog::on_buttonBox_accepted()
{
    m_reader_id = ui->idLineEdit->text();
    QSqlQuery query{"SELECT password FROM reader "
                    "WHERE reader_id = " + m_reader_id};
    query.next();
    QString password {query.value(0).toString()};

    if (password == "") {
        QMessageBox::critical(this, "Error", "There is no reader with such id.");
        return reject();
    }

    if (password != ui->passwordLineEdit->text()) {
        QMessageBox::critical(this, "Error", "Incorrect password.");
        return reject();
    }

    return accept();
}


void ReaderSignInDialog::on_pushButton_clicked()
{
    RegistrationDialog dialog;
    dialog.exec();
}

