#include "stdafx.hh"

#include <format>

auto main() -> int
{
    using std::cout, std::endl;
    cout << std::format("Name:{} Version: {}",
                        simpc::BASIC_INFO::NAME,
                        simpc::BASIC_INFO::VERSION)
         << endl;
    return 0;
}
