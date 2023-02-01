#ifndef INVENTORYDIALOG_H
#define INVENTORYDIALOG_H

#include <QDialog>
#include <QLineEdit>

namespace Ui {
class InventoryDialog;
}

class InventoryDialog : public QDialog
{
    Q_OBJECT

public:
    explicit InventoryDialog(QWidget *parent = nullptr);
    ~InventoryDialog();

private slots:
    void onTextChanged();

    void on_buttonBox_accepted();

    void on_moreAuhtorsPushButton_clicked();

private:
    QList<QLineEdit*> m_author_lineedits;

    Ui::InventoryDialog *ui;
};

#endif // INVENTORYDIALOG_H
