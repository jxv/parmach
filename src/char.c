#include <stdio.h>
#include <string.h>
#include "parmach.h"

static bool is_number(char c)
{
	return c >= '0' && c <= '9';
}

static bool is_lower(char c)
{
	return c >= 'a' && c <= 'z';
}

static bool is_upper(char c)
{
	return c >= 'A' && c <= 'Z';
}

static bool is_hexadecimal(char c)
{
	return is_number(c) ||
		(c >= 'a' && c <= 'f') ||
		(c >= 'A' && c <= 'F');
}

bool pm_one_of_fn(const pm_data_t d, const str_t *src, pm_state_t *state, pm_result_t *res)
{
	const str_t *s = d.ptr;
	if (pm_out_of_range(src, state, res))
		return false;
	const char c = pm_step_state(src, state);
	for (long i = 0; i < s->len; i++) {
		if (c == s->data[i]) {
			if (res)
				res->data.prim.c = c;
			return true;
		}
	}
	if (res)
		res->error.state = *state;
	return false;
}

bool pm_none_of_fn(const pm_data_t d, const str_t *src, pm_state_t *state, pm_result_t *res)
{
	const str_t *s = d.ptr;
	if (pm_out_of_range(src, state, res))
		return false;
	const char c = pm_step_state(src, state);
	for (long i = 0; i < s->len; i++) {
		if (c == s->data[i]) {
			if (res)
				res->error.state = *state;
			return false;
		}
	}
	if (res)
		res->data.prim.c = c;
	return true;
}

bool pm_char_fn(const pm_data_t d, const str_t *src, pm_state_t *state, pm_result_t *res)
{
	if (pm_out_of_range(src, state, res))
		return false;
	const char c = pm_step_state(src, state);
	if (c == d.prim.c) {
		if (res)
			res->data.prim.c = c;
		return true;
	}
	if (res)
		res->error.state = *state;
	return false;
}

bool pm_satisfy_fn(const pm_data_t d, const str_t *src, pm_state_t *state, pm_result_t *res)
{
	if (pm_out_of_range(src, state, res))
		return false;
	const char c = pm_step_state(src, state);
	bool (**fn)(char) = d.ptr;
	if ((*fn)(c)) {
		if (res)
			res->data.prim.c = c;
		return true;
	}
	if (res)
		res->error.state = *state;
	return false;
}

bool pm_string_fn(const pm_data_t d, const str_t *src, pm_state_t *state, pm_result_t *res)
{
	const str_t *s = d.ptr;
	for (long i = 0; i < s->len; i++) {
		if (pm_out_of_range(src, state, res))
			return false;
		const char c = pm_step_state(src, state);
		if (c != s->data[i]) {
			if (res)
				res->error.state = *state;
			return false;
		}
	}
	if (res) {
		// TODO: group, tag
		str_t *sv = res->data.ptr;
		sv->data = (char*)src;
		sv->len = s->len;
	}
	return true;
}

bool pm_space_fn(pm_data_t d, const str_t *src, pm_state_t *state, pm_result_t *res)
{
	if (pm_out_of_range(src, state, res))
		return false;
	const char c = pm_step_state(src, state);
	if (c == ' ' || c == '\t') {
		if (res)
			res->data.prim.c = c;
		return true;
	}
	if (res)
		res->error.state = *state;
	return false;
}

struct pm_parser pm_space = PM_FN(pm_space_fn);

struct pm_parser pm_newline = PM_CHAR('\n');

struct pm_parser pm_tab = PM_CHAR('\t');

bool pm_upper_fn(const pm_data_t d, const str_t *src, pm_state_t *state, pm_result_t *res)
{
	if (pm_out_of_range(src,  state, res))
		return false;
	const char c = pm_step_state(src, state);
	if (c >= 'A' && c <= 'Z') {
		if (res)
			res->data.prim.c = c;
		return true;
	}
	if (res)
		res->error.state = *state;
	return false;
}

