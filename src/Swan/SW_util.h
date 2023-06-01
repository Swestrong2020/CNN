#ifndef SW_UTIL_H
#define SW_UTIL_H

#include <stdint.h>
#include <math.h>
#include <stdlib.h>

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

/* expects 'outputBuf' and 'input' to be of the same size 'nValues' */
static inline void SW_Softmax(float *input, float *outBuf, uint32_t nValues)
{
    float sum = 0;
    for (uint32_t i = 0; i < nValues; i++)
        sum += (outBuf[i] = exp(input[i]));
    for (uint32_t i = 0; i < nValues; i++)
        outBuf[i] /= sum;
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

/* WIP */
static inline float SW_Softmax_Derivative(float input)
{
    // TODO: Implement function
    return .0f;
}

/* expects 'p' and 'q' to be of the same size 'nValues' */
static inline float SW_CrossEntropy(float *p, float *q, uint32_t nValues)
{
    float r = 0;
    for (uint32_t i = 0; i < nValues; i++)
        r += p[i] * log2(q[i]);
    return -r;
}

static inline float SW_randf(float min, float max)
{
    float s = (float)(rand()) / (float)(RAND_MAX);
    return (s * (max - min)) + min;
}

#endif // SW_UTIL_H