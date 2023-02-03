#include "librarianmode.h"
#include "ui_librarianmode.h"
#include "inventorydialog.h"
#include "requestwidget.h"
#include "readerinfowidget.h"
#include "bookholder.h"
#include "bookreturnwidget.h"

#include <QWidget>
#include <QSqlQueryModel>
#include <QTableView>
#include <QInputDialog>
#include <QMessageBox>
#include <QSqlQuery>
#include <QSqlQueryModel>

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
    QSqlQueryModel* debtors_model {new QSqlQueryModel};
    debtors_model->setQuery("SELECT * FROM return_table "
                            "WHERE DATE_FORMAT(return_date, \'%Y-%m-%d\') <= CURDATE()");

    if (debtors_model->rowCount() == 0) {
        QMessageBox::information(this, "No data", "There are no debtors");
        return;
    }

    QTableView* table {new QTableView};
    table->setModel(debtors_model);
    table->setFixedWidth(table->columnWidth(1) * 5.5);
    table->show();
}


void LibrarianMode::on_pushButton_7_clicked()
{
    BookHolder* widget {new BookHolder};
    widget->show();
}


void LibrarianMode::on_pushButton_8_clicked()
{
    BookReturnWidget* widget {new BookReturnWidget};
    widget->show();
}

