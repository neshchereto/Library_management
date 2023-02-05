#include "librarianmode.h"
#include "ui_librarianmode.h"
#include "inventorydialog.h"
#include "requestwidget.h"
#include "readerinfowidget.h"
#include "bookholderwidget.h"
#include "bookreturnwidget.h"
#include "inventorizecopieswidget.h"

#include <QWidget>
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
    InventorizeCopiesWidget* widget {new InventorizeCopiesWidget};
    widget->show();
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
    } else {
        QTableView* table {new QTableView};
        table->setModel(debtors_model);
        table->setMinimumWidth(table->columnWidth(0)
                            * debtors_model->columnCount()
                            + 20);
        table->show();
    }
}


void LibrarianMode::on_pushButton_7_clicked()
{
    BookHolderWidget* widget {new BookHolderWidget};
    widget->show();
}


void LibrarianMode::on_pushButton_8_clicked()
{
    BookReturnWidget* widget {new BookReturnWidget};
    widget->show();
}

