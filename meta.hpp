#include <type_traits>

template <typename T, bool is_trivially_copyable>
struct copy_fast;

template <typename T>
struct copy_fast<T, false>
{
    using type = const T &;
};

template <typename T>
struct copy_fast<T, true>
{
    using type = T;
};

template <typename T>
using copy_fast_t = copy_fast<T, std::is_trivially_copyable_v<T>>::value;
