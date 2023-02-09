#include "dbsetup.h"
#include "ui_dbsetup.h"

#include <QtSql>
#include <QtSql/QSqlDatabase>
#include <QMessageBox>

DBSetup::DBSetup(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DBSetup)
{
    ui->setupUi(this);
}

DBSetup::~DBSetup()
{
    delete ui;
}

void DBSetup::on_buttonBox_accepted()
{
    // If "Ok" was pressed - connect to DB
    QSqlDatabase db {QSqlDatabase::addDatabase("QMYSQL")};
    db.setDatabaseName(ui->DBNameLineEdit  ->text());
    db.setUserName    (ui->userLineEdit    ->text());
    db.setPassword    (ui->passwordLineEdit->text());
    db.setHostName    (ui->serverLineEdit  ->text());

    if (db.open()) {
        QMessageBox::information(this, "Success", "Connected to data base.");
        return accept();
    } else {
        QMessageBox::critical(this, "Error", db.lastError().text());
        return reject();
    }
}

