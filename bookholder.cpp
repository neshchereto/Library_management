#include "bookholder.h"
#include "ui_bookholder.h"

#include <QDate>
#include <QDebug>
#include <QSqlQuery>
#include <QSqlQueryModel>

BookHolder::BookHolder(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::BookHolder)
{
    ui->setupUi(this);

    QSqlQueryModel* books_on_hands_model {new QSqlQueryModel};
    books_on_hands_model->setQuery("SELECT * FROM return_table");

    ui->tableView->setModel(books_on_hands_model);
    ui->tableView->setMinimumWidth(ui->tableView->verticalHeader()->width()
                                 + ui->tableView->columnWidth(0)
                                 * books_on_hands_model->columnCount());
}

BookHolder::~BookHolder()
{
    delete ui;
}


void BookHolder::on_tableView_activated(const QModelIndex &index)
{
    QString val {ui->tableView->model()->data(index).toString()};

    // Change date format to MySQL like.
    QString date_string {val};
    QDate::fromString(date_string, "yyyy-MM-dd");

    QSqlQuery query;
    query.exec("SELECT reader_id FROM return_table "
               "WHERE return_id = "    + val +
               " OR reader_id = "      + val +
               " OR inventoryed_id = " + val +
               " OR issue_date = '"    + date_string +
               "' OR return_date = '"  + date_string + "'");
    query.next();
    QString reader_id {query.value(0).toString()};

    QSqlQuery info_query;
    info_query.exec("SELECT full_name, passport, address, mobile, birthday, entry_day "
                    "FROM reader "
                    "WHERE reader_id = " + reader_id);

    info_query.next();

    ui->fullNameLabel->setText(info_query.value(0).toString());
    ui->passportLabel->setText(info_query.value(1).toString());
    ui->addressLabel ->setText(info_query.value(2).toString());
    ui->mobileLabel  ->setText(info_query.value(3).toString());
    ui->birthDayLabel->setText(info_query.value(4).toString());
    ui->entryDayLabel->setText(info_query.value(5).toString());
}

