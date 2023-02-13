#include "inventorizecopieswidget.h"
#include "ui_inventorizecopieswidget.h"

#include <QSqlError>
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

    ui->tableView->verticalHeader()->setVisible(false); // remove indexes on leftside
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
    // Set book_id as idLabel text
    ui->idLabel->setText(
                ui->tableView->model()->index(index.row(), 0).data().toString());

    ui->inventorizePushButton->setEnabled(true);
}

void InventorizeCopiesWidget::on_inventorizePushButton_clicked()
{
    const QString book_id {ui->idLabel->text()};

    bool ok {true};
    QSqlQuery query;
    for (int i {}; i != ui->spinBox->value() && ok; ++i) {
        ok = query.exec("INSERT INTO card (book_id) "
                        "VALUES (" + book_id + ")");
    }

    if (ok) {
        QMessageBox::information(this, "Success", "Copies were inventorized.");
    } else {
        QMessageBox::critical(this, "Error", query.lastError().text());
    }
}
