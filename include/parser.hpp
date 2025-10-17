#include <string>
#include <optional>
#include <unordered_map>

constexpr char ascii_lower(char c) noexcept { return c | 0x20; }

constexpr bool ascii_is_alpha(char c) noexcept {
    char cl = ascii_lower(c);
    return (cl >= 'a') && (cl <= 'z');
}

constexpr bool starts_with_windows_drive_letter(std::string_view s) noexcept {
    if (s.size() < 2) return false;
    if (!ascii_is_alpha(s[0]) || ((s[1] != ':') && (s[1] != '|'))) return false;
    return (s.size() == 2
        || s[2] == '/' || s[2] == '\\' || s[2] == '?' || s[2] == '#');
}

std::optional<short> get_default_port_for_scheme(std::string_view scheme) noexcept {
    /* Simple linear scan as small # of possibilities. */
    if (scheme == "ftp")   return 21;
    if (scheme == "http")  return 80;
    if (scheme == "https") return 443;
    if (scheme == "ws")    return 80;
    if (scheme == "wss")   return 443;
    if (scheme == "file")  return -1;
    return std::nullopt;
}

void remove_tab_newline(std::string &url) noexcept {
    url.erase(std::remove_if(url.begin(), url.end(),
        [](char c) { return c == '\t' || c == '\r' || c == '\n'; }),
        url.end());
}