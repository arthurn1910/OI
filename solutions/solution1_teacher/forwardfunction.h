#ifndef FORWARDFUNCTION_H
#define FORWARDFUNCTION_H

#include "activationfunction.h"

class ForwardFunction : public ActivationFunction {
public:
    ForwardFunction();
    double activate(double sum);
    double derivativeActivate(double sum);
};

#endif // FORWARDFUNCTION_H
