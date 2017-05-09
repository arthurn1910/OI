#include "sigmoidfunction.h"
#include <cmath>
#include <QDebug>

SigmoidFunction::SigmoidFunction() {
    beta = 0.01;
}

SigmoidFunction::SigmoidFunction(double beta) {
    this->beta = 1;
}

double SigmoidFunction::activate(double sum) {
    return (1 / (1 + std::exp((-1) * beta * sum)));
}

double SigmoidFunction::derivativeActivate(double sum) {
    double value = activate(sum);

    return beta * value * (1 - value);
}
