#ifndef BOOKREQUESTWIDGET_H
#define BOOKREQUESTWIDGET_H

#include <QWidget>

namespace Ui {
class BookRequestWidget;
}

class BookRequestWidget : public QWidget
{
    Q_OBJECT

public:
    explicit BookRequestWidget(QWidget *parent = nullptr);
    ~BookRequestWidget();

private:
    Ui::BookRequestWidget *ui;
};

#endif // BOOKREQUESTWIDGET_H
