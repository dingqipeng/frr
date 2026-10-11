// SPDX-License-Identifier: GPL-2.0-or-later
#ifdef NDEBUG
#undef NDEBUG
#endif
#include <assert.h>
#include <stdio.h>
#include "bgpd/bgp_gr_epoch.h"

struct scenario {
	bool gr, refresh, usable, stale;
	uint64_t path_epoch, clearing_epoch;
	bool llgr, retain;
};

int main(void)
{
	static const struct scenario cases[] = {
		/* Ordinary clearing and first GR; refresh-only staleness survives. */
		{ false, false, true, false, 0, 1, false, false },
		{ true, false, true, false, 0, 1, false, true },
		{ true, false, true, true, 0, 1, false, true },
		{ true, true, true, true, 0, 1, false, true },
		/* Old GR routes expire; refreshed routes enter the new epoch. */
		{ true, false, true, true, 1, 2, false, false },
		{ true, true, true, true, 1, 2, false, false },
		{ true, false, true, false, 0, 2, false, true },
		/* Repeated work on one epoch must be idempotent. */
		{ true, false, true, true, 2, 2, false, true },
		{ true, true, true, true, 2, 2, false, true },
		/* Refresh without GR remains independent of the prior GR epoch. */
		{ false, true, true, true, 1, 2, false, true },
		{ false, true, true, false, 0, 2, false, true },
		{ false, false, true, true, 1, 2, false, false },
		/* LLGR exception applies to old GR, not arbitrary unusable paths. */
		{ true, false, true, true, 1, 2, true, true },
		{ true, true, true, true, 1, 2, true, true },
		{ true, false, false, true, 1, 2, true, false },
		{ true, false, false, false, 0, 1, false, false },
		{ false, true, false, true, 0, 1, false, false },
		/* Epoch comparison is equality, independent of ordering/wrap. */
		{ true, false, true, true, UINT64_MAX, 1, false, false },
		{ true, false, true, true, UINT64_MAX, UINT64_MAX, false, true },
		{ true, false, true, true, 1, UINT64_MAX, false, false },
	};
	for (unsigned int n = 0; n < sizeof(cases) / sizeof(cases[0]); n++) {
		const struct scenario *s = &cases[n];
		assert(bgp_gr_retain_path(s->gr, s->refresh, s->usable, s->stale, s->path_epoch,
					  s->clearing_epoch, s->llgr) == s->retain);
	}
	puts("BGP GR epoch retention scenarios passed.");
	return 0;
}
