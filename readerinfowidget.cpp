#include "readerinfowidget.h"
#include "ui_readerinfowidget.h"

#include <QComboBox>
#include <QSqlQuery>
#include <QSqlQueryModel>

ReaderInfoWidget::ReaderInfoWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ReaderInfoWidget)
{
    ui->setupUi(this);

    QSqlQueryModel* readers_model {new QSqlQueryModel};
    readers_model->setQuery("SELECT reader_id, full_name FROM reader");

    ui->readersTableView->setModel(readers_model);

    QSqlQueryModel* model {new QSqlQueryModel};
    model->setQuery("SELECT reader_id FROM reader "
                    "ORDER BY (reader_id)");

    ui->idComboBox->setModel(model);

    loadReaderInfo();
    connect(ui->idComboBox, SIGNAL(currentTextChanged(QString)), this, SLOT(loadReaderInfo()));
}

ReaderInfoWidget::~ReaderInfoWidget()
{
    delete ui;
}


void ReaderInfoWidget::loadReaderInfo()
{
    QSqlQuery query;
    query.exec("SELECT full_name, passport, address, mobile, birthday, entry_day "
               "FROM reader "
               "WHERE reader_id = " + ui->idComboBox->currentText());
    query.next();

    ui->fullNameLabel->setText(query.value(0).toString());
    ui->passportLabel->setText(query.value(1).toString());
    ui->addressLabel ->setText(query.value(2).toString());
    ui->mobileLabel  ->setText(query.value(3).toString());
    ui->birthDayLabel->setText(query.value(4).toString());
    ui->entryDayLabel->setText(query.value(5).toString());

    QSqlQuery query2("SELECT rt.inventoryed_id, b.title, b.edition, rt.issue_date, rt.return_date "
                     "FROM return_table rt "
                     "JOIN card c ON rt.inventoryed_id = c.inventoryed_id "
                     "JOIN book b ON c.book_id = b.book_id "
                     "WHERE reader_id = " + ui->idComboBox->currentText());
    QSqlQueryModel* books_on_hands_model {new QSqlQueryModel};
    books_on_hands_model->setQuery(query2);

    ui->booksOnHandsTableView->setModel(books_on_hands_model);

    int colWidth = ui->booksOnHandsTableView->columnWidth(1);
    ui->booksOnHandsTableView->setFixedWidth(5.5 * colWidth);
}
