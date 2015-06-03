#include "parmach.h"
#include "internal.h"

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

bool pm_or_fn(const union pm_data d, const char *src, long len, struct pm_state *state, struct pm_result *res)
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

bool pm_and_fn(const union pm_data d, const char *src, long len, struct pm_state *state, struct pm_result *res)
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

bool pm_try_fn(union pm_data d, const char *src, long len, struct pm_state *state, struct pm_result *res)
{
	struct pm_state backup = *state;
	if (pm_parse_step(d.parser, src, len, state, res)) {
		return true;
	}
	*state = backup;
	return false;
}

void pm_try(struct pm_parser *p, struct pm_parser *q)
{
	*q = (struct pm_parser) {
		.self.parser = p,
		.fn = pm_try_fn
	};
}

bool pm_eof_fn(union pm_data d, const char *src, long len, struct pm_state *state, struct pm_result *res)
{
	if (len == state->pos) {
		if (res) {
			res->value.data.ptr = NULL;
		}
		return true; 
	}
	if (res) {
		res->error.state = *state;
	}
	return false;
}

struct pm_parser pm_eof = {
	.self.ptr = NULL,
	.fn = pm_eof_fn,
};
