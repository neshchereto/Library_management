#include "inventorizecopieswidget.h"
#include "ui_inventorizecopieswidget.h"

#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QMessageBox>
#include <QSortFilterProxyModel>

InventorizeCopiesWidget::InventorizeCopiesWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::InventorizeCopiesWidget)
{
    ui->setupUi(this);

    QSqlQueryModel* books_on_hands_model {new QSqlQueryModel};
    books_on_hands_model->setQuery("SELECT * FROM book");
    QSortFilterProxyModel* proxy_model {new QSortFilterProxyModel};
    proxy_model->setSourceModel(books_on_hands_model);
    ui->tableView->setModel(proxy_model);
    ui->tableView->setSortingEnabled(true);
    ui->tableView->sortByColumn(0, Qt::AscendingOrder);
    ui->tableView->setMinimumWidth(ui->tableView->columnWidth(0)
                                 * books_on_hands_model->columnCount()
                                 + 50);
}

InventorizeCopiesWidget::~InventorizeCopiesWidget()
{
    delete ui;
}

void InventorizeCopiesWidget::on_tableView_activated(const QModelIndex &index)
{
    const QString book_id {ui->tableView->model()->index(index.row(), 0).data().toString()};
    ui->idLabel->setText(book_id);
}

void InventorizeCopiesWidget::on_inventorizePushButton_clicked()
{
    const QString book_id {ui->idLabel->text()};

    bool ok {true};
    for (int i {}; i != ui->spinBox->value() && ok; ++i) {
        QSqlQuery query;
        ok = query.exec("INSERT INTO card (book_id) "
                        "VALUES (" + book_id + ")");
    }

    if (ok) {
        QMessageBox::information(this, "Success", "Copies were inventorized.");
    } else {
        QMessageBox::critical(this, "Error", "Error occured. "
                                             "Some copies might be inventorized.");
    }
}
