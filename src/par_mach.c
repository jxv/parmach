#include <stdio.h>
#include "par_mach.h"

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

struct pm_value pm_prim_u8(uint8_t u8)
{
	return (struct pm_value) {
		.group = PM_PRIM,
		.tag = PM_UINT8,
		.data.prim.u8 = u8,
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

struct pm_value pm_prim_u32(uint32_t u32)
{
	return (struct pm_value) {
		.group = PM_PRIM,
		.tag = PM_UINT32,
		.data.prim.u32 = u32,
	};
}

struct pm_value pm_prim_u64(uint64_t u64)
{
	return (struct pm_value) {
		.group = PM_PRIM,
		.tag = PM_UINT64,
		.data.prim.u64 = u64,
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

struct pm_value pm_prim_i16(int16_t i16)
{
	return (struct pm_value) {
		.group = PM_PRIM,
		.tag = PM_INT16,
		.data.prim.i16 = i16,
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

struct pm_value pm_prim_i64(int64_t i64)
{
	return (struct pm_value) {
		.group = PM_PRIM,
		.tag = PM_INT64,
		.data.prim.i64 = i64,
	};
}

struct pm_value pm_prim_f(float f)
{
	return (struct pm_value) {
		.group = PM_PRIM,
		.tag = PM_INT64,
		.data.prim.f = f,
	};
}

struct pm_value pm_prim_d(double d)
{
	return (struct pm_value) {
		.group = PM_PRIM,
		.tag = PM_DOUBLE,
		.data.prim.d = d,
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

bool pm_char_fn(const union pm_data d,
	const char *src, long len,
	struct pm_state *state, union pm_result *res)
{
	if (pm_out_of_range(src, len, state, res)) {
		return false;
	}
	if (src[state->pos] == d.prim.c) {
		res->value = pm_prim_c(d.prim.c);
		state->pos++;
		return true;
	}
	state->pos++;
	return false;
}

void pm_char(char c, struct pm_parser *q)
{
	*q = (struct pm_parser) {
		.self.prim.c = c,
		.fn = pm_char_fn,
	};
}

bool pm_or_fn(const union pm_data d,
	const char *src, long len,
	struct pm_state *state, union pm_result *res)
{
	const struct pm_parser *p = d.ptr;
	return
		p[0].fn(p[0].self, src, len, state, res) ||
		p[1].fn(p[1].self, src, len, state, res);
}

void pm_or(struct pm_parser *ps, struct pm_parser *q)
{
	*q = (struct pm_parser) {
		.self.parser = ps,
		.fn = pm_or_fn,
	};
}

bool pm_try_fn(union pm_data d,
	const char *src, long len,
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

bool pm_parse(struct pm_parser *p, const char *src, long len, union pm_result *res)
{
	struct pm_state state = { .pos = 0, .row = 0, .col = 0 };
	return p->fn(p->self, src, len, &state, res);
}