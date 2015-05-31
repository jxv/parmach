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
	const char *name;
	const char *msg;
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

struct pm_str {
	const char *data;
	int len;
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

union pm_result {
	struct pm_value value;
	struct pm_error error;
};

typedef bool (*pm_parser_fn)(union pm_data, const char *, long, struct pm_state *, union pm_result *);

struct pm_parser {
	union pm_data self;
	pm_parser_fn fn;
};

void pm_one_of(struct pm_str *strm, struct pm_parser *q);
void pm_char(char c, struct pm_parser *q);
void pm_or(struct pm_parser p[2], struct pm_parser *q);
void pm_and(struct pm_parser p[2], struct pm_parser *q);
void pm_try(struct pm_parser p[1], struct pm_parser *q);

bool pm_parse(struct pm_parser p[1], const char *src, long len, union pm_result *res);

extern const struct pm_parser pm_digit;

#endif
