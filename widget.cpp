#include "widget.h"
#include "ui_widget.h"
#include "dbsetup.h"
#include "readersignindialog.h"

#include "librarianmode.h"
#include "readermode.h"

#include <QInputDialog>
#include <QMessageBox>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
}

Widget::~Widget()
{
    delete ui;
}

void Widget::on_DBSetupPushButton_clicked()
{
    DBSetup* db_setup {new DBSetup(this)};
    db_setup->exec();

    if (db_setup->connected) {
        ui->openMenuPushButton->setEnabled(true);
    } else {
        ui->openMenuPushButton->setEnabled(false);
    }
}

void Widget::on_openMenuPushButton_clicked()
{
    QWidget* mode {};

    if (ui->librarianRadioButton->isChecked()) {
        bool ok;
        QString inputed_password {QInputDialog::getText(this, "Password",
                                                        "Enter password:", QLineEdit::Password,
                                                        "", &ok)};
        if (!ok) {
            return;
        }

        if (inputed_password == password) {
            mode = new LibrarianMode;
        } else {
            QMessageBox::critical(this, "Error", "Incorrect password");
            return;
        }
    } else {
        ReaderSignInDialog* dialog {new ReaderSignInDialog(this)};
        if (dialog->exec() == QMessageBox::Accepted) {
            mode = new ReaderMode;
        }
    }

    if (mode != nullptr) {
        mode->show();
    }
}
