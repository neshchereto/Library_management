#ifndef INVENTORIZECOPIESWIDGET_H
#define INVENTORIZECOPIESWIDGET_H

#include <QWidget>

namespace Ui {
class InventorizeCopiesWidget;
}

class InventorizeCopiesWidget : public QWidget
{
    Q_OBJECT

public:
    explicit InventorizeCopiesWidget(QWidget *parent = nullptr);
    ~InventorizeCopiesWidget();

private slots:
    void on_tableView_activated(const QModelIndex &index);

    void on_inventorizePushButton_clicked();

private:
    Ui::InventorizeCopiesWidget *ui;
};

#endif // INVENTORIZECOPIESWIDGET_H
