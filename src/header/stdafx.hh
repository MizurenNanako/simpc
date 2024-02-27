#ifndef __STDAFX
#define __STDAFX

/**
 * @authors MizurenNanako
 * @brief This header is for header pre-compilation.
 */

#include <algorithm>
#include <concepts>
#include <cstdint>
#include <functional>
#include <iostream>
#include <list>
#include <map>
#include <numeric>
#include <ranges>
#include <set>
#include <sstream>
#include <string>
#include <string_view>
#include <type_traits>
#include <vector>

namespace simpc
{
    namespace R = std::ranges;
    using namespace std::literals;

    namespace BASIC_INFO
    {
        constexpr auto NAME    = "SimpC"s;
        constexpr auto VERSION = "0.0.1"s;
    } // namespace BASIC_INFO
} // namespace simpc


#endif // __STDAFX
