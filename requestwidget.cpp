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
    ui->tableView->setMinimumWidth(ui->tableView->columnWidth(0)
                                 * requests_model->columnCount()
                                 + 10);
}

void RequestWidget::on_tableView_activated(const QModelIndex &index)
{
    const QString request_id {ui->tableView->model()->index(index.row(), 0).data().toString()};
    ui->idLabel->setText(request_id);
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

    QSqlQuery query2;
    query2.exec("INSERT INTO return_table (inventoryed_id, issue_date, return_date, reader_id) "
                "VALUES (" + inventoryed_id
                           + ", CURDATE(), "
                           + "CURDATE() + INTERVAL "
                           + QString::number(ui->timeSpinBox->value()) + " "
                           + ui->periodComboBox->currentText() + ", "
                           + reader_id + ")");

    QSqlQuery query3;
    query3.exec("DELETE FROM request "
                "WHERE request_id = " + request_id);

    QMessageBox::information(this, "Success", "Successfully accepted.");

    loadTable();
}
