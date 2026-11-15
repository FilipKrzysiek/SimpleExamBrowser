//
// Created by Filip on 25.10.2026.
//

#ifndef SIMPLEEXAMBROWSER_CALCULATOR_H
#define SIMPLEEXAMBROWSER_CALCULATOR_H

#include <QWidget>
#include <QJSEngine>


QT_BEGIN_NAMESPACE

namespace Ui {
    class Calculator;
}

QT_END_NAMESPACE

class Calculator : public QWidget {
    Q_OBJECT

public:
    explicit Calculator(QWidget *parent = nullptr);

    ~Calculator() override;

protected:
    void showEvent(QShowEvent *e) override;

    void keyPressEvent(QKeyEvent *e) override;

private:
    enum class Operation {
        None,
        Add,
        Subtract,
        Multiply,
        Divide,
        Power,
        Sqrt,
        Equal,
    };

    Ui::Calculator *ui;
    QString leftHand = "";
    QString rightHand = "0";
    QString error = "";
    Operation operation = Operation::None;


    void makeConnects();

    void addDigitToNumber(QChar digit);

    void refreshLcd();

    void operationAdd(QString &result);
    void operationSubtract(QString &result);
    void operationMultiply(QString &result);
    void operationDivide(QString &result);
    void operationPow(QString &result);

private slots:
    void addComa();

    void calculate();

    void clear();

    void deleteLastChar();

    void negateRightHand();

    void calculateFromEqual();

    void minusClicked();
};


#endif //SIMPLEEXAMBROWSER_CALCULATOR_H
