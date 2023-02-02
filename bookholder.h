#ifndef BOOKHOLDER_H
#define BOOKHOLDER_H

#include <QSqlQueryModel>
#include <QWidget>

namespace Ui {
class BookHolder;
}

class BookHolder : public QWidget
{
    Q_OBJECT

public:
    explicit BookHolder(QWidget *parent = nullptr);
    ~BookHolder();

private slots:
    void on_tableView_activated(const QModelIndex &index);

private:
    Ui::BookHolder *ui;
};

#endif // BOOKHOLDER_H
