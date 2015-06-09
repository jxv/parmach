#include "parmach.h"

#include <stdio.h>

bool pm_choice_fn(const union pm_data d, const str_t *src, struct pm_state *state, struct pm_result *res)
{
	struct pm_parsers *p = d.ptr;
	for (long i = 0; i < p->len; i++) {
		struct pm_parser *q = p->data + i;
		if ((q->fn)(q->data, src, state, res))
			return true;
	}
	return false;
}

void pm_choice(struct pm_parsers *p, struct pm_parser *q)
{
	*q = (struct pm_parser) { .data.ptr = p, pm_choice_fn };
}

bool pm_choice_try_fn(const union pm_data d, const str_t *src, struct pm_state *state, struct pm_result *res)
{
	pm_parsers_t *p = d.ptr;
	for (long i = 0; i < p->len; i++) {
		pm_parser_t try = PM_TRY(p->data + i);
		if (pm_parse_step(&try, src, state, res))
			return true;
	}
	return false;
}

void pm_choice_try(struct pm_parsers *p, struct pm_parser *q)
{
	*q = (struct pm_parser) { .data.ptr = p, pm_choice_try_fn };
}

bool pm_trail_fn(const union pm_data d, const str_t *src, struct pm_state *state, struct pm_result *res)
{
	str_t *s = res->data.ptr;
	s->data = state->pos + (char *)src;
	s->len = 0;
	while (state->pos < src->len) {
		const char c = pm_step_state(src, state);
		if (c == '\n' || c == '\r' || state->pos == src->len)
			break;
		s->len++;
	}
	return true;
}

struct pm_parser pm_trail = { .data.ptr = NULL, pm_trail_fn };

bool pm_until_fn(const union pm_data d, const str_t *src, struct pm_state *state, struct pm_result *res)
{
	str_t *s = res->data.ptr;
	s->data = state->pos + (char *)src;
	s->len = 0;
	pm_parser_t try = PM_TRY(d.ptr);
	while (state->pos < src->len) {
		if (pm_parse_step(&try, src, state, NULL))
			return true;
		if (pm_out_of_range(src, state, NULL))
			return false;
		pm_step_state(src, state);
		s->len++;
	}
	return true;
}

void pm_until(struct pm_parser *p, struct pm_parser *q)
{
	*q = (struct pm_parser) { .data.ptr = p, pm_until_fn };
}

struct pm_parser pm_until_space = PM_UNTIL(&pm_space);
