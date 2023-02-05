#ifndef READERINFOWIDGET_H
#define READERINFOWIDGET_H

#include <QWidget>

namespace Ui {
class ReaderInfoWidget;
}

class ReaderInfoWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ReaderInfoWidget(QWidget *parent = nullptr);
    ~ReaderInfoWidget();

private slots:
    void on_readersTableView_activated(const QModelIndex &index);

private:

    Ui::ReaderInfoWidget *ui;
};

#endif // READERINFOWIDGET_H
