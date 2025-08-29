#include "mathlib.h"
#include <cmath>
#include <cstring>
#include <cstdlib>

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

// Circle functions
Circle create_circle(Point2D center, double radius) {
    Circle c;
    c.center = center;
    c.radius = radius;
    return c;
}

double circle_area(Circle c) {
    return PI * c.radius * c.radius;
}

int circle_contains_point(Circle c, Point2D p) {
    double dist = distance(c.center, p);
    return dist <= c.radius ? 1 : 0;
}

// Union functions
NumberUnion create_int_union(int value) {
    NumberUnion u;
    u.i = value;
    return u;
}

NumberUnion create_double_union(double value) {
    NumberUnion u;
    u.d = value;
    return u;
}

int get_int_from_union(NumberUnion u) {
    return u.i;
}

double get_double_from_union(NumberUnion u) {
    return u.d;
}

// DataArray functions
DataArray create_data_array(const char* name) {
    DataArray arr;
    strncpy(arr.name, name ? name : "unnamed", 31);
    arr.name[31] = '\0';
    arr.count = 0;
    for (int i = 0; i < 10; i++) {
        arr.values[i] = 0.0;
    }
    return arr;
}

void add_value_to_array(DataArray* arr, double value) {
    if (arr && arr->count < 10) {
        arr->values[arr->count] = value;
        arr->count++;
    }
}

double sum_array_values(DataArray* arr) {
    if (!arr) return 0.0;
    double sum = 0.0;
    for (int i = 0; i < arr->count; i++) {
        sum += arr->values[i];
    }
    return sum;
}

// Opaque context implementation
struct OpaqueContext {
    int internal_value;
    void* internal_data;
};

ContextHandle create_context(void) {
    ContextHandle ctx = (ContextHandle)malloc(sizeof(struct OpaqueContext));
    if (ctx) {
        ctx->internal_value = 42;
        ctx->internal_data = nullptr;
    }
    return ctx;
}

void destroy_context(ContextHandle ctx) {
    if (ctx) {
        free(ctx);
    }
}

int context_do_something(ContextHandle ctx, int value) {
    if (!ctx) return -1;
    return ctx->internal_value + value;
}

// Typedef'd type functions
Scalar scalar_multiply(Scalar a, Scalar b) {
    return a * b;
}

Integer integer_add(Integer a, Integer b) {
    return a + b;
}

// Complex message functions
ComplexMessage create_message(const char* text) {
    ComplexMessage msg;
    msg.message = text;
    msg.data = nullptr;
    msg.size = text ? strlen(text) : 0;
    msg.flags = 0;
    return msg;
}

const char* get_message_text(ComplexMessage* msg) {
    return msg ? msg->message : nullptr;
}

// Example transform function for testing
static double example_transform(double x) {
    return x * x;  // Square function
}

// Function pointer operations
double apply_transform(Transform t, double x) {
    if (t.transform) {
        return t.transform(x + t.parameter);
    }
    return x;
}

Transform create_transform(MathFunc func, double param) {
    Transform t;
    t.transform = func ? func : example_transform;
    t.parameter = param;
    return t;
}

