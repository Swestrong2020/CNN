#ifndef SW_UTIL_H
#define SW_UTIL_H

#include <math.h>

static inline float SW_Sigmoid(float input)
{
    return 1.0f / (1.0f + expf(-input));
}

static inline float SW_ReLu(float input)
{
    if (input > .0f)
        return input;
    else
        return .0f;
}

static inline float SW_Softmax(float input)
{
    // TODO: Implement function
    return .0f;
}

static inline float SW_Tanh(float input)
{
    return tanh(input) * 0.5f + 0.5f;
}

static inline float SW_Sigmoid_Derivative(float input)
{
    return input * (1.0f - input);
}

static inline float SW_Tanh_Derivative(float input)
{
    return 1 - input * input;
}

static inline float SW_ReLu_Derivative(float input)
{
    return (input > 0.0f) ? 1.0f : 0.0f;
}

static inline float SW_Softmax_Derivative(float input)
{
    // TODO: Implement function
    return .0f;
}


#endif // SW_UTIL_H