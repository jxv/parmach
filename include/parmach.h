#ifndef PARMACH_H
#define PARMACH_H

#include <unistd.h>
#include <stdint.h>
#include <stdbool.h>
#include <str.h>

struct pm_parser;

struct pm_state {
	size_t pos;
	size_t line;
};

typedef struct pm_state pm_state_t;

struct pm_error {
	pm_state_t state;
};

typedef struct pm_error pm_error_t;

struct pm_parsers {
	struct pm_parser *data;
	size_t len;
};

typedef struct pm_parsers pm_parsers_t;

union pm_prim {
	bool b;
	char c;
	int8_t i8;
	uint8_t u8;
	int16_t i16;
	uint16_t u16;
	int32_t i32;
	uint32_t u32;
	float f;
};

typedef union pm_prim pm_prim_t;

union pm_data {
	pm_prim_t prim;
	void *ptr;
};

typedef union pm_data pm_data_t;

struct pm_result {
	pm_data_t data;
	pm_error_t error;
};

typedef struct pm_result pm_result_t;

typedef bool (*pm_parser_fn) (const pm_data_t, const str_t *, pm_state_t *, pm_result_t *);

struct pm_parser {
	const pm_data_t data;
	pm_parser_fn fn;
};

typedef struct pm_parser pm_parser_t;

bool pm_out_of_range(const str_t *, pm_state_t *, pm_result_t *);

char pm_step_state(const str_t *, pm_state_t *);

bool pm_parse_step(pm_parser_t p[1], const str_t *src, pm_state_t *state, pm_result_t *res);

bool pm_try_fn(pm_data_t, const str_t *, pm_state_t *, pm_result_t *);
bool pm_one_of_fn(const pm_data_t, const str_t *, pm_state_t *, pm_result_t *);
bool pm_none_of_fn(const pm_data_t, const str_t *, pm_state_t *, pm_result_t *);
bool pm_char_fn(const pm_data_t, const str_t *, pm_state_t *, pm_result_t *);
bool pm_satisfy_fn(const pm_data_t, const str_t *, pm_state_t *, pm_result_t *);
bool pm_string_fn(const pm_data_t, const str_t *, pm_state_t *, pm_result_t *);
bool pm_or_fn(const pm_data_t, const str_t *, pm_state_t *, pm_result_t *);
bool pm_and_fn(const pm_data_t, const str_t *, pm_state_t *, pm_result_t *);
bool pm_until_fn(const pm_data_t, const str_t *, pm_state_t *, pm_result_t *);
bool pm_choice_fn(const pm_data_t, const str_t *, pm_state_t *, pm_result_t *);
bool pm_choice_try_fn(const pm_data_t, const str_t *, pm_state_t *, pm_result_t *);

extern pm_parser_t pm_fail;
extern pm_parser_t pm_space;
extern pm_parser_t pm_newline;
extern pm_parser_t pm_tab;
extern pm_parser_t pm_upper;
extern pm_parser_t pm_lower;
extern pm_parser_t pm_alpha_num;
extern pm_parser_t pm_letter;
extern pm_parser_t pm_digit;
extern pm_parser_t pm_hex_digit;
extern pm_parser_t pm_oct_digit;
extern pm_parser_t pm_any_char;
extern pm_parser_t pm_crlf;
extern pm_parser_t pm_eof;
extern pm_parser_t pm_trail;
extern pm_parser_t pm_until_space;

bool pm_parse(pm_parser_t p[1], const str_t *src, pm_result_t *res);

#define PM_PAR(PTR, FN) { .data.ptr = PTR, .fn = FN }
#define PM_FN(FN) PM_PAR(NULL, FN)
#define PM_TRY(PAR) PM_PAR(PAR, pm_try_fn)
#define PM_ONE_OF(STR) PM_PAR(STR, pm_one_of_fn)
#define PM_NONE_OF(STR) PM_PAR(STR, pm_none_of_fn)
#define PM_CHAR(CHAR) { .data.prim.c = CHAR, .fn = pm_char_fn }
#define PM_STAISFY(FN) PM_PAR(FN, pm_satisfy_fn) // FN = bool (*)(char)
#define PM_STRING(STR) PM_PAR(STR, pm_string_fn)
#define PM_OR(PAR2) PM_PAR(PAR2, pm_or_fn)
#define PM_AND(PAR2) PM_PAR(PAR2, pm_and_fn)
#define PM_UNTIL(PAR) PM_PAR(PAR, pm_until_fn)
#define PM_CHOICE(PAR) PM_PAR(PAR, pm_choice_fn)
#define PM_CHOICE_TRY(PAR) PM_PAR(PAR, pm_choice_try_fn)

#endif
