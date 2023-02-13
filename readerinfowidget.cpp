#include "readerinfowidget.h"
#include "ui_readerinfowidget.h"

#include <QComboBox>
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QSortFilterProxyModel>

ReaderInfoWidget::ReaderInfoWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ReaderInfoWidget)
{
    ui->setupUi(this);

    QSqlQueryModel* readers_model {new QSqlQueryModel};
    readers_model->setQuery("SELECT reader_id, full_name FROM reader");
    QSortFilterProxyModel* proxy_model {new QSortFilterProxyModel};
    proxy_model->setSourceModel(readers_model);
    ui->readersTableView->setModel(proxy_model);
    ui->readersTableView->setSortingEnabled(true);
    ui->readersTableView->sortByColumn(0, Qt::AscendingOrder);

    ui->readersTableView->verticalHeader()->setVisible(false); // remove indexes on leftside
    ui->readersTableView->setColumnWidth(1, 200);
    ui->readersTableView->setMinimumWidth(ui->readersTableView->columnWidth(0)
                                        + ui->readersTableView->columnWidth(1)
                                        + 20);
}

ReaderInfoWidget::~ReaderInfoWidget()
{
    delete ui;
}

void ReaderInfoWidget::on_readersTableView_activated(const QModelIndex &index)
{
    const QString reader_id {ui->readersTableView->model()->index(index.row(), 0).data().toString()};

    QSqlQuery query{"SELECT full_name, passport, address, mobile, birthday, entry_day "
                    "FROM reader "
                    "WHERE reader_id = " + reader_id};
    query.next();
    ui->fullNameLabel->setText(query.value(0).toString());
    ui->passportLabel->setText(query.value(1).toString());
    ui->addressLabel ->setText(query.value(2).toString());
    ui->mobileLabel  ->setText(query.value(3).toString());
    ui->birthDayLabel->setText(query.value(4).toString());
    ui->entryDayLabel->setText(query.value(5).toString());

    QSqlQueryModel* books_on_hands_model {new QSqlQueryModel};
    books_on_hands_model->setQuery("SELECT rt.inventoryed_id, b.title, b.edition, rt.issue_date, rt.return_date "
                                   "FROM return_table rt "
                                   "JOIN card c ON rt.inventoryed_id = c.inventoryed_id "
                                   "JOIN book b ON c.book_id = b.book_id "
                                   "WHERE reader_id = " + reader_id);
    ui->booksOnHandsTableView->setModel(books_on_hands_model);

    ui->booksOnHandsTableView->verticalHeader()->setVisible(false); // remove indexes on leftside
    ui->booksOnHandsTableView->setMinimumWidth(ui->booksOnHandsTableView->columnWidth(0)
                                           * books_on_hands_model->columnCount()
                                           + 30);
}
