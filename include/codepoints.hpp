#ifndef YAPP_CODEPOINTS_HPP
#define YAPP_CODEPOINTS_HPP

#include <array>
#include <cstdint>

static inline bool is_c0_control(uint8_t c)          { return c <  ' '; }
static inline bool is_c0_control_or_space(uint8_t c) { return c <= ' '; }

#endif /* YAPP_CODEPOINTS_HPP */