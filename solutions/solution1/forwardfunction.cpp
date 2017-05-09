#include "forwardfunction.h"

ForwardFunction::ForwardFunction() : ActivationFunction()
{

}

double ForwardFunction::process(double input)
{
    return 10 * input;
}
