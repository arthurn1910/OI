#ifndef ACTIVATIONFUNCTION_H
#define ACTIVATIONFUNCTION_H

class ActivationFunction {
public:
    virtual double activate(double sum) = 0;
    virtual double derivativeActivate(double sum) = 0;
};

#endif // ACTIVATIONFUNCTION_H
