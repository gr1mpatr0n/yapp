#ifndef YAPP_HPP
#define YAPP_HPP

#include <string>
#include <sstream>
#include <iomanip>
#include <map>
#include <cctype>
#include <stdexcept>

namespace yapp {

class URL {
public:
    URL(const std::string& url_string) : port_(-1) {
        size_t pos = 0;

        size_t scheme_end = url_string.find("://");
        if (scheme_end != std::string::npos) {
            scheme_ = url_string.substr(0, scheme_end);
            pos = scheme_end + 3;
        }

        size_t path_start = url_string.find('/', pos);
        size_t query_start = url_string.find('?', pos);
        size_t host_end = std::min(path_start, query_start);
        if (host_end == std::string::npos) host_end = url_string.length();

        std::string host_port = url_string.substr(pos, host_end - pos);
        size_t port_pos = host_port.find(':');
        if (port_pos != std::string::npos) {
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
            size_t query_end = (frag_start != std::string::npos) ? frag_start : url_string.length();
            std::string query_str = url_string.substr(pos, query_end - pos);

            size_t param_pos = 0;
            while (param_pos < query_str.length()) {
                size_t amp_pos = query_str.find('&', param_pos);
                size_t param_end = (amp_pos != std::string::npos) ? amp_pos : query_str.length();
                std::string param = query_str.substr(param_pos, param_end - param_pos);

                size_t eq_pos = param.find('=');
                if (eq_pos != std::string::npos) {
                    query_[param.substr(0, eq_pos)] = param.substr(eq_pos + 1);
                }

                param_pos = (amp_pos != std::string::npos) ? amp_pos + 1 : query_str.length();
            }

            pos = query_end;
        }

        if (pos < url_string.length() && url_string[pos] == '#') {
            fragment_ = url_string.substr(pos + 1);
        }
    }

    URL operator/(const std::string& segment) const {
        URL new_url = *this;

        if (!new_url.path_.empty() && new_url.path_.back() == '/') {
            new_url.path_.pop_back();
        }

        if (segment.empty() || segment[0] != '/') {
            new_url.path_ += "/" + encode(segment);
        } else {
            new_url.path_ += encode(segment);
        }

        return new_url;
    }

    URL operator%=(std::initializer_list<std::pair<std::string, std::string>> params) const {
        URL new_url = *this;
        for (const auto &param : params) {
            new_url.query_[param.first] = param.second;
        }
        return new_url;
    }

    std::string str() const {
        std::ostringstream url;

        if (!scheme_.empty()) {
            url << scheme_ << "://";
        }

        url << host_;

        if (port_ != -1) {
            url << ":" << port_;
        }

        url << path_;

        std::string query_str = build_query();
        if (!query_str.empty()) {
            url << "?" << query_str;
        }

        if (!fragment_.empty()) {
            url << "#" << fragment_;
        }

        return url.str();
    }

    std::string scheme() const { return scheme_; }
    std::string host() const { return host_; }
    int port() const { return port_; }
    std::string path() const { return path_; }
    std::map<std::string, std::string> query() const { return query_; }
    std::string fragment() const { return fragment_; }

    URL &with_scheme(const std::string& scheme) {
        scheme_ = scheme;
        return *this;
    }

    URL &with_host(const std::string& host) {
        host_ = host;
        return *this;
    }

    URL &with_port(int port) {
        port_ = port;
        return *this;
    }

    URL &with_fragment(const std::string& fragment) {
        fragment_ = fragment;
        return *this;
    }

private:
    std::string scheme_;
    std::string host_;
    int port_;
    std::string path_;
    std::map<std::string, std::string> query_;
    std::string fragment_;

    static std::string encode(const std::string &value) {
        std::ostringstream escaped;
        escaped.fill('0');
        escaped << std::hex;

        for (char c : value) {
            if (std::isalnum(static_cast<unsigned char>(c)) ||
                c == '-' || c == '_' | c == '.' | c == '~') {
                    escaped << c;
            } else {
                escaped << '%' << std::setw(2)
                    << int(static_cast<unsigned char>(c));    
            }
        }

        return escaped.str();
    }

    std::string build_query() const {
        if (query_.empty()) return "";

        std::ostringstream qs;
        bool first = true;
        for (const auto &pair : query_) {
            if (!first) qs << "&";
            qs << encode(pair.first) << "=" << encode(pair.second);
            first = false;
        }
        return qs.str();
    }

};

} // namespace yapp

#endif /* YAPP_HPP */