#ifndef REQUESTWIDGET_H
#define REQUESTWIDGET_H

#include <QWidget>

namespace Ui {
class RequestWidget;
}

class RequestWidget : public QWidget
{
    Q_OBJECT

public:
    explicit RequestWidget(QWidget *parent = nullptr);
    ~RequestWidget();

private slots:
    void loadTable();

    void on_acceptPushButton_clicked();

private:
    Ui::RequestWidget *ui;
};

#endif // REQUESTWIDGET_H
