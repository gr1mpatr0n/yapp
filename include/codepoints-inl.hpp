#ifndef YAPP_CODEPOINTS_HPP
#define YAPP_CODEPOINTS_HPP

#include <cstdint>

inline bool is_alpha_lower(const uint8_t c) { return c >= 'a' && c <= 'z'; }
inline bool is_alpha(const uint8_t c) { return is_alpha_lower(c | 0x20); }
inline bool is_num(const uint8_t c) { return c >= '0' && c <= '9'; }
inline bool is_c0_control(const uint8_t c) { return c < ' '; }
inline bool is_c0_control_or_space(const uint8_t c) { return c <= ' '; }
inline bool is_c0_control_percent_encode_set(const uint8_t c) {
    return is_c0_control(c) || c > 0x7E;
}
inline bool is_fragment_percent_encode_set(const uint8_t c) {
    return is_c0_control_percent_encode_set(c) || (c == ' ')
        || (c == '"') || (c == '<') || (c == '>') || (c == '`');
}
inline bool is_query_percent_encode_set(const uint8_t c) {
    return is_c0_control_percent_encode_set(c) || (c == ' ')
        || (c == '"') || (c == '#') || (c == '<') || (c == '>');
}
inline bool is_tab_or_newline(const uint8_t c) {
    return c == '\t' || c == '\r' || c == '\n';
}
inline bool is_scheme_codepoint(const uint8_t c) {
    return is_alpha(c) || is_num(c) || c == '+' || c == '-' || c == '.';
}

#endif /* YAPP_CODEPOINTS_HPP */