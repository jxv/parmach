#include "parmach.h"

bool pm_or_fn(const pm_data_t d, const str_t *src,  pm_state_t *state, pm_result_t *res)
{
	struct pm_parser *p1 = d.ptr, *p2 = p1 + 1;
	return
		p1->fn(p1->data, src, state, res) ||
		p2->fn(p2->data, src, state, res);
}

bool pm_and_fn(const pm_data_t d, const str_t *src, pm_state_t *state, pm_result_t *res)
{
	struct pm_parser *p1 = d.ptr, *p2 = p1 + 1;
	return
		p1->fn(p1->data, src, state, res) &&
		p2->fn(p2->data, src, state, res);
}

bool pm_try_fn(const pm_data_t d, const str_t *src, pm_state_t *state, pm_result_t *res)
{
	pm_state_t backup = *state;
	if (pm_parse_step(d.ptr, src, state, res))
		return true;
	*state = backup;
	return false;
}

bool pm_eof_fn(const pm_data_t d, const str_t *src, pm_state_t *state, pm_result_t *res)
{
	if (src->len == state->pos) {
		if (res)
			res->data.ptr = NULL;
		return true; 
	}
	if (res)
		res->error.state = *state;
	return false;
}

struct pm_parser pm_eof = PM_FN(pm_eof_fn);
