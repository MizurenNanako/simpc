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
} // namespace simpc


#endif // __STDAFX
