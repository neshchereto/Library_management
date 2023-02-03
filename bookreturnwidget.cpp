#include "bookreturnwidget.h"
#include "ui_bookreturnwidget.h"

#include <QDate>
#include <QMessageBox>
#include <QSqlQuery>
#include <QSqlQueryModel>

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
    QString val {ui->tableView->model()->data(index).toString()};

    // Change date format to MySQL like.
    QString date_string {val};
    QDate::fromString(date_string, "yyyy-MM-dd");

    QSqlQuery query;
    query.exec("SELECT return_id FROM return_table "
               "WHERE return_id = "    + val +
               " OR reader_id = "      + val +
               " OR inventoryed_id = " + val +
               " OR issue_date = '"    + date_string +
               "' OR return_date = '"  + date_string + "'");
    query.next();
    QString return_id {query.value(0).toString()};

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

    ui->tableView->setModel(books_on_hands_model);
    ui->tableView->setMinimumWidth(ui->tableView->verticalHeader()->width()
                                 + ui->tableView->columnWidth(0)
                                 * books_on_hands_model->columnCount());
}

