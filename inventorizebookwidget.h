#ifndef INVENTORIZEBOOKWIDGET_H
#define INVENTORIZEBOOKWIDGET_H

#include <QWidget>
#include <QLineEdit>

namespace Ui {
class InventorizeBookWidget;
}

class InventorizeBookWidget : public QWidget
{
    Q_OBJECT

public:
    explicit InventorizeBookWidget(QWidget *parent = nullptr);
    ~InventorizeBookWidget();

private slots:
    void onTextChanged();

    void on_moreAuhtorsPushButton_clicked();

    void on_pushButton_clicked();

private:
    QList<QLineEdit*> m_author_lineedits;

    Ui::InventorizeBookWidget *ui;
};

#endif // INVENTORIZEBOOKWIDGET_H
