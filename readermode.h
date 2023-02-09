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
    explicit ReaderMode(const QString& reader_id, QWidget *parent = nullptr);
    ~ReaderMode();

private slots:
    void on_tableView_activated(const QModelIndex &index);

    void on_pushButton_clicked();

private:
    const QString m_reader_id;

    Ui::ReaderMode *ui;
};

#endif // READERMODE_H
