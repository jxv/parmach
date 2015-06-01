#include <stdio.h>
#include <string.h>
#include "parmach.h"
#include "internal.h"

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
