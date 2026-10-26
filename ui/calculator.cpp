//
// Created by Filip on 25.10.2026.
//

// You may need to build the project (run Qt uic code generator) to get "ui_Calculator.h" resolved

#include "calculator.h"

#include <QPushButton>

#include "ui_Calculator.h"
#include <QKeyEvent>


Calculator::Calculator(QWidget *parent) : QWidget(parent), ui(new Ui::Calculator) {
    ui->setupUi(this);
    setWindowFlags(Qt::Window | Qt::WindowStaysOnTopHint);
    makeConnects();
}

Calculator::~Calculator() {
    delete ui;
}

void Calculator::showEvent(QShowEvent *e) {
    setFocus();
    raise();
    activateWindow();
    QWidget::showEvent(e);
}

void Calculator::keyPressEvent(QKeyEvent *e) {
    if (e->key() == Qt::Key_0) {
        emit ui->b_0->clicked();
    } else if (e->key() == Qt::Key_1) {
        emit ui->b_1->clicked();
    } else if (e->key() == Qt::Key_2) {
        emit ui->b_2->clicked();
    } else if (e->key() == Qt::Key_3) {
        emit ui->b_3->clicked();
    } else if (e->key() == Qt::Key_4) {
        emit ui->b_4->clicked();
    } else if (e->key() == Qt::Key_5) {
        emit ui->b_5->clicked();
    } else if (e->key() == Qt::Key_6) {
        emit ui->b_6->clicked();
    } else if (e->key() == Qt::Key_7) {
        emit ui->b_7->clicked();
    } else if (e->key() == Qt::Key_8) {
        emit ui->b_8->clicked();
    } else if (e->key() == Qt::Key_9) {
        emit ui->b_9->clicked();
    } else if (e->key() == Qt::Key_Return || e->key() == Qt::Key_Enter) {
        emit ui->b_equal->clicked();
    } else if (e->key() == Qt::Key_Minus) {
        emit ui->b_minus->clicked();
    } else if (e->key() == Qt::Key_Plus) {
        emit ui->b_plus->clicked();
    } else if (e->key() == Qt::Key_Asterisk) {
        emit ui->b_multiple->clicked();
    } else if (e->key() == Qt::Key_Slash) {
        emit ui->b_divide->clicked();
    } else if (e->key() == Qt::Key_Backspace) {
        emit ui->b_backspace->clicked();
    } else if (e->key() == Qt::Key_Delete) {
        emit ui->b_C->clicked();
    } else if (e->key() == Qt::Key_Comma) {
        emit ui->b_coma->clicked();
    } else {
        QWidget::keyPressEvent(e);
    }
}

void Calculator::makeConnects() {
    connect(ui->b_0, &QPushButton::clicked, [this] { addDigitToNumber('0'); });
    connect(ui->b_1, &QPushButton::clicked, [this] { addDigitToNumber('1'); });
    connect(ui->b_2, &QPushButton::clicked, [this] { addDigitToNumber('2'); });
    connect(ui->b_3, &QPushButton::clicked, [this] { addDigitToNumber('3'); });
    connect(ui->b_4, &QPushButton::clicked, [this] { addDigitToNumber('4'); });
    connect(ui->b_5, &QPushButton::clicked, [this] { addDigitToNumber('5'); });
    connect(ui->b_6, &QPushButton::clicked, [this] { addDigitToNumber('6'); });
    connect(ui->b_7, &QPushButton::clicked, [this] { addDigitToNumber('7'); });
    connect(ui->b_8, &QPushButton::clicked, [this] { addDigitToNumber('8'); });
    connect(ui->b_9, &QPushButton::clicked, [this] { addDigitToNumber('9'); });

    connect(ui->b_C, &QPushButton::clicked, this, &Calculator::clear);

    connect(ui->b_plus, &QPushButton::clicked, [this] {
        operation = Operation::Add;
        calculate();
    });

    connect(ui->b_minus, &QPushButton::clicked, this, &Calculator::minusClicked);

    connect(ui->b_multiple, &QPushButton::clicked, [this] {
        operation = Operation::Multiply;
        calculate();
    });

    connect(ui->b_divide, &QPushButton::clicked, [this] {
        operation = Operation::Divide;
        calculate();
    });

    connect(ui->b_pow, &QPushButton::clicked, [this] {
        operation = Operation::Power;
        calculate();
    });

    connect(ui->b_equal, &QPushButton::clicked, this, &Calculator::calculateFromEqual);
    connect(ui->b_negate, &QPushButton::clicked, this, &Calculator::negateRightHand);
    connect(ui->b_backspace, &QPushButton::clicked, this, &Calculator::deleteLastChar);

    connect(ui->b_coma, &QPushButton::clicked, this, &Calculator::addComa);
}

void Calculator::addDigitToNumber(QChar digit) {
    if (!error.isEmpty()) {
        rightHand = digit;
        error = "";
    } else if (operation == Operation::Equal) {
        operation = Operation::None;
        rightHand = digit;
        error = "";
    } else {
        if (rightHand == "0") {
            if (digit == '0') {
                return;
            } else {
                rightHand = digit;
            }
        } else {
            rightHand = rightHand + digit;
        }
    }
    refreshLcd();
}

