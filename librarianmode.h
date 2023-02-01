#ifndef LIBRARIANMODE_H
#define LIBRARIANMODE_H

#include <QWidget>

namespace Ui {
class LibrarianMode;
}

class LibrarianMode : public QWidget
{
    Q_OBJECT

public:
    explicit LibrarianMode(QWidget *parent = nullptr);
    ~LibrarianMode();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_5_clicked();

    void on_pushButton_6_clicked();

    void on_pushButton_7_clicked();

    void on_pushButton_8_clicked();

private:
    Ui::LibrarianMode *ui;
};

#endif // LIBRARIANMODE_H
