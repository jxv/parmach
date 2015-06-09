#include "parmach.h"

bool pm_fail_fn(const str_t *src, pm_state_t *state, pm_result_t *res)
{
	if (res) {
		res->error.state = *state;
	}
	return false;
}

bool pm_out_of_range(const str_t *src, pm_state_t *state, pm_result_t *res)
{
	if (state->pos >= 0 && state->pos < src->len) {
		return false;
	}
	if (res) {
		res->error.state = *state;
	}
	return true;
	
}

inline
char pm_step_state(const str_t *src, pm_state_t *state)
{
	const char c = src->data[state->pos];
	state->pos++;
	if (c == '\n') {
		state->line++;
	}
	return c;
}

bool pm_parse_step(struct pm_parser p[1], const str_t *src, pm_state_t *state, pm_result_t *res)
{
	return p->fn(p->data, src, state, res);
}

bool pm_parse(struct pm_parser *p, const str_t *src, pm_result_t *res)
{
	pm_state_t state = { .pos = 0, .line = 0 };
	return pm_parse_step(p, src, &state, res);
}
