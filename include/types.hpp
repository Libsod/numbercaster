#include <cstdint>
#include <optional>
#include <string>
#include <termios.h>

using i8 = int8_t;
using i32 = int32_t;
using i64 = int64_t;
using u8 = uint8_t;
using u32 = uint32_t;
using u64 = uint64_t;
using f32 = float;
using f64 = double;
using str = std::string;

using Termios = struct termios;

template <typename T> using Optional = std::optional<T>;
