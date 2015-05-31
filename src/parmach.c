#include <stdio.h>
#include <string.h>
#include "parmach.h"

struct pm_value pm_prim_b(bool b)
{
	return (struct pm_value) {
		.group = PM_PRIM,
		.tag = PM_BOOL,
		.data.prim.b = b,
	};
}

struct pm_value pm_prim_c(char c)
{
	return (struct pm_value) {
		.group = PM_PRIM,
		.tag = PM_CHAR,
		.data.prim.c = c,
	};
}

struct pm_value pm_prim_i8(int8_t i8)
{
	return (struct pm_value) {
		.group = PM_PRIM,
		.tag = PM_INT8,
		.data.prim.i8 = i8,
	};
}

struct pm_value pm_prim_u8(uint8_t u8)
{
	return (struct pm_value) {
		.group = PM_PRIM,
		.tag = PM_UINT8,
		.data.prim.u8 = u8,
	};
}

struct pm_value pm_prim_wc(wchar_t wc)
{
	return (struct pm_value) {
		.group = PM_PRIM,
		.tag = PM_WCHAR,
		.data.prim.wc = wc,
	};
}

struct pm_value pm_prim_i16(int16_t i16)
{
	return (struct pm_value) {
		.group = PM_PRIM,
		.tag = PM_INT16,
		.data.prim.i16 = i16,
	};
}

struct pm_value pm_prim_u16(uint16_t u16)
{
	return (struct pm_value) {
		.group = PM_PRIM,
		.tag = PM_UINT16,
		.data.prim.u16 = u16,
	};
}

struct pm_value pm_prim_i32(int32_t i32)
{
	return (struct pm_value) {
		.group = PM_PRIM,
		.tag = PM_INT32,
		.data.prim.i32 = i32,
	};
}

struct pm_value pm_prim_u32(uint32_t u32)
{
	return (struct pm_value) {
		.group = PM_PRIM,
		.tag = PM_UINT32,
		.data.prim.u32 = u32,
	};
}

struct pm_value pm_prim_f(float f)
{
	return (struct pm_value) {
		.group = PM_PRIM,
		.tag = PM_FLOAT,
		.data.prim.f = f,
	};
}

bool pm_out_of_range(const char *src, long len,
	struct pm_state *state, union pm_result *res)
{
	if (state->pos >= 0 && state->pos < len) {
		return false;
	}
	res->error = (struct pm_error) {
		.state = *state,
		.name = NULL,
		.msg = NULL,
	};
	return true;
	
}

inline
char pm_step_state(const char *src, struct pm_state *state)
{
	const char c = src[state->pos];
	state->pos++;
	if (c == '\n') {
		state->line++;
	}
	return c;
}

bool pm_one_of_fn(const union pm_data d, const char *src, long len,
	struct pm_state *state, union pm_result *res)
{
	if (pm_out_of_range(src, len, state, res)) {
		return false;
	}
	const char c = pm_step_state(src, state);
	for (long i = 0; i < d.str->len; i++) {
		if (c == d.str->data[i]) {
			res->value = pm_prim_c(c);
			return true;
		}
	}
	return false;
}

void pm_one_of(struct pm_str *str, struct pm_parser *q)
{
	*q = (struct pm_parser) {
		.self.str = str,
		.fn = pm_one_of_fn,
	};
}

bool pm_none_of_fn(const union pm_data d, const char *src, long len,
	struct pm_state *state, union pm_result *res)
{
	if (pm_out_of_range(src, len, state, res)) {
		return false;
	}
	const char c = pm_step_state(src, state);
	for (long i = 0; i < d.str->len; i++) {
		if (c == d.str->data[i]) {
			return false;
		}
	}
	res->value = pm_prim_c(c);
	return true;
}

void pm_none_of(struct pm_str *str, struct pm_parser *q)
{
	*q = (struct pm_parser) {
		.self.str = str,
		.fn = pm_none_of_fn,
	};
}

