#include "requestwidget.h"
#include "ui_requestwidget.h"

#include <QMessageBox>
#include <QSqlQuery>
#include <QSqlQueryModel>

RequestWidget::RequestWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::RequestWidget)
{
    ui->setupUi(this);

    loadTable();
}

RequestWidget::~RequestWidget()
{
    delete ui;
}

void RequestWidget::loadTable()
{
    ui->idLabel->setText("*Double click on request*");

    QSqlQueryModel* requests_model {new QSqlQueryModel};
    requests_model->setQuery("SELECT * FROM request");
    ui->tableView->setModel(requests_model);

    ui->tableView->verticalHeader()->setVisible(false); // remove indexes on leftside
    ui->tableView->setMinimumWidth(ui->tableView->columnWidth(0)
                                 * requests_model->columnCount()
                                 + 10);
}

void RequestWidget::on_tableView_activated(const QModelIndex &index)
{
    // Set request_id as idLabel text
    ui->idLabel->setText(
                ui->tableView->model()->index(index.row(), 0).data().toString());

    ui->acceptPushButton->setEnabled(true);
}

void RequestWidget::on_acceptPushButton_clicked()
{
    const QString request_id {ui->idLabel->text()};

    QSqlQuery query;
    query.exec("SELECT inventoryed_id, reader_id FROM request "
               "WHERE request_id = " + request_id);
    query.next();
    QString inventoryed_id {query.value(0).toString()};
    QString reader_id      {query.value(1).toString()};

    QSqlQuery query2{"INSERT INTO return_table (inventoryed_id, issue_date, return_date, reader_id) "
                     "VALUES (" + inventoryed_id
                                + ", CURDATE(), "
                                + "CURDATE() + INTERVAL "
                                + QString::number(ui->timeSpinBox->value()) + " "
                                + ui->periodComboBox->currentText() + ", "
                                + reader_id + ")"};
    if (query2.exec()) {
        QMessageBox::information(this, "Success", "Successfully accepted.");
        QSqlQuery query3;
        query3.exec("DELETE FROM request "
                    "WHERE request_id = " + request_id);
        loadTable(); // update after erasing.
        ui->acceptPushButton->setEnabled(false); // id is not choosed, so disable button
    } else {
        QMessageBox::information(this, "Success", "Successfully accepted.");
    }
}
