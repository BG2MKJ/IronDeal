#ifndef REGISTERATE_H
#define REGISTERATE_H

#include <QDialog>

namespace Ui {
class registerate;
}

class registerate : public QDialog
{
    Q_OBJECT

public:
    explicit registerate(QWidget *parent = nullptr);
    ~registerate();

private:
    Ui::registerate *ui;
};

#endif // REGISTERATE_H
