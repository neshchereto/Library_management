#include "inventorydialog.h"
#include "ui_inventorydialog.h"

#include <QSqlQueryModel>
#include <QPushButton>
#include <QMessageBox>
#include <QInputDialog>
#include <QSqlQuery>
#include <QTableView>

InventoryDialog::InventoryDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::InventoryDialog)
{
    ui->setupUi(this);

    ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);

    m_author_lineedits.append(ui->authorLineEdit_1);

    connect(ui->titleLineEdit   , SIGNAL(textChanged(QString)), this, SLOT(onTextChanged()));
    connect(ui->editionLineEdit , SIGNAL(textChanged(QString)), this, SLOT(onTextChanged()));
    connect(ui->authorLineEdit_1, SIGNAL(textChanged(QString)), this, SLOT(onTextChanged()));
}

InventoryDialog::~InventoryDialog()
{
    delete ui;
}

void InventoryDialog::on_buttonBox_accepted()
{
    QString title   {ui->titleLineEdit  ->text()};
    QString edition {ui->editionLineEdit->text()};

    // check if such a book is already in the library
    QSqlQuery query;
    query.exec("SELECT * FROM author_book ab "
               "JOIN book b ON ab.book_id = b.book_id "
               "JOIN author a ON ab.author_id = a.author_id "
               "WHERE a.full_name = '" + ui->authorLineEdit_1->text() +
               "' AND b.title = '"     + title +
               "' AND b.edition = '"   + edition + "'");

    // It's a first copy
    if (!query.next()) {
        QSqlQueryModel* model {new QSqlQueryModel};
        model->setQuery("SELECT * FROM udk_thematic");

        QTableView* view {new QTableView};
        view->setModel(model);
        view->setSizeAdjustPolicy(QAbstractScrollArea::AdjustToContents);
        view->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Maximum);
        view->show();

        bool ok;
        const int udk_id {QInputDialog::getInt(this, "FIRST COPY OF THE BOOK",
                                               "Enter UDK ID of book: ", 1, 1, 9, 1, &ok)};
        view->close();

        if (!ok) {
            return;
        }
        // Add the book
        QSqlQuery query2;
        query2.exec("INSERT INTO book (title, edition, udk_id) "
                    "VALUES ('" + title + "', '" + edition + "', " + QString::number(udk_id) +")");
    }

    // Connect the book and authors
    for (int i {}; i != m_author_lineedits.count(); ++i) {
        QString author {m_author_lineedits.at(i)->text()};

        if (!author.isEmpty()) {
            QSqlQuery query3;
            query3.exec("SELECT * FROM author WHERE full_name = '"
                       + author + "'");

            // If there is no such author in the table - add.
            if (!query3.next()) {
                QSqlQuery query4;
                query4.exec("INSERT INTO author (full_name) "
                           "VALUES ('" + author + "')");
            }

            // Connection auhtor_id-book_id
            QSqlQuery query5;
            query5.exec("INSERT INTO author_book (author_id, book_id) "
                        "VALUES ((SELECT author_id FROM author WHERE full_name = '"
                        + author + "'), "
                        + "(SELECT book_id FROM book "
                          "WHERE title = '" + title
                        + "' AND edition = '" + edition + "'))");
        }
    }

    // Inventorize copies
    bool ok;
    const int number_of_copies {QInputDialog::getInt(this, "Copies",
                                                    "Enter number of copies: ", 1, 1, 500, 1, &ok)};

    if (ok) {
        for (int i {}; i != number_of_copies; ++i) {
            QSqlQuery query6;
            query6.exec("INSERT INTO card (book_id) "
                        "VALUES ((SELECT book_id FROM book "
                        "WHERE title = '" + title
                        + "' AND edition = '" + edition + "'))");
        }

        QMessageBox::information(this, "Success", "New book was inserted.");
    }
}


void InventoryDialog::onTextChanged()
{
    ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(!ui->titleLineEdit   ->text().isEmpty()
                                                         && !ui->editionLineEdit ->text().isEmpty()
                                                         && !ui->authorLineEdit_1->text().isEmpty());
}

void InventoryDialog::on_moreAuhtorsPushButton_clicked()
{
    static int author_counter{1};

    QLineEdit* another_lineedit {new QLineEdit};
    QLabel*    another_label    {new QLabel};

    m_author_lineedits.append(another_lineedit);

    switch (++author_counter) {
    case 2:
        another_lineedit->setObjectName("authorLineEdit_2");
        break;
    case 3:
        another_lineedit->setObjectName("authorLineEdit_3");
        break;
    case 4:
        another_lineedit->setObjectName("authorLineEdit_4");
        break;
    case 5:
        another_lineedit->setObjectName("authorLineEdit_5");
        break;
    }

    another_label->setText("Author #" + QString::number(author_counter));

    ui->formLayout->addRow(another_label, another_lineedit);

    // If there are 5 author line edits, button sets unenabled
    if (author_counter == 5) {
        ui->moreAuhtorsPushButton->setEnabled(false);
    }
}

