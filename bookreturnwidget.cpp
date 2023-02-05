#include "bookreturnwidget.h"
#include "ui_bookreturnwidget.h"

#include <QDate>
#include <QMessageBox>
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QSortFilterProxyModel>

BookReturnWidget::BookReturnWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::BookReturnWidget)
{
    ui->setupUi(this);

    loadTable();
}

BookReturnWidget::~BookReturnWidget()
{
    delete ui;
}

void BookReturnWidget::on_tableView_activated(const QModelIndex &index)
{
    const QString return_id {ui->tableView->model()->index(index.row(), 0).data().toString()};
    ui->idLabel->setText(return_id);
}


void BookReturnWidget::on_returnPushButton_clicked()
{
    QSqlQuery query2;
    if (query2.exec("DELETE FROM return_table "
                    "WHERE return_id = " + ui->idLabel->text()))
    {
        QMessageBox::information(this, "Success", "Book was returned.");
    } else {
        QMessageBox::critical(this, "Error", "Book was NOT returned.");
    }

    // Update table after deleting
    loadTable();
}

void BookReturnWidget::loadTable()
{
    QSqlQueryModel* books_on_hands_model {new QSqlQueryModel};
    books_on_hands_model->setQuery("SELECT * FROM return_table");
    QSortFilterProxyModel* proxy_model {new QSortFilterProxyModel};
    proxy_model->setSourceModel(books_on_hands_model);
    ui->tableView->setModel(proxy_model);
    ui->tableView->setSortingEnabled(true);
    ui->tableView->sortByColumn(0, Qt::AscendingOrder);
    ui->tableView->setMinimumWidth(ui->tableView->columnWidth(0)
                                 * books_on_hands_model->columnCount()
                                 + 20);
}

