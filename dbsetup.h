#ifndef DBSETUP_H
#define DBSETUP_H

#include <QDialog>

namespace Ui {
class DBSetup;
}

class DBSetup : public QDialog
{
    Q_OBJECT

public:
    bool connected {false};

    explicit DBSetup(QWidget *parent = nullptr);
    ~DBSetup();

private slots:
    void on_buttonBox_accepted();

private:
    Ui::DBSetup *ui;
};

#endif // DBSETUP_H
