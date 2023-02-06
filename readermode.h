#ifndef READERMODE_H
#define READERMODE_H

#include <QWidget>

namespace Ui {
class ReaderMode;
}

class ReaderMode : public QWidget
{
    Q_OBJECT

public:
    explicit ReaderMode(QWidget *parent = nullptr);
    ~ReaderMode();

private slots:
    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_4_clicked();

    void on_pushButton_5_clicked();

    void on_pushButton_6_clicked();

    void on_pushButton_7_clicked();

    void on_pushButton_8_clicked();

private:
    Ui::ReaderMode *ui;
};

#endif // READERMODE_H
