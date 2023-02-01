#ifndef REGISTRATIONDIALOG_H
#define REGISTRATIONDIALOG_H

#include <QDialog>

namespace Ui {
class RegistrationDialog;
}

class RegistrationDialog : public QDialog
{
    Q_OBJECT

public:
    explicit RegistrationDialog(QWidget *parent = nullptr);
    ~RegistrationDialog();

private slots:
    void onTextChanged();

    void on_buttonBox_accepted();

private:

    Ui::RegistrationDialog *ui;
};

#endif // REGISTRATIONDIALOG_H
