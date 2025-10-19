#ifndef YAPP_CPP_YAPP_H
#define YAPP_CPP_YAPP_H

#ifdef _WIN32
    #ifdef YAPP_LIBEXPORT
        #define YAPP_API __declspec(dllexport)
    #else
        #define YAPP_API __declspec(dllimport)
    #endif
#else
    #define YAPP_API __attribute__((visibility("default")))
#endif

#include <algorithm>
#include <array>
#include <map>
#include <string>

namespace yapp {
    class YAPP_API URL {
    public:
        explicit URL(const std::string& url_string);

        auto parse_input(std::string_view input) -> void;

    private:
        std::string scheme_;
        std::string host_;
        int port_;
        std::string path_;
        std::map<std::string, std::string> query_;
        std::string fragment_;
    };
}

inline auto is_scheme_special(const std::string_view scheme) -> bool {
    constexpr std::array special = {
        "ftp", "file", "http", "https", "ws", "wss"
    };
    return std::find(special.begin(), special.end(), scheme) != special.end();
}

#endif //YAPP_CPP_YAPP_H