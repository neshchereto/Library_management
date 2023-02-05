#ifndef BOOKHOLDERWIDGET_H
#define BOOKHOLDERWIDGET_H

#include <QSqlQueryModel>
#include <QWidget>

namespace Ui {
class BookHolderWidget;
}

class BookHolderWidget : public QWidget
{
    Q_OBJECT

public:
    explicit BookHolderWidget(QWidget *parent = nullptr);
    ~BookHolderWidget();

private slots:
    void on_tableView_activated(const QModelIndex &index);

private:
    Ui::BookHolderWidget *ui;
};

#endif // BOOKHOLDERWIDGET_H