bool pm_char_fn(const union pm_data d, const char *src, long len,
	struct pm_state *state, union pm_result *res)
{
	if (pm_out_of_range(src, len, state, res)) {
		return false;
	}
	const char c = pm_step_state(src, state);
	if (c == d.prim.c) {
		res->value = pm_prim_c(c);
		return true;
	}
	return false;
}

void pm_char(char c, struct pm_parser *q)
{
	*q = (struct pm_parser) {
		.self.prim.c = c,
		.fn = pm_char_fn,
	};
}

bool pm_satisfy_fn(const union pm_data d, const char *src, long len,
	struct pm_state *state, union pm_result *res)
{
	if (pm_out_of_range(src, len, state, res)) {
		return false;
	}
	const char c = pm_step_state(src, state);
	bool (**fn)(char) = d.ptr;
	if ((*fn)(c)) {
		res->value = pm_prim_c(c);
		return true;
	}
	return false;
}

void pm_satisfy(bool (*fn)(char), struct pm_parser *q)
{
	*q = (struct pm_parser) {
		.self.ptr = &fn,
		.fn = pm_satisfy_fn,
	};
}

bool pm_string_fn(const union pm_data d, const char *src, long len,
	struct pm_state *state, union pm_result *res)
{
	if (pm_out_of_range(src, len + d.str->len, state, res)) {
		return false;
	}
	for (long i = 0; i < d.str->len; i++) {
		const char c = pm_step_state(src, state);
		if (c != d.str->data[i]) {
			return false;
		}
	}
	{
		// TODO: group, tag
		res->value.data.str->data = src;
		res->value.data.str->len = d.str->len;
	}
	return true;
}

void pm_string(struct pm_str *str, struct pm_parser *q)
{
	*q = (struct pm_parser) {
		.self.str = str,
		.fn = pm_string_fn,
	};
}

bool pm_or_fn(const union pm_data d, const char *src, long len,
	struct pm_state *state, union pm_result *res)
{
	return
		d.parser[0].fn(d.parser[0].self, src, len, state, res) ||
		d.parser[1].fn(d.parser[1].self, src, len, state, res);
}

void pm_or(struct pm_parser p[2], struct pm_parser *q)
{
	*q = (struct pm_parser) {
		.self.parser = p,
		.fn = pm_or_fn,
	};
}

bool pm_and_fn(const union pm_data d, const char *src, long len,
	struct pm_state *state, union pm_result *res)
{
	return
		d.parser[0].fn(d.parser[0].self, src, len, state, &res[0]) &&
		d.parser[1].fn(d.parser[1].self, src, len, state, &res[1]);
}

void pm_and(struct pm_parser p[2], struct pm_parser *q)
{
	*q = (struct pm_parser) {
		.self.parser = p,
		.fn = pm_and_fn,
	};
}

bool pm_try_fn(union pm_data d, const char *src, long len,
	struct pm_state *state, union pm_result *res)
{
	struct pm_state backup = *state;
	if (d.parser->fn(d.parser->self, src, len, state, res)) {
		return true;
	} else {
		*state = backup;
		return false;
	}
}

void pm_try(struct pm_parser *p, struct pm_parser *q)
{
	*q = (struct pm_parser) {
		.self.parser = p,
		.fn = pm_try_fn
	};
}

bool pm_parse(struct pm_parser *p, const char *src, long len,
	union pm_result *res)
{
	struct pm_state state = { .pos = 0, .line = 0 };
	return p->fn(p->self, src, len, &state, res);
}

bool pm_space_fn(union pm_data d, const char *src, long len,
	struct pm_state *state, union pm_result *res)
{
	if (pm_out_of_range(src, len, state, res)) {
		return false;
	}
	const char c = pm_step_state(src, state);
	if (c == ' ' || c == '\t') {
		res->value = pm_prim_c(c);
		return true;
	}
	return false;
}

struct pm_parser pm_space = {
	.self.parser = NULL,
	.fn = pm_space_fn,
};

struct pm_parser pm_newline = {
	.self.prim.c = '\n',
	.fn = pm_char_fn,
};

