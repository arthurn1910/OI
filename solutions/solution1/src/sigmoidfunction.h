#ifndef SIGMOIDFUNCTION_H
#define SIGMOIDFUNCTION_H

#include "activationfunction.h"


class SigmoidFunction : public ActivationFunction {
public:
    double beta = 1;

    SigmoidFunction();
    SigmoidFunction(double beta);

    double process(double sum);

};

#endif // SIGMOIDFUNCTION_H
