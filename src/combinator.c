#include "parmach.h"

bool pm_choice_fn(const pm_data_t d, const str_t *src, pm_state_t *state, pm_result_t *res)
{
	pm_parsers_t *p = d.ptr;
	for (long i = 0; i < p->len; i++) {
		pm_parser_t *q = p->data + i;
		if ((q->fn)(q->data, src, state, res))
			return true;
	}
	return false;
}

bool pm_choice_try_fn(const pm_data_t d, const str_t *src, pm_state_t *state, pm_result_t *res)
{
	pm_parsers_t *p = d.ptr;
	for (long i = 0; i < p->len; i++) {
		pm_parser_t try = PM_TRY(p->data + i);
		if (pm_parse_step(&try, src, state, res))
			return true;
	}
	return false;
}

bool pm_trail_fn(const pm_data_t d, const str_t *src, pm_state_t *state, pm_result_t *res)
{
	str_t *s = res->data.ptr;
	s->data = state->pos + (char *)src->data;
	s->len = 0;
	while (state->pos < src->len) {
		const char c = pm_step_state(src, state);
		if (c == '\n' || c == '\r' || state->pos == src->len)
			break;
		s->len++;
	}
	return true;
}

const pm_parser_t pm_trail = PM_FN(pm_trail_fn);

bool pm_until_fn(const pm_data_t d, const str_t *src, pm_state_t *state, pm_result_t *res)
{
	str_t *s = res->data.ptr;
	s->data = state->pos + (char *)src->data;
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

const pm_parser_t pm_until_space = PM_UNTIL((void *)&pm_space);
