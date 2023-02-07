#include "readermode.h"
#include "ui_readermode.h"

#include <QSqlQueryModel>
#include <QSortFilterProxyModel>

ReaderMode::ReaderMode(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ReaderMode)
{
    ui->setupUi(this);

    QSqlQueryModel* books_and_copies_model {new QSqlQueryModel};
    books_and_copies_model->setQuery("SELECT DISTINCT b.title, b.edition, ut.topic, "
                                         "(SELECT GROUP_CONCAT(DISTINCT a.full_name SEPARATOR ', ') "
                                         "FROM  author_book ab "
                                         "JOIN  author a ON ab.author_id = a.author_id "
                                         "WHERE ab.book_id = b.book_id) AS 'authors', "
                                         "(SELECT GROUP_CONCAT(DISTINCT card.inventoryed_id SEPARATOR ', ') "
                                         "FROM  card "
                                         "WHERE card.book_id = b.book_id) AS 'inventoryed ids' "
                                     "FROM card "
                                     "JOIN book b ON card.book_id = b.book_id "
                                     "JOIN udk_thematic ut "
                                     "WHERE ut.udk_id = b.udk_id");
    QSortFilterProxyModel* proxy_model {new QSortFilterProxyModel};
    proxy_model->setSourceModel(books_and_copies_model);
    ui->tableView->setModel(proxy_model);
    ui->tableView->setColumnWidth(0, 200);
    ui->tableView->setColumnWidth(2, 200);
    ui->tableView->setColumnWidth(3, 150);
    ui->tableView->verticalHeader()->setVisible(false); // remove indexes on leftside
    ui->tableView->setSortingEnabled(true);
    ui->tableView->sortByColumn(0, Qt::AscendingOrder); // sort by title (alphabetically)
    ui->tableView->setMinimumWidth(ui->tableView->columnWidth(0)
                                 + ui->tableView->columnWidth(1)
                                 + ui->tableView->columnWidth(2)
                                 + ui->tableView->columnWidth(3)
                                 + ui->tableView->columnWidth(4)
                                 + 25);
}

ReaderMode::~ReaderMode()
{
    delete ui;
}

void ReaderMode::on_tableView_activated(const QModelIndex &index)
{
//    const QString book_id {ui->tableView->model()->index(index.row(), 0).data().toString()};
//    ui->idLabel->setText(book_id);

    // SET INVENTORY IDS TO COMBOBOX
}

