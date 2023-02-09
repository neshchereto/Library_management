#include "readermode.h"
#include "ui_readermode.h"

#include <QMessageBox>
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QSortFilterProxyModel>

ReaderMode::ReaderMode(const QString& reader_id, QWidget *parent)
    : QWidget(parent)
    , m_reader_id(reader_id)
    , ui(new Ui::ReaderMode)
{
    ui->setupUi(this);

    QSqlQueryModel* topics_model {new QSqlQueryModel};
    topics_model->setQuery("SELECT topic FROM udk_thematic");
    ui->topicComboBox->setModel(topics_model);

    QSqlQueryModel* books_and_copies_model {new QSqlQueryModel};
    books_and_copies_model->setQuery("SELECT DISTINCT b.book_id, b.title, b.edition, ut.topic, "
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

    // Add filters
    QSortFilterProxyModel* title_filter_model {new QSortFilterProxyModel};
    title_filter_model->setSourceModel(books_and_copies_model);
    title_filter_model->setFilterKeyColumn(1);
    title_filter_model->setFilterCaseSensitivity(Qt::CaseInsensitive);
    connect(ui->titleLineEdit, &QLineEdit::editingFinished, title_filter_model, [=]() {
        title_filter_model->setFilterFixedString(ui->titleLineEdit->text());
    });

    QSortFilterProxyModel* author_filter_model {new QSortFilterProxyModel};
    author_filter_model->setSourceModel(title_filter_model);
    author_filter_model->setFilterKeyColumn(4);
    author_filter_model->setFilterCaseSensitivity(Qt::CaseInsensitive);
    connect(ui->authorLineEdit, &QLineEdit::editingFinished, author_filter_model, [=]() {
        author_filter_model->setFilterFixedString(ui->authorLineEdit->text());
    });

    QSortFilterProxyModel* topic_filter_model {new QSortFilterProxyModel};
    topic_filter_model->setSourceModel(author_filter_model);
    topic_filter_model->setFilterKeyColumn(3);
    connect(ui->topicComboBox, &QComboBox::currentTextChanged, topic_filter_model, [=]() {
        topic_filter_model->setFilterFixedString(ui->topicComboBox->currentText());
    });


    connect(ui->checkBox, &QCheckBox::clicked, ui->topicComboBox, [=]() {
        if (ui->checkBox->isChecked()) {
            ui->topicComboBox->setEnabled(true);
            topic_filter_model->setFilterFixedString(ui->topicComboBox->currentText());
        } else {
            ui->topicComboBox->setEnabled(false);
            topic_filter_model->setFilterFixedString(""); // remove filter
        }
    });

    ui->tableView->setModel(topic_filter_model);
    ui->tableView->verticalHeader()->setVisible(false); // remove indexes on leftside
    ui->tableView->setSortingEnabled(true);
    ui->tableView->sortByColumn(1, Qt::AscendingOrder); // sort by title (alphabetically)

    ui->tableView->setColumnHidden(0, true);
    ui->tableView->setColumnWidth(1, 250);
    ui->tableView->setColumnWidth(3, 200);
    ui->tableView->setColumnWidth(4, 150);
    ui->tableView->setMinimumWidth(ui->tableView->columnWidth(1)
                                 + ui->tableView->columnWidth(2)
                                 + ui->tableView->columnWidth(3)
                                 + ui->tableView->columnWidth(4)
                                 + ui->tableView->columnWidth(5)
                                 + 25);
}

ReaderMode::~ReaderMode()
{
    delete ui;
}

void ReaderMode::on_tableView_activated(const QModelIndex &index)
{
    const QString book_id {ui->tableView->model()->index(index.row(), 0).data().toString()};

    QSqlQueryModel* ids_model {new QSqlQueryModel};
    ids_model->setQuery("SELECT inventoryed_id FROM card "
                        "WHERE book_id = " + book_id);
    ui->idComboBox->setModel(ids_model);

    ui->pushButton->setEnabled(true);
}


void ReaderMode::on_pushButton_clicked()
{
    QSqlQuery query("INSERT INTO request (reader_id, inventoryed_id, request_date) VALUES ("
                  + m_reader_id + ", "
                  + ui->idComboBox->currentText() + ", "
                  + "CURDATE())");

    if (query.exec()) {
        QMessageBox::information(this, "Success", "The book was requested.");
    } else {
        QMessageBox::critical(this, "Error", "The book was NOT requested.");
    }
}

