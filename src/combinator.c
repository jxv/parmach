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
