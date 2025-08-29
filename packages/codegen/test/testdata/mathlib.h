#ifndef MATHLIB_H
#define MATHLIB_H

#include <stddef.h>  // for size_t

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

// Complex types for testing code generation

// Nested struct
typedef struct {
    Point2D center;
    double radius;
} Circle;

// Union type
typedef union {
    int i;
    double d;
    char bytes[8];
} NumberUnion;

// Struct with arrays
typedef struct {
    char name[32];
    double values[10];
    int count;
} DataArray;

// Opaque pointer type (forward declaration)
typedef struct OpaqueContext* ContextHandle;

// Typedef to basic type
typedef double Scalar;
typedef int Integer;

// Struct with various field types
typedef struct {
    const char* message;
    void* data;
    size_t size;
    int flags;
} ComplexMessage;

// Function pointer typedef
typedef double (*MathFunc)(double x);

// Struct containing function pointer
typedef struct {
    MathFunc transform;
    double parameter;
} Transform;

// Functions using complex types
Circle create_circle(Point2D center, double radius);
double circle_area(Circle c);
int circle_contains_point(Circle c, Point2D p);

NumberUnion create_int_union(int value);
NumberUnion create_double_union(double value);
int get_int_from_union(NumberUnion u);
double get_double_from_union(NumberUnion u);

DataArray create_data_array(const char* name);
void add_value_to_array(DataArray* arr, double value);
double sum_array_values(DataArray* arr);

// Opaque handle functions
ContextHandle create_context(void);
void destroy_context(ContextHandle ctx);
int context_do_something(ContextHandle ctx, int value);

// Function using typedef'd types
Scalar scalar_multiply(Scalar a, Scalar b);
Integer integer_add(Integer a, Integer b);

// Complex message functions
ComplexMessage create_message(const char* text);
const char* get_message_text(ComplexMessage* msg);

// Function pointer operations
double apply_transform(Transform t, double x);
Transform create_transform(MathFunc func, double param);

#ifdef __cplusplus
}
#endif

#endif // MATHLIB_H
