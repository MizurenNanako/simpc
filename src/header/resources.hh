#ifndef __RESOURCES
#define __RESOURCES

// prepcompiled header
#include "stdafx.hh"

namespace simpc
{
    namespace resources
    {
        class FileNotFoundError : std::runtime_error {
          public:
            FileNotFoundError(const std::filesystem::path &path)
                : runtime_error{std::format(
                    "file: {}", path.generic_string())} {}
            ~FileNotFoundError() = default;
        };

        class filemanager {
          public:
            using path_t = std::filesystem::path;

          private:
            std::set<path_t> _system_header_path;

          private:
            filemanager() = default;

          public:
            inline static auto instance() -> const filemanager &
            {
                // Yes, cpp can do this, no java bullshit.
                static filemanager _singleton;
                return _singleton;
            }
            ~filemanager() = default;

            /// @brief add path for include searching
            /// @param path the path
            /// @exception FileNotFoundError
            inline auto add_header_path(const path_t &path) -> void
            {
                if (std::filesystem::is_directory(path))
                    _system_header_path.emplace(path);
                throw(FileNotFoundError{path});
            }

            inline auto open_header(const std::string_view name,
                                    bool                   is_system = true) const
                -> std::ifstream
            {
                for (auto &&header : _system_header_path)
                {
                    if (auto &&full = header / name;
                        std::filesystem::exists(full))
                    {
                        auto &&tmp = std::ifstream{full};
                        if (not tmp) throw(FileNotFoundError{full});
                        return tmp; // move semantic
                    }
                }
                if (not is_system)
                {
                    auto &&tmp = std::ifstream{std::string{name}};
                    if (not tmp) throw(FileNotFoundError{name});
                    return tmp; // move semantic
                }
                throw(FileNotFoundError{name});
            }
        };
    } // namespace resources

} // namespace simpc


#endif // __RESOURCES