struct pm_parser pm_upper = PM_FN(pm_upper_fn);

bool pm_lower_fn(const pm_data_t d, const str_t *src, pm_state_t *state, pm_result_t *res)
{
	if (pm_out_of_range(src, state, res))
		return false;
	const char c = pm_step_state(src, state);
	if (is_lower(c)) {
		if (res)
			res->data.prim.c = c;
		return true;
	}
	if (res)
		res->error.state = *state;
	return false;
}

struct pm_parser pm_lower = PM_FN(pm_lower_fn);

bool pm_alpha_num_fn(const pm_data_t d, const str_t *src, pm_state_t *state, pm_result_t *res)
{
	if (pm_out_of_range(src, state, res))
		return false;
	const char c = pm_step_state(src, state);
	if (is_lower(c) || is_upper(c) || is_number(c)) {
		if (res)
			res->data.prim.c = c;
		return true;
	}
	if (res)
		res->error.state = *state;
	return false;
}

struct pm_parser pm_alpha_num = PM_FN(pm_alpha_num_fn);

bool pm_letter_fn(const pm_data_t d, const str_t *src, pm_state_t *state, pm_result_t *res)
{
	if (pm_out_of_range(src, state, res))
		return false;
	const char c = pm_step_state(src, state);
	if (is_lower(c) || is_upper(c)) {
		if (res)
			res->data.prim.c = c;
		return true;
	}
	if (res)
		res->error.state = *state;
	return false;
}

struct pm_parser pm_letter = PM_FN(pm_letter_fn);

bool pm_digit_fn(const pm_data_t d, const str_t *src, pm_state_t *state, pm_result_t *res)
{
	if (pm_out_of_range(src, state, res))
		return false;
	const char c = pm_step_state(src, state);
	if (is_number(c)) {
		if (res)
			res->data.prim.c = c;
		return true;
	}
	if (res)
		res->error.state = *state;
	return false;
}

struct pm_parser pm_digit = PM_FN(pm_digit_fn);

bool pm_hex_digit_fn(const pm_data_t d, const str_t *src, pm_state_t *state, pm_result_t *res)
{
	if (pm_out_of_range(src, state, res))
		return false;
	const char c = pm_step_state(src, state);
	if (is_hexadecimal(c)) {
		if (res)
			res->data.prim.c = c;
		return true;
	}
	if (res)
		res->error.state = *state;
	return false;
}

struct pm_parser pm_hex_digit_num = PM_FN(pm_hex_digit_fn);

bool pm_oct_digit_fn(const pm_data_t d, const str_t *src, pm_state_t *state, pm_result_t *res)
{
	if (pm_out_of_range(src, state, res))
		return false;
	const char c = pm_step_state(src, state);
	if (c >= '0' && c <= '7') {
		if (res)
			res->data.prim.c = c;
		return true;
	}
	if (res)
		res->error.state = *state;
	return false;
}

struct pm_parser pm_oct_digit = PM_FN(pm_oct_digit_fn);

bool pm_any_char_fn(const pm_data_t d, const str_t *src, pm_state_t *state, pm_result_t *res)
{
	if (pm_out_of_range(src, state, res))
		return false;
	if (res)
		res->data.prim.c = pm_step_state(src, state);
	return true;
}

struct pm_parser pm_any_char = PM_FN(pm_any_char_fn);

bool pm_crlf_fn(pm_data_t d, const str_t *src, pm_state_t *state, pm_result_t *res)
{
	if (pm_out_of_range(src, state, res))
		return false;
	if (pm_step_state(src, state) != '\r')
		goto fail;
	if (pm_out_of_range(src, state, res))
		return false;
	if (pm_step_state(src, state) != '\n')
		goto fail;
	return true;
fail:
	if (res)
		res->error.state = *state;
	return false;
}

struct pm_parser pm_crlf = PM_FN(pm_crlf_fn);
