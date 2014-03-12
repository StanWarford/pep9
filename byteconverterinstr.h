#ifndef BYTECONVERTERINSTR_H
#define BYTECONVERTERINSTR_H

#include <QWidget>

namespace Ui {
    class ByteConverterInstr;
}

class ByteConverterInstr : public QWidget {
    Q_OBJECT
public:
    ByteConverterInstr(QWidget *parent = 0);
    ~ByteConverterInstr();

    void setValue(int data);

protected:
    void changeEvent(QEvent *e);

private:
    Ui::ByteConverterInstr *ui;
};

#endif // BYTECONVERTERINSTR_H
