#include "bookholderwidget.h"
#include "ui_bookholderwidget.h"

#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QSortFilterProxyModel>

BookHolderWidget::BookHolderWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::BookHolderWidget)
{
    ui->setupUi(this);

    QSqlQueryModel* books_on_hands_model {new QSqlQueryModel};
    books_on_hands_model->setQuery("SELECT * FROM return_table");
    QSortFilterProxyModel* proxy_model {new QSortFilterProxyModel};
    proxy_model->setSourceModel(books_on_hands_model);
    ui->tableView->setModel(proxy_model);
    ui->tableView->setSortingEnabled(true);
    ui->tableView->sortByColumn(0, Qt::AscendingOrder);

    ui->tableView->verticalHeader()->setVisible(false);
    ui->tableView->setMinimumWidth(ui->tableView->columnWidth(0)
                                 * books_on_hands_model->columnCount()
                                 + 15);
}

BookHolderWidget::~BookHolderWidget()
{
    delete ui;
}


void BookHolderWidget::on_tableView_activated(const QModelIndex &index)
{
    const QString reader_id {ui->tableView->model()->index(index.row(), 1).data().toString()};

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

