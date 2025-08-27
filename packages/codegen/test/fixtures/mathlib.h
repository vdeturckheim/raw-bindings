#ifndef MATHLIB_H
#define MATHLIB_H

#ifdef __cplusplus
extern "C" {
#endif

// Basic arithmetic operations
int add(int a, int b);
int subtract(int a, int b);
int multiply(int a, int b);
double divide(double a, double b);

// More math operations
int factorial(int n);
double power(double base, int exponent);
double square_root(double x);

// Constants
extern const double PI;
extern const double E;

// Simple struct for 2D point
typedef struct {
    double x;
    double y;
} Point2D;

// Operations on points
Point2D create_point(double x, double y);
double distance(Point2D p1, Point2D p2);
Point2D add_points(Point2D p1, Point2D p2);

// Enum for operation types
typedef enum {
    OP_ADD = 0,
    OP_SUBTRACT = 1,
    OP_MULTIPLY = 2,
    OP_DIVIDE = 3
} MathOperation;

// Function that uses enum
double calculate(double a, double b, MathOperation op);

#ifdef __cplusplus
}
#endif

#endif // MATHLIB_H