#pragma once

#include <cstdint>  // Include int32_t, int64_t
#include <optional> // Include std::optional
#include <string>   // Include std::string

/* Type aliases for commonly used types to improve readability (I like Rust btw)
 */
using i32 = int32_t;     // Alias for 32-bit signed integer
using i64 = int64_t;     // Alias for 64-bit signed integer
using f64 = double;      // Alias for double
using str = std::string; // Alias for std::string

/* Template alias for std::optional, providing a shorthand way to declare
 * optional types: Optional<T> */
template <typename T> using Option = std::optional<T>;
