#ifndef __STDAFX
#define __STDAFX

/**
 * @authors MizurenNanako
 * @brief This header is for header pre-compilation.
 */

#include <algorithm>
#include <any>
#include <array>
#include <concepts>
#include <cstdint>
#include <deque>
#include <exception>
#include <filesystem>
#include <format>
#include <fstream>
#include <functional>
#include <iostream>
#include <list>
#include <map>
#include <memory>
#include <numeric>
#include <optional>
#include <ranges>
#include <set>
#include <sstream>
#include <stack>
#include <stdexcept>
#include <string>
#include <string_view>
#include <type_traits>
#include <variant>
#include <vector>

namespace simpc
{
    namespace R = std::ranges;
    using namespace std::literals;
    class common_iterator_sentinel {};

    namespace BASIC_INFO
    {
        constexpr auto NAME    = "SimpC"s;
        constexpr auto VERSION = "0.0.1"s;
    } // namespace BASIC_INFO

    template<typename T, typename... Args>
    inline static constexpr auto is_one_of(T a, Args... args)
    {
        return (... || (a == args));
    }

    template<typename T, typename... Args>
    inline static constexpr auto is_none_of(T a, Args... args)
    {
        return not is_one_of(a, args...);
    }
} // namespace simpc


#endif // __STDAFX
