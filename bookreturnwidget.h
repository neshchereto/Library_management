#ifndef BOOKRETURNWIDGET_H
#define BOOKRETURNWIDGET_H

#include <QWidget>

namespace Ui {
class BookReturnWidget;
}

class BookReturnWidget : public QWidget
{
    Q_OBJECT

public:
    explicit BookReturnWidget(QWidget *parent = nullptr);
    ~BookReturnWidget();

private slots:
    void on_tableView_activated(const QModelIndex &index);

    void on_returnPushButton_clicked();

private:
    void loadTable();

    Ui::BookReturnWidget *ui;
};

#endif // BOOKRETURNWIDGET_H
