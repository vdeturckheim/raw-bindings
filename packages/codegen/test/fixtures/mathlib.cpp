#include "mathlib.h"
#include <cmath>

// Constants
const double PI = 3.14159265358979323846;
const double E = 2.71828182845904523536;

// Basic arithmetic operations
int add(int a, int b) {
    return a + b;
}

int subtract(int a, int b) {
    return a - b;
}

int multiply(int a, int b) {
    return a * b;
}

double divide(double a, double b) {
    if (b == 0) {
        return 0; // Simple error handling
    }
    return a / b;
}

// More math operations
int factorial(int n) {
    if (n <= 1) return 1;
    int result = 1;
    for (int i = 2; i <= n; i++) {
        result *= i;
    }
    return result;
}

double power(double base, int exponent) {
    double result = 1.0;
    int abs_exp = exponent < 0 ? -exponent : exponent;
    
    for (int i = 0; i < abs_exp; i++) {
        result *= base;
    }
    
    return exponent < 0 ? 1.0 / result : result;
}

double square_root(double x) {
    return sqrt(x);
}

// Point operations
Point2D create_point(double x, double y) {
    Point2D p;
    p.x = x;
    p.y = y;
    return p;
}

double distance(Point2D p1, Point2D p2) {
    double dx = p2.x - p1.x;
    double dy = p2.y - p1.y;
    return sqrt(dx * dx + dy * dy);
}

Point2D add_points(Point2D p1, Point2D p2) {
    Point2D result;
    result.x = p1.x + p2.x;
    result.y = p1.y + p2.y;
    return result;
}

// Function that uses enum
double calculate(double a, double b, MathOperation op) {
    switch (op) {
        case OP_ADD:
            return a + b;
        case OP_SUBTRACT:
            return a - b;
        case OP_MULTIPLY:
            return a * b;
        case OP_DIVIDE:
            return divide(a, b);
        default:
            return 0;
    }
}