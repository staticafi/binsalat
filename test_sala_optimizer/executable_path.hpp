#ifndef TESTS_SALA_OPTIMIZER_EXECUTABLE_PATH_HPP_INCLUDED
#define TESTS_SALA_OPTIMIZER_EXECUTABLE_PATH_HPP_INCLUDED
#include <filesystem>
#include <stdexcept>

#if defined(_WIN32)
#include <windows.h>
#elif defined(__APPLE__)
#include <climits>
#include <mach-o/dyld.h>
#endif

namespace optimizer::tests
{

inline std::filesystem::path executable_path()
{
#if defined(_WIN32)
    wchar_t     buf[MAX_PATH];
    const DWORD len = GetModuleFileNameW(nullptr, buf, MAX_PATH);
    if (len == 0 || len == MAX_PATH)
        throw std::runtime_error{"GetModuleFileNameW failed"};
    return std::filesystem::path{std::wstring{buf, len}};
#elif defined(__APPLE__)
    char     buf[PATH_MAX];
    uint32_t size = sizeof(buf);
    if (_NSGetExecutablePath(buf, &size) != 0)
        throw std::runtime_error{"executable path buffer too small"};
    return std::filesystem::canonical(std::filesystem::path{buf});
#else
    return std::filesystem::canonical("/proc/self/exe");
#endif
}

inline std::filesystem::path executable_dir()
{
    return executable_path().parent_path();
}

} // namespace optimizer::tests
#endif
