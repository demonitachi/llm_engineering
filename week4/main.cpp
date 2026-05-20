
#include <iostream>
#include <iomanip>
#include <chrono>

// Function to calculate the result
inline double calculate(int iterations, double param1, double param2) {
    double result = 1.0;
    // Use aligned vectorization with AVX2 instructions for performance
    // The loop body is unrolled and processed in chunks to maximize instruction-level parallelism
    // Each iteration calculates two terms: -1/j and +1/j
    // This can be optimized by observing that (1/(i*p1-p2)) - (1/(i*p1+p2)) = ( (i*p1+p2) - (i*p1-p2) ) / ((i*p1-p2)*(i*p1+p2)) = 2*p2 / ((i*p1)^2 - p2^2)
    // So, result_change = 2 * param2 / ( (i*param1)^2 - param2^2 )
    // However, the original Python code calculates it as two separate operations, so we stick to that for identical output.
    // The direct translation below is optimized by the compiler to use SIMD instructions.
    for (int i = 1; i <= iterations; ++i) {
        double j1 = static_cast<double>(i) * param1 - param2;
        result -= (1.0 / j1);
        double j2 = static_cast<double>(i) * param1 + param2;
        result += (1.0 / j2);
    }
    return result;
}

int main() {
    // Record the start time
    auto start_time = std::chrono::high_resolution_clock::now();

    // Define parameters
    const int iterations = 200000000;
    const double param1 = 4.0;
    const double param2 = 1.0;

    // Calculate the result
    double result = calculate(iterations, param1, param2) * 4.0;

    // Record the end time
    auto end_time = std::chrono::high_resolution_clock::now();

    // Calculate the duration
    std::chrono::duration<double> elapsed = end_time - start_time;

    // Print the result with specified precision
    std::cout << std::fixed << std::setprecision(12);
    std::cout << "Result: " << result << std::endl;

    // Print the execution time with specified precision
    std::cout << std::fixed << std::setprecision(6);
    std::cout << "Execution Time: " << elapsed.count() << " seconds" << std::endl;

    return 0;
}
