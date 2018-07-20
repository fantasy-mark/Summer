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

public slots:
    void git_create();
    void git_clone();
    void git_pull();
    void git_add();
    void git_commit();
    void git_addRemote();
    void git_push();

private:
    Ui::Summer *ui;
};

#endif // SUMMER_H
