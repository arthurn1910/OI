#ifndef FORWARDFUNCTION_H
#define FORWARDFUNCTION_H

#include "activationfunction.h"

class ForwardFunction : public ActivationFunction
{
public:
    ForwardFunction();
    double process(double input);
};

#endif // FORWARDFUNCTION_H
