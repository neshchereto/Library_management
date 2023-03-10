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
                                         "WHERE ab.book_id = b.book_id) AS 'authors' "
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
    ui->tableView->setSortingEnabled(true);
    ui->tableView->sortByColumn(1, Qt::AscendingOrder); // sort by title (alphabetically)

    ui->tableView->verticalHeader()->setVisible(false); // remove indexes on leftside
    ui->tableView->setColumnHidden(0, true);
    ui->tableView->setColumnWidth(1, 250);
    ui->tableView->setColumnWidth(3, 200);
    ui->tableView->setColumnWidth(4, 150);
    ui->tableView->setMinimumWidth(ui->tableView->columnWidth(1)
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
    const QString book_id {ui->tableView->model()->index(index.row(), 0).data().toString()};
    const QString title   {ui->tableView->model()->index(index.row(), 1).data().toString()};

    QSqlQueryModel* ids_model {new QSqlQueryModel};
    // Reader can only see inventoryed ids that are not on others readers' hands
    ids_model->setQuery("SELECT c.inventoryed_id FROM card c "
                        "LEFT JOIN return_table rt ON rt.inventoryed_id = c.inventoryed_id "
                        "WHERE c.book_id = " + book_id + " AND rt.inventoryed_id IS NULL");
    ui->idComboBox->setModel(ids_model);

    ui->titleLabel->setText(title.left(22) + (title.count() > 22 ? "..." : "")); // Show first 22 chars of title
    ui->pushButton->setEnabled(true);
    ui->idComboBox->setEnabled(true);
}


void ReaderMode::on_pushButton_clicked()
{
    QSqlQuery query;
    query.prepare("INSERT INTO request (reader_id, inventoryed_id, request_date) VALUES ("
                + m_reader_id + ", "
                + ui->idComboBox->currentText() + ", "
                + "CURDATE())");

    if (query.exec()) {
        QMessageBox::information(this, "Success", "The book was requested.");
    } else {
        QMessageBox::critical(this, "Error", "The book was NOT requested.\nCheck connection to DB.");
    }
}
