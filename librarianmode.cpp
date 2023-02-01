#include "librarianmode.h"
#include "ui_librarianmode.h"
#include "inventorydialog.h"
#include "requestwidget.h"
#include "readerinfowidget.h"

#include <QWidget>
#include <QSqlQueryModel>
#include <QTableView>
#include <QInputDialog>
#include <QMessageBox>
#include <QSqlQuery>

LibrarianMode::LibrarianMode(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::LibrarianMode)
{
    ui->setupUi(this);
}

LibrarianMode::~LibrarianMode()
{
    delete ui;
}


void LibrarianMode::on_pushButton_clicked()
{
    InventoryDialog* dialog {new InventoryDialog};
    dialog->exec();
}


void LibrarianMode::on_pushButton_2_clicked()
{
    bool ok;
    const int book_id {QInputDialog::getInt(this, "Book ID",
                                            "Enter book_id: ", 1, 1, 1000000, 1, &ok)};

    if (!ok) {
        return;
    }

    QSqlQuery query;
    query.exec("SELECT book_id FROM book "
               "WHERE book_id = " + QString::number(book_id));

    // Check if book with such id exists.
    if (!query.next()) {
        QMessageBox::critical(this, "Error", "There is no book with such id.");
        return;
    }

    const int number_of_copies {QInputDialog::getInt(this, "Copies",
                                                     "Enter number of copies: ", 1, 1, 500, 1, &ok)};

    if (ok) {
        for (int i {}; i != number_of_copies; ++i) {
            QSqlQuery query6;
            query6.exec("INSERT INTO card (book_id) "
                        "VALUES (" + QString::number(book_id) + ")");
        }

        QMessageBox::information(this, "Success", "New book was inserted.");
    }
}


void LibrarianMode::on_pushButton_3_clicked()
{
    RequestWidget* widget {new RequestWidget};
    widget->show();
}


void LibrarianMode::on_pushButton_5_clicked()
{
    ReaderInfoWidget* widget {new ReaderInfoWidget};
    widget->show();
}


void LibrarianMode::on_pushButton_6_clicked()
{

}


void LibrarianMode::on_pushButton_7_clicked()
{

}


void LibrarianMode::on_pushButton_8_clicked()
{

}

