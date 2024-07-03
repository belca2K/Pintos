#ifndef THREADS_FIXED_POINT_H
#define THREADS_FIXED_POINT_H

typedef int fixed_point;    /* Fixed point number */
#define P 17                /* Integer bits */
#define Q 14                /* Fractional bits */
#define F (1 << Q)          /* 2^Q */

#define INT_TO_FLOAT(N) ((N) * F)
#define FLOAT_TO_INT_ZERO(X) ((X) / F)
#define FLOAT_TO_INT_NEAR(X) ((X) >= 0 ? ((X) + F / 2) / F : ((X) - F / 2) / F)

#define FLOAT_ADD(X, Y) ((X) + (Y))
#define FLOAT_ADD_MIX(X, N) ((X) + (N) * F)


#define FLOAT_SUB(X, Y) ((X) - (Y))
#define FLOAT_SUB_MIX(X, N) ((X) - (N) * F)

#define FLOAT_MULT(X, Y) (((int64_t) (X)) * (Y) / F)
#define FLOAT_MULT_MIX(X, N) ((X) * (N))

#define FLOAT_DIV(X, Y) (((int64_t) (X)) * F / (Y))
#define FLOAT_DIV_MIX(X, N) ((X) / (N))

#endif /* threads/fixed-point.h */