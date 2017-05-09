#ifndef ACTIVATIONFUNCTION_H
#define ACTIVATIONFUNCTION_H


class ActivationFunction
{
public:
    ActivationFunction();
    ~ActivationFunction();
    virtual double process(double input) = 0;
};

#endif // ACTIVATIONFUNCTION_H