void Calculator::refreshLcd() {
    QString tmp = error + " " + leftHand;
    if (operation == Operation::Add) {
        tmp += " + ";
    } else if (operation == Operation::Subtract) {
        tmp += " - ";
    } else if (operation == Operation::Multiply) {
        tmp += " * ";
    } else if (operation == Operation::Divide) {
        tmp += " / ";
    } else if (operation == Operation::Power) {
        tmp += " ^ ";
    }
    tmp += rightHand;

    ui->lcdScreen->setText(tmp);
}

void Calculator::addComa() {
    if (operation != Operation::Equal) {
        if (rightHand.isEmpty()) {
            rightHand = "0.";
        } else {
            if (!rightHand.contains('.')) {
                rightHand += ".";
            }
        }
        refreshLcd();
    }
}

void Calculator::calculate() {
    if (error.isEmpty()) {
        if (leftHand.isEmpty()) {
            leftHand = rightHand;
            rightHand = "";
        } else if (rightHand.isEmpty() || rightHand == "-") {
            //only refresh
        } else if (operation != Operation::None && operation != Operation::Equal) {
            if (operation == Operation::Add) {
                operationAdd(leftHand);
            } else if (operation == Operation::Subtract) {
                operationSubtract(leftHand);
            } else if (operation == Operation::Multiply) {
                operationMultiply(rightHand);
            } else if (operation == Operation::Divide) {
                operationDivide(rightHand);
            } else if (operation == Operation::Power) {
                operationPow(rightHand);
            }

            rightHand = "";
        }
        refreshLcd();
    }
}

void Calculator::clear() {
    error = "";
    leftHand = "";
    rightHand = "0";
    operation = Operation::None;
    refreshLcd();
}

void Calculator::deleteLastChar() {
    if (!rightHand.isEmpty()) {
        rightHand.chop(1);
    }
    refreshLcd();
}

void Calculator::negateRightHand() {
    if (!rightHand.isEmpty() && error.isEmpty()) {
        if (rightHand[0] == '-') {
            rightHand.removeAt(0);
        } else {
            rightHand.prepend('-');
        }
    }
    refreshLcd();
}

void Calculator::calculateFromEqual() {
    if (leftHand.isEmpty()) {

    } else if (operation != Operation::None && operation != Operation::Equal) {
        if (operation == Operation::Add) {
            operationAdd(rightHand);
        } else if (operation == Operation::Subtract) {
            operationSubtract(rightHand);
        } else if (operation == Operation::Multiply) {
            operationMultiply(rightHand);
        } else if (operation == Operation::Divide) {
            operationDivide(rightHand);
        } else if (operation == Operation::Power) {
            operationPow(rightHand);
        }

        leftHand = "";
        operation = Operation::Equal;
    }

    refreshLcd();
}

void Calculator::minusClicked() {
    if (rightHand.isEmpty()) {
        rightHand = "-";
        refreshLcd();
    } else if ((operation == Operation::None || operation == Operation::Equal) && rightHand == "0") {
        rightHand = "-";
        refreshLcd();
    } else {
        operation = Operation::Subtract;
        calculate();
    }
}

void Calculator::operationAdd(QString &result) {
    bool isOKA = false;
    bool isOKB = false;
    double a = leftHand.toDouble(&isOKA);
    double b = rightHand.toDouble(&isOKB);
    if (isOKA && isOKB) {
        result = QString::number(a + b, 'g', 12);
    } else {
        result = "";
        error = "Parse error";
    }
}

void Calculator::operationSubtract(QString &result) {
    bool isOKA = false;
    bool isOKB = false;
    double a = leftHand.toDouble(&isOKA);
    double b = rightHand.toDouble(&isOKB);
    if (isOKA && isOKB) {
        result = QString::number(a - b, 'g', 12);
    } else {
        result = "";
        error = "Parse error";
    }
}

void Calculator::operationMultiply(QString &result) {
    bool isOKA = false;
    bool isOKB = false;
    double a = leftHand.toDouble(&isOKA);
    double b = rightHand.toDouble(&isOKB);
    if (isOKA && isOKB) {
        result = QString::number(a * b, 'g', 12);
    } else {
        result = "";
        error = "Parse error";
    }
}

void Calculator::operationDivide(QString &result) {
    bool isOKA = false;
    bool isOKB = false;
    double a = leftHand.toDouble(&isOKA);
    double b = rightHand.toDouble(&isOKB);
    if (b == 0.0) {
        result = "";
        error = tr("Division by zero!");
    } else {
        if (isOKA && isOKB) {
            result = QString::number(a / b, 'g', 12);
        } else {
            result = "";
            error = "Parse error";
        }
    }
}

void Calculator::operationPow(QString &result) {
    bool isOKA = false;
    bool isOKB = false;
    double a = leftHand.toDouble(&isOKA);
    double b = rightHand.toDouble(&isOKB);
    if (isOKA && isOKB) {
        result = QString::number(std::pow(a, b), 'g', 12);
    } else {
        result = "";
        error = "Parse error";
    }
}
