#ifndef YAPP_HPP
#define YAPP_HPP

#include "yapp.hpp"

#include "codepoints-inl.hpp"

#include <string>
#include <sstream>
#include <iomanip>
#include <map>
#include <cctype>
#include <cstddef>
#include <algorithm>
#include <iostream>
#include <iterator>
#include <stdexcept>

namespace yapp {

struct QueryParam {
    std::string_view key;
    std::string_view value;
};

enum URLParseState {
    SchemeStart,
    Scheme,
    NoScheme,
    SpecialRelativeOrAuthority,
    PathOrAuthority,
    Relative,
    RelativeSlash,
    SpecialAuthoritySlashes,
    SpecialAuthorityIgnoreSlashes,
    Authority,
    Host,
    Hostname,
    Port,
    File,
    FileSlash,
    FileHost,
    PathStart,
    Path,
    OpaquePath,
    Query,
    Fragment,
};

URL::URL(const std::string& url_string) : port_(-1) {
        size_t pos = 0;

        if (size_t scheme_end = url_string.find("://"); scheme_end != std::string::npos) {
            scheme_ = url_string.substr(0, scheme_end);
            pos = scheme_end + 3;
        }

        size_t path_start = url_string.find('/', pos);
        size_t query_start = url_string.find('?', pos);
        size_t host_end = std::min(path_start, query_start);
        if (host_end == std::string::npos) host_end = url_string.length();

        std::string host_port = url_string.substr(pos, host_end - pos);
        if (size_t port_pos = host_port.find(':'); port_pos != std::string::npos) {
            host_ = host_port.substr(0, port_pos);
            port_ = std::stoi(host_port.substr(port_pos + 1));
        } else {
            host_ = host_port;
        }

        pos = host_end;

        if (path_start != std::string::npos && path_start == pos) {
            size_t path_end = url_string.find('?', pos);
            if (path_end == std::string::npos) path_end = url_string.length();
            path_ = url_string.substr(pos, path_end - pos);
            pos = path_end;
        } else {
            path_ = "/";
        }

        if (query_start != std::string::npos && query_start == pos) {
            pos++;
            size_t frag_start = url_string.find('#', pos);
            size_t query_end = frag_start != std::string::npos ? frag_start : url_string.length();
            std::string query_str = url_string.substr(pos, query_end - pos);

            size_t param_pos = 0;
            while (param_pos < query_str.length()) {
                size_t amp_pos = query_str.find('&', param_pos);
                size_t param_end = (amp_pos != std::string::npos) ? amp_pos : query_str.length();
                std::string param = query_str.substr(param_pos, param_end - param_pos);

                if (size_t eq_pos = param.find('='); eq_pos != std::string::npos) {
                    query_[param.substr(0, eq_pos)] = param.substr(eq_pos + 1);
                }

                param_pos = amp_pos != std::string::npos ? amp_pos + 1 : query_str.length();
            }

            pos = query_end;
        }

        if (pos < url_string.length() && url_string[pos] == '#') {
            fragment_ = url_string.substr(pos + 1);
        }
    }

    // URL operator/(const std::string& segment) const {
    //     URL new_url = *this;
    //
    //     if (!new_url.path_.empty() && new_url.path_.back() == '/') {
    //         new_url.path_.pop_back();
    //     }
    //
    //     if (segment.empty() || segment[0] != '/') {
    //         new_url.path_ += "/" + encode(segment);
    //     } else {
    //         new_url.path_ += encode(segment);
    //     }
    //
    //     return new_url;
    // }
    //
    // URL operator%(const QueryParam &qp) const {
    //     URL new_url = *this;
    //     new_url.query_[std::string(qp.key)] = qp.value;
    //     return new_url;
    // }
    //
    // std::string str() const {
    //     std::ostringstream url;
    //
    //     if (!scheme_.empty()) {
    //         url << scheme_ << "://";
    //     }
    //
    //     url << host_;
    //
    //     if (port_ != -1) {
    //         url << ":" << port_;
    //     }
    //
    //     url << path_;
    //
    //     std::string query_str = build_query();
    //     if (!query_str.empty()) {
    //         url << "?" << query_str;
    //     }
    //
    //     if (!fragment_.empty()) {
    //         url << "#" << fragment_;
    //     }
    //
    //     return url.str();
    // }

//     URL &with_scheme(const std::string& scheme) {
//         scheme_ = scheme;
//         return *this;
//     }
//
//     URL &with_host(const std::string& host) {
//         host_ = host;
//         return *this;
//     }
//
//     URL &with_port(int port) {
//         port_ = port;
//         return *this;
//     }
//
//     URL &with_fragment(const std::string& fragment) {
//         fragment_ = fragment;
//         return *this;
//     }
//
// private:
//
//     static std::string encode(const std::string &value) {
//         std::ostringstream escaped;
//         escaped.fill('0');
//         escaped << std::hex;
//
//         for (char c : value) {
//             if (std::isalnum(static_cast<unsigned char>(c)) ||
//                 c == '-' || c == '_' | c == '.' | c == '~') {
//                     escaped << c;
//             } else {
//                 escaped << '%' << std::setw(2)
//                     << int(static_cast<unsigned char>(c));
//             }
//         }
//
//         return escaped.str();
//     }
//
//     std::string build_query() const {
//         if (query_.empty()) return "";
//
//         std::ostringstream qs;
//         bool first = true;
//         for (const auto &pair : query_) {
//             if (!first) qs << "&";
//             qs << encode(pair.first) << "=" << encode(pair.second);
//             first = false;
//         }
//         return qs.str();
//     }

auto URL::parse_input(std::string_view input) -> void {
    std::string work;
    work.reserve(input.size());

    auto i = work.begin();

    bool at_sign_seen = false, inside_brackets = false, password_token_seen = false;
    auto state = URLParseState::SchemeStart;

	const auto first_legal = std::find_if_not(input.begin(), input.end(), is_c0_control_or_space);
    const auto last_legal = std::find_if_not(input.rbegin(), input.rend(), is_c0_control_or_space).base();
    std::copy(first_legal, last_legal, std::back_inserter(work));

    work.erase(std::remove_if(work.begin(), work.end(),
        [](const char c) { return c == '\t' || c == '\r' || c == '\n'; }),
        work.end()
    );

    while (i != work.end()) {
        switch (state) {
        case SchemeStart:
            if (is_alpha(*i)) {
                work.push_back(*i | 0x20);
                state = Scheme;
                break;
            }
            state = NoScheme;
            --i;
            break;
        case Scheme:
            if (is_scheme_codepoint(*i)) {
                work.push_back(*i | 0x20);
            } else if (*i == ':') {
                scheme_ = {work.begin(), i};
                if (scheme_ == "file") {
                    state = File;
                    break;
                }
                if (is_scheme_special(scheme_)) {
                    state = SpecialAuthoritySlashes;
                    break;
                }
                if (*(i+1) == '/') {
                    state = PathOrAuthority;
                    ++i;
                    break;
                }
                path_ = {};
                state = OpaquePath;
                break;
            } else {
                state = NoScheme;
                i = work.begin();
                continue;
            }
        }
        ++i;
    }

    std::cout << work << '\n';
}

} // namespace yapp

#endif /* YAPP_HPP */