#ifndef SUMMER_H
#define SUMMER_H

#include <QWidget>

namespace Ui {
class Summer;
}

class Summer : public QWidget
{
    Q_OBJECT

public:
    explicit Summer(QWidget *parent = 0);
    ~Summer();

private:
    Ui::Summer *ui;
};

#endif // SUMMER_H
