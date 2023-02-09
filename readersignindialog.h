#ifndef READERSIGNINDIALOG_H
#define READERSIGNINDIALOG_H

#include <QDialog>

namespace Ui {
class ReaderSignInDialog;
}

class ReaderSignInDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ReaderSignInDialog(QWidget *parent = nullptr);
    ~ReaderSignInDialog();

    QString m_reader_id;

private slots:
    void on_buttonBox_accepted();

    void onTextChanged();

    void on_pushButton_clicked();

private:
    Ui::ReaderSignInDialog *ui;
};

#endif // READERSIGNINDIALOG_H
