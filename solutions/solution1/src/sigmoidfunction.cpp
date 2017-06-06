#include "sigmoidfunction.h"
#include <cmath>
#include <QDebug>

SigmoidFunction::SigmoidFunction() {
    beta = 0.01;
}

SigmoidFunction::SigmoidFunction(double beta) {
    this->beta = beta;
}

double SigmoidFunction::process(double sum) {
    return (1 / (1 + std::exp((-1) * beta * sum)));
}