struct pm_parser pm_tab = {
	.self.prim.c = '\t',
	.fn = pm_char_fn,
};

bool pm_upper_fn(union pm_data d, const char *src, long len,
	struct pm_state *state, union pm_result *res)
{
	if (pm_out_of_range(src, len, state, res)) {
		return false;
	}
	const char c = pm_step_state(src, state);
	if (c >= 'A' && c <= 'Z') {
		res->value = pm_prim_c(c);
		return true;
	}
	return false;
}

struct pm_parser pm_upper = {
	.self.ptr = NULL,
	.fn = pm_upper_fn,
};

bool pm_lower_fn(union pm_data d, const char *src, long len,
	struct pm_state *state, union pm_result *res)
{
	if (pm_out_of_range(src, len, state, res)) {
		return false;
	}
	const char c = pm_step_state(src, state);
	if (c >= 'a' && c <= 'z') {
		res->value = pm_prim_c(c);
		return true;
	}
	return false;
}

struct pm_parser pm_lower = {
	.self.ptr = NULL,
	.fn = pm_lower_fn,
};

bool pm_alpha_num_fn(union pm_data d, const char *src, long len,
	struct pm_state *state, union pm_result *res)
{
	if (pm_out_of_range(src, len, state, res)) {
		return false;
	}
	const char c = pm_step_state(src, state);
	if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9')) {
		res->value = pm_prim_c(c);
		return true;
	}
	return false;
}

struct pm_parser pm_alpha_num = {
	.self.ptr = NULL,
	.fn = pm_alpha_num_fn,
};

bool pm_letter_fn(union pm_data d, const char *src, long len,
	struct pm_state *state, union pm_result *res)
{
	if (pm_out_of_range(src, len, state, res)) {
		return false;
	}
	const char c = pm_step_state(src, state);
	if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z')) {
		res->value = pm_prim_c(c);
		return true;
	}
	return false;
}

struct pm_parser pm_letter_num = {
	.self.ptr = NULL,
	.fn = pm_letter_fn,
};

bool pm_digit_fn(union pm_data d, const char *src, long len,
	struct pm_state *state, union pm_result *res)
{
	if (pm_out_of_range(src, len, state, res)) {
		return false;
	}
	const char c = pm_step_state(src, state);
	if (c >= '0' && c <= '9') {
		res->value = pm_prim_c(c);
		return true;
	}
	return false;
}

struct pm_parser pm_digit = {
	.self.parser = NULL,
	.fn = pm_digit_fn,
};

bool pm_hex_digit_fn(union pm_data d, const char *src, long len,
	struct pm_state *state, union pm_result *res)
{
	if (pm_out_of_range(src, len, state, res)) {
		return false;
	}
	const char c = pm_step_state(src, state);
	if ((c >= '0' && c <= '9') || (c >= 'a' && c <= 'f') || (c >= 'A' && c <= 'F')) {
		res->value = pm_prim_c(c);
		return true;
	}
	return false;
}

struct pm_parser pm_hex_digit_num = {
	.self.ptr = NULL,
	.fn = pm_hex_digit_fn,
};

bool pm_oct_digit_fn(union pm_data d, const char *src, long len,
	struct pm_state *state, union pm_result *res)
{
	if (pm_out_of_range(src, len, state, res)) {
		return false;
	}
	const char c = pm_step_state(src, state);
	if (c >= '0' && c <= '7') {
		res->value = pm_prim_c(c);
		return true;
	}
	return false;
}

struct pm_parser pm_oct_digit = {
	.self.parser = NULL,
	.fn = pm_oct_digit_fn,
};

bool pm_any_char_fn(union pm_data d, const char *src, long len,
	struct pm_state *state, union pm_result *res)
{
	if (pm_out_of_range(src, len, state, res)) {
		return false;
	}
	res->value = pm_prim_c(pm_step_state(src, state));
	return true;
}

struct pm_parser pm_any_char = {
	.self.parser = NULL,
	.fn = pm_any_char_fn,
};
