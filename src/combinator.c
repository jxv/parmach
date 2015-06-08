#include "parmach.h"
#include "internal.h"

#include <stdio.h>

bool pm_choice_fn(const union pm_data d, const char *src, long len, struct pm_state *state, struct pm_result *res)
{
	struct pm_parsers *p = d.ptr;
	for (long i = 0; i < p->len; i++) {
		struct pm_parser *q = p->data + i;
		if ((q->fn)(q->self, src, len, state, res))
			return true;
	}
	return false;
}

void pm_choice(struct pm_parsers *p, struct pm_parser *q)
{
	*q = (struct pm_parser) { .self.ptr = p, pm_choice_fn };
}

bool pm_choice_try_fn(const union pm_data d, const char *src, long len, struct pm_state *state, struct pm_result *res)
{
	struct pm_parsers *p = d.ptr;
	struct pm_parser try;
	for (long i = 0; i < p->len; i++) {
		pm_try(p->data + i, &try);
		if (pm_parse_step(&try, src, len, state, res))
			return true;
	}
	return false;
}

void pm_choice_try(struct pm_parsers *p, struct pm_parser *q)
{
	*q = (struct pm_parser) { .self.ptr = p, pm_choice_try_fn };
}

bool pm_trail_fn(const union pm_data d, const char *src, long len, struct pm_state *state, struct pm_result *res)
{
	str_t *s = res->value.data.ptr;
	s->data = state->pos + (char *)src;
	s->len = 0;
	while (state->pos < len) {
		const char c = pm_step_state(src, state);
		if (c == '\n' || state->pos == len)
			break;
		s->len++;
	}
	return true;
}

struct pm_parser pm_trail = { .self.ptr = NULL, pm_trail_fn };

bool pm_until_fn(const union pm_data d, const char *src, long len, struct pm_state *state, struct pm_result *res)
{
	str_t *s = res->value.data.ptr;
	s->data = state->pos + (char *)src;
	s->len = 0;
	struct pm_parser try;
	pm_try(d.ptr, &try);
	while (state->pos < len) {
		if (pm_parse_step(&try, src, len, state, NULL))
			return true;
		if (pm_out_of_range(src, len, state, NULL))
			return false;
		pm_step_state(src, state);
		s->len++;
	}
	return true;
}

void pm_until(struct pm_parser *p, struct pm_parser *q)
{
	*q = (struct pm_parser) { .self.ptr = p, pm_until_fn };
}

struct pm_parser pm_until_space = { .self.ptr = &pm_space, pm_until_fn };
