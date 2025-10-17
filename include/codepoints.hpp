#ifndef YAPP_CODEPOINTS_HPP
#define YAPP_CODEPOINTS_HPP

#include <array>
#include <cstdint>

static inline bool is_c0_control(uint8_t c) { return c < ' '; }
static inline bool is_c0_control_or_space(uint8_t c) { return c <= ' '; }
static inline bool is_c0_control_percent_encode_set(uint8_t c) { 
    return is_c0_control(c) || c > 0x7E;
}
static inline bool is_fragment_percent_encode_set(uint8_t c) {
    return is_c0_control_percent_encode_set(c) || (c == ' ')
        || (c == '"') || (c == '<') || (c == '>') || (c == '`');
}
static inline bool is_query_percent_encode_set(uint8_t c) {
    return is_c0_control_percent_encode_set(c) || (c == ' ')
        || (c == '"') || (c == '#') || (c == '<') || (c == '>');
}

#endif /* YAPP_CODEPOINTS_HPP */