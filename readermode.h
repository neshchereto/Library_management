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
    void on_tableView_activated(const QModelIndex &index);

private:
    Ui::ReaderMode *ui;
};

#endif // READERMODE_H
