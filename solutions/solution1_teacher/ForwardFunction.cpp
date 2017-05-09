#include "forwardfunction.h"
ForwardFunction::ForwardFunction()
{

}

double ForwardFunction::activate(double sum) {
    return sum;
}

double ForwardFunction::derivativeActivate(double sum) {
    return 1.0;
}
