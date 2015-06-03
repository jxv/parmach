#include "parmach.h"
#include "internal.h"

bool pm_choice_fn(const union pm_data d, const char *src, long len, struct pm_state *state, union pm_result *res)
{
	if (pm_out_of_range(src, len, state, res)) {
		return false;
	}
	struct pm_parsers *p = d.ptr;
	for (long i = 0; i < p->len; i++) {
		struct pm_parser *q = p->data + i;
		if ((q->fn)(q->self, src, len, state, res)) {
			return true;
		}
	}
	res->error.state = *state;
	return false;
}

void pm_choice(struct pm_parsers *p, struct pm_parser *q)
{
	*q = (struct pm_parser) {
		.self.ptr = p,
		.fn = pm_choice_fn,
	};
}

bool pm_trail_fn(const union pm_data d, const char *src, long len, struct pm_state *state, union pm_result *res)
{
	struct pm_str *str = res->value.data.str;
	str->data = state->pos + src;
	str->len = 0;
	while (state->pos < len) {
		const char c = pm_step_state(src, state);
		if (c == '\n' || state->pos == len) {
			break;
		}
		str->len++;
	}
	return true;
}

struct pm_parser pm_trail = {
	.self.ptr = NULL,
	.fn = pm_trail_fn,
};
