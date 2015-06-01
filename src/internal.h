#ifndef PM_INTERNAL_H
#define PM_INTERNAL_H

struct pm_value pm_prim_b(bool b);
struct pm_value pm_prim_c(char c);
struct pm_value pm_prim_i8(int8_t i8);
struct pm_value pm_prim_u8(uint8_t u8);
struct pm_value pm_prim_wc(wchar_t wc);
struct pm_value pm_prim_i16(int16_t i16);
struct pm_value pm_prim_u16(uint16_t u16);
struct pm_value pm_prim_i32(int32_t i32);
struct pm_value pm_prim_u32(uint32_t u32);
struct pm_value pm_prim_f(float f);

bool pm_out_of_range(const char *src, long len, struct pm_state *state, union pm_result *res);

char pm_step_state(const char *src, struct pm_state *state);

bool pm_one_of_fn(const union pm_data d, const char *src, long len, struct pm_state *state, union pm_result *res);
void pm_one_of(struct pm_str *str, struct pm_parser *q);
bool pm_none_of_fn(const union pm_data d, const char *src, long len, struct pm_state *state, union pm_result *res);
void pm_none_of(struct pm_str *str, struct pm_parser *q);
bool pm_char_fn(const union pm_data d, const char *src, long len, struct pm_state *state, union pm_result *res);
void pm_char(char c, struct pm_parser *q);
bool pm_satisfy_fn(const union pm_data d, const char *src, long len, struct pm_state *state, union pm_result *res);
void pm_satisfy(bool (*fn)(char), struct pm_parser *q);
bool pm_string_fn(const union pm_data d, const char *src, long len, struct pm_state *state, union pm_result *res);
void pm_string(struct pm_str *str, struct pm_parser *q);
bool pm_or_fn(const union pm_data d, const char *src, long len, struct pm_state *state, union pm_result *res);
void pm_or(struct pm_parser p[2], struct pm_parser *q);
bool pm_and_fn(const union pm_data d, const char *src, long len, struct pm_state *state, union pm_result *res);
void pm_and(struct pm_parser p[2], struct pm_parser *q);
bool pm_try_fn(union pm_data d, const char *src, long len, struct pm_state *state, union pm_result *res);
void pm_try(struct pm_parser *p, struct pm_parser *q);
bool pm_parse(struct pm_parser *p, const char *src, long len, union pm_result *res);
bool pm_space_fn(union pm_data d, const char *src, long len, struct pm_state *state, union pm_result *res);
bool pm_upper_fn(union pm_data d, const char *src, long len, struct pm_state *state, union pm_result *res);
bool pm_lower_fn(union pm_data d, const char *src, long len, struct pm_state *state, union pm_result *res);
bool pm_alpha_num_fn(union pm_data d, const char *src, long len, struct pm_state *state, union pm_result *res);
bool pm_letter_fn(union pm_data d, const char *src, long len, struct pm_state *state, union pm_result *res);
bool pm_digit_fn(union pm_data d, const char *src, long len, struct pm_state *state, union pm_result *res);
bool pm_hex_digit_fn(union pm_data d, const char *src, long len, struct pm_state *state, union pm_result *res);
bool pm_oct_digit_fn(union pm_data d, const char *src, long len, struct pm_state *state, union pm_result *res);
bool pm_any_char_fn(union pm_data d, const char *src, long len, struct pm_state *state, union pm_result *res);

#endif
