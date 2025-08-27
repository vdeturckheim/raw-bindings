#ifndef SAMPLE_H
#define SAMPLE_H

#include <stddef.h>
#include <stdint.h>

/**
 * Status codes for operations
 */
typedef enum {
    STATUS_SUCCESS = 0,
    STATUS_ERROR = -1,
    STATUS_TIMEOUT = -2,
    STATUS_NOT_FOUND = -3
} Status;

/**
 * Point structure representing 2D coordinates
 */
typedef struct {
    float x;
    float y;
} Point;

/**
 * Rectangle structure
 */
typedef struct Rectangle {
    Point top_left;
    Point bottom_right;
    float area;
} Rectangle;

/**
 * Opaque handle type
 */
typedef void* Handle;

/**
 * Calculate the distance between two points
 * @param p1 First point
 * @param p2 Second point
 * @return Distance between the points
 */
float calculate_distance(Point p1, Point p2);

/**
 * Create a rectangle from two points
 * @param top_left Top-left corner
 * @param bottom_right Bottom-right corner
 * @return A new rectangle
 */
Rectangle create_rectangle(Point top_left, Point bottom_right);

/**
 * Initialize the system
 * @param config Configuration string
 * @return Status code
 */
Status initialize_system(const char* config);

/**
 * Process data buffer
 * @param buffer Input buffer
 * @param size Size of the buffer
 * @param output Output buffer
 * @param output_size Size of output buffer
 * @return Number of bytes processed, or negative error code
 */
int process_data(const uint8_t* buffer, size_t size, uint8_t* output, size_t output_size);

/**
 * Cleanup and release resources
 */
void cleanup(void);

#endif // SAMPLE_H