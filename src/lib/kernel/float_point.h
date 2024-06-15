#ifndef FLOAT_POINT
#define FLOAT_POINT

#define FLOAT_SHIFT_AMOUNT 17
#define Q 14
#define F mypow(Q)

// vai retornar apenas 2^p, pois tudo é na base 2
int mypow(int p);
typedef int float_type;

// converter integer to float_type
#define INT_FLOAT(n) ((float_type)(n * F))
// soma/sub entre dois float type
#define FLOAT_ADD(x, y) (x + y)
#define FLOAT_SUB(x, y) (x - y)
// multi/divide entre dois float type
#define FLOAT_MUL(x, y) ((float_type)(((int64_t) x) * (y / F)))
#define FLOAT_DIV(x, y) ((float_type)(((int64_t) x) * (F / y)))
// soma/sub entre float type(x) e integer(n)
#define FLOAT_INT_ADD(x, n) (x + (n * F))
#define FLOAT_INT_SUB(x, n) (x - (n * F))
// multi/div entre float type(x) e integer(n)
#define FLOAT_INT_MUL(x, n) (x * n)
#define FLOAT_INT_DIV(x, n) (x / n)

// converte x to integer, arredondando para zero
// nao sei se esse aqui funciona
#define FLOAT_INT_ZERO(x) ((int) (x >> FLOAT_SHIFT_AMOUNT)
// talvez trocar por isso, na doc ta assim
// #define FLOAT_INT_ZERO(x) ((int) (x / F)
// round de float
#define FLOAT_INT_ROUND(x) (x >= 0 ? ((A + (1 * mypow(Q - 1))) / F)) \
                                : ((x - (1 * mypow(Q - 1))) / F)
#endif
