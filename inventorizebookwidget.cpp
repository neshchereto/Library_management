#include "inventorizebookwidget.h"
#include "ui_inventorizebookwidget.h"

#include <QDialog>
#include <QDialogButtonBox>
#include <QSqlError>
#include <QSqlQueryModel>
#include <QPushButton>
#include <QMessageBox>
#include <QInputDialog>
#include <QSqlQuery>
#include <QTableView>

InventorizeBookWidget::InventorizeBookWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::InventorizeBookWidget)
{
    ui->setupUi(this);

    m_author_lineedits.append(ui->authorLineEdit_1);

    connect(ui->titleLineEdit   , SIGNAL(textChanged(QString)), this, SLOT(onTextChanged()));
    connect(ui->editionLineEdit , SIGNAL(textChanged(QString)), this, SLOT(onTextChanged()));
    connect(ui->authorLineEdit_1, SIGNAL(textChanged(QString)), this, SLOT(onTextChanged()));
}

InventorizeBookWidget::~InventorizeBookWidget()
{
    delete ui;
}

void InventorizeBookWidget::onTextChanged()
{
    ui->pushButton->setEnabled(!ui->titleLineEdit   ->text().isEmpty()
                            && !ui->editionLineEdit ->text().isEmpty()
                            && !ui->authorLineEdit_1->text().isEmpty());
}

void InventorizeBookWidget::on_moreAuhtorsPushButton_clicked()
{
    static int author_counter {1};

    QLineEdit* another_lineedit {new QLineEdit};
    QLabel   * another_label    {new QLabel};

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


void InventorizeBookWidget::on_pushButton_clicked()
{
    QString title   {ui->titleLineEdit  ->text()};
    QString edition {ui->editionLineEdit->text()};

    // check if such a book is already in the library
    QSqlQuery query{"SELECT * FROM author_book ab "
                    "JOIN book b ON ab.book_id = b.book_id "
                    "JOIN author a ON ab.author_id = a.author_id "
                    "WHERE a.full_name = '" + ui->authorLineEdit_1->text() +
                    "' AND b.title = '"     + title +
                    "' AND b.edition = '"   + edition + "'"};

    // It's a first copy
    if (!query.next()) {
        // Choose the book's topic Dialog
        QDialog* dialog {new QDialog};

        QDialogButtonBox* buttons {new QDialogButtonBox(QDialogButtonBox::Ok
                                                     | QDialogButtonBox::Cancel)};
        buttons->button(QDialogButtonBox::Ok)->setEnabled(false);
        connect(buttons, &QDialogButtonBox::accepted, dialog, &QDialog::accept);
        connect(buttons, &QDialogButtonBox::rejected, dialog, &QDialog::reject);

        QSqlQueryModel* model {new QSqlQueryModel};
        model->setQuery("SELECT topic FROM udk_thematic");
        QTableView* view {new QTableView};
        view->setModel(model);
        view->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        view->setColumnWidth(0, 250);
        view->setMinimumHeight(view->rowHeight(0)
                               * model->rowCount()
                               + 50);
        view->setMinimumWidth(view->columnWidth(0) + 25);
        dialog->setSizePolicy(QSizePolicy::MinimumExpanding,
                              QSizePolicy::MinimumExpanding);

        QVBoxLayout* layout {new QVBoxLayout(dialog)};
        layout->addWidget(view);
        layout->addWidget(buttons);

        int udk_id;
        connect(view, &QTableView::activated, this, [=]() {
            buttons->button(QDialogButtonBox::Ok)->setEnabled(true);
        });

        if (dialog->exec() != QDialog::Accepted) {
            return;
        } else {
            udk_id = view->currentIndex().row() + 1;
        }


        // Add the book
        QSqlQuery query2;
        query2.prepare("INSERT INTO book (title, edition, udk_id) "
                       "VALUES ('" + title + "', '"
                                   + edition + "', "
                                   + QString::number(udk_id) +")");
        if (!query2.exec()) {
            QMessageBox::critical(this, "Error", query2.lastError().text());
            return;
        }
    }

    // Connect the book and authors
    for (int i {}; i != m_author_lineedits.count(); ++i) {
        QString author {m_author_lineedits.at(i)->text()};

        if (!author.isEmpty()) {
            QSqlQuery query3{"SELECT * FROM author WHERE full_name = '"
                             + author + "'"};
            // If there is no such author in the table - add.
            if (!query3.next()) {
                QSqlQuery query4;
                query4.prepare("INSERT INTO author (full_name) "
                               "VALUES ('" + author + "')");
                if (!query4.exec()) {
                    QMessageBox::critical(this, "Error", query4.lastError().text());
                    return;
                }
            }

            // Connection auhtor_id-book_id
            QSqlQuery query5;
            query5.prepare("INSERT INTO author_book (author_id, book_id) "
                           "VALUES ((SELECT author_id FROM author WHERE full_name = '"
                         + author + "'), "
                         + "(SELECT book_id FROM book "
                           "WHERE title = '" + title
                         + "' AND edition = '" + edition + "'))");
            if (!query5.exec()) {
                QMessageBox::critical(this, "Error", query5.lastError().text());
                return;
            }
        }
    }

    // Inventorize copies
    bool ok;
    const int number_of_copies {QInputDialog::getInt(this, "Copies",
                                                    "Enter number of copies: ", 1, 1, 500, 1, &ok)};

    if (ok) {
        bool inserted {true};
        QSqlQuery query6;
        for (int i {}; i != number_of_copies; ++i) {
            query6.prepare("INSERT INTO card (book_id) "
                           "VALUES ((SELECT book_id FROM book "
                           "WHERE title = '" + title
                         + "' AND edition = '" + edition + "'))");
            if (!query6.exec()) {
                inserted = false;
                break;
            }
        }

        if (inserted) {
            QMessageBox::information(this, "Success", "The book was added.");
        } else {
            QMessageBox::critical(this, "Error", query6.lastError().text());
        }
    }
}

