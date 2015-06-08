#ifndef PARMACH_H
#define PARMACH_H

#include <stdint.h>
#include <stdbool.h>
#include <wchar.h>

struct pm_parser;

struct pm_state {
	long pos;
	long line;
};

struct pm_error {
	struct pm_state state;
};

struct pm_str {
	const char *data;
	long len;
};

struct pm_parsers {
	struct pm_parser *data;
	long len;
};

enum pm_prim_tag {
	PM_BOOL,
	PM_CHAR,
	PM_INT8,
	PM_UINT8,
	PM_WCHAR,
	PM_INT16,
	PM_UINT16,
	PM_INT32,
	PM_UINT32,
	PM_FLOAT,
};

union pm_prim {
	bool b;
	char c;
	int8_t i8;
	uint8_t u8;
	wchar_t wc;
	int16_t i16;
	uint16_t u16;
	int32_t i32;
	uint32_t u32;
	float f;
};

union pm_data {
	union pm_prim prim;
	struct pm_parser *parser;
	struct pm_str *str;
	void *ptr;
};

#define PM_PRIM 0

struct pm_value {
	int group;
	int tag;
	union pm_data data;
};

struct pm_result {
	struct pm_value value;
	struct pm_error error;
};

typedef bool (*pm_parser_fn) (const union pm_data, const char *, long, struct pm_state *, struct pm_result *);

struct pm_parser {
	union pm_data self;
	pm_parser_fn fn;
};

bool pm_out_of_range(const char *src, long len, struct pm_state *state, struct pm_result *res);

char pm_step_state(const char *src, struct pm_state *state);

void pm_one_of(struct pm_str *str, struct pm_parser *q);
void pm_none_of(struct pm_str *str, struct pm_parser *q);
void pm_char(char c, struct pm_parser *q);
void pm_satisfy(bool (*fn)(char), struct pm_parser *q);
void pm_string(struct pm_str *str, struct pm_parser *q);

void pm_or(struct pm_parser p[2], struct pm_parser *q);
void pm_and(struct pm_parser p[2], struct pm_parser *q);
void pm_try(struct pm_parser p[1], struct pm_parser *q);

void pm_choice(struct pm_parsers *p, struct pm_parser *q);
void pm_choice_try(struct pm_parsers *p, struct pm_parser *q);
void pm_until(struct pm_parser *p, struct pm_parser *q);

bool pm_parse_step(struct pm_parser p[1], const char *src, long len, struct pm_state *state, struct pm_result *res);
bool pm_parse(struct pm_parser p[1], const char *src, long len, struct pm_result *res);

extern struct pm_parser pm_fail;
extern struct pm_parser pm_space;
extern struct pm_parser pm_newline;
extern struct pm_parser pm_tab;
extern struct pm_parser pm_upper;
extern struct pm_parser pm_lower;
extern struct pm_parser pm_alpha_num;
extern struct pm_parser pm_letter;
extern struct pm_parser pm_digit;
extern struct pm_parser pm_hex_digit;
extern struct pm_parser pm_oct_digit;
extern struct pm_parser pm_any_char;
extern struct pm_parser pm_crlf;
extern struct pm_parser pm_eof;
extern struct pm_parser pm_trail;
extern struct pm_parser pm_until_space;

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

bool pm_one_of_fn(const union pm_data d, const char *src, long len, struct pm_state *state, struct pm_result *res);
void pm_one_of(struct pm_str *str, struct pm_parser *q);
bool pm_none_of_fn(const union pm_data d, const char *src, long len, struct pm_state *state, struct pm_result *res);
void pm_none_of(struct pm_str *str, struct pm_parser *q);
bool pm_char_fn(const union pm_data d, const char *src, long len, struct pm_state *state, struct pm_result *res);
void pm_char(char c, struct pm_parser *q);
bool pm_satisfy_fn(const union pm_data d, const char *src, long len, struct pm_state *state, struct pm_result *res);
void pm_satisfy(bool (*fn)(char), struct pm_parser *q);
bool pm_string_fn(const union pm_data d, const char *src, long len, struct pm_state *state, struct pm_result *res);
void pm_string(struct pm_str *str, struct pm_parser *q);
bool pm_or_fn(const union pm_data d, const char *src, long len, struct pm_state *state, struct pm_result *res);
void pm_or(struct pm_parser p[2], struct pm_parser *q);
bool pm_and_fn(const union pm_data d, const char *src, long len, struct pm_state *state, struct pm_result *res);
void pm_and(struct pm_parser p[2], struct pm_parser *q);
bool pm_try_fn(union pm_data d, const char *src, long len, struct pm_state *state, struct pm_result *res);
void pm_try(struct pm_parser *p, struct pm_parser *q);
bool pm_parse(struct pm_parser *p, const char *src, long len, struct pm_result *res);
bool pm_space_fn(union pm_data d, const char *src, long len, struct pm_state *state, struct pm_result *res);
bool pm_upper_fn(union pm_data d, const char *src, long len, struct pm_state *state, struct pm_result *res);
bool pm_lower_fn(union pm_data d, const char *src, long len, struct pm_state *state, struct pm_result *res);
bool pm_alpha_num_fn(union pm_data d, const char *src, long len, struct pm_state *state, struct pm_result *res);
bool pm_letter_fn(union pm_data d, const char *src, long len, struct pm_state *state, struct pm_result *res);
bool pm_digit_fn(union pm_data d, const char *src, long len, struct pm_state *state, struct pm_result *res);
bool pm_hex_digit_fn(union pm_data d, const char *src, long len, struct pm_state *state, struct pm_result *res);
bool pm_oct_digit_fn(union pm_data d, const char *src, long len, struct pm_state *state, struct pm_result *res);
bool pm_any_char_fn(union pm_data d, const char *src, long len, struct pm_state *state, struct pm_result *res);

#endif
