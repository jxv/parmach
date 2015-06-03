#include "parmach.h"
#include "internal.h"

bool pm_out_of_range(const char *src, long len, struct pm_state *state, struct pm_result *res)
{
	if (state->pos >= 0 && state->pos < len) {
		return false;
	}
	if (res) {
		res->error.state = *state;
	}
	return true;
	
}

inline
char pm_step_state(const char *src, struct pm_state *state)
{
	const char c = src[state->pos];
	state->pos++;
	if (c == '\n') {
		state->line++;
	}
	return c;
}

bool pm_parse_step(struct pm_parser p[1], const char *src, long len, struct pm_state *state, struct pm_result *res)
{
	return p->fn(p->self, src, len, state, res);
}

bool pm_parse(struct pm_parser *p, const char *src, long len, struct pm_result *res)
{
	struct pm_state state = { .pos = 0, .line = 0 };
	return pm_parse_step(p, src, len, &state, res);
}
