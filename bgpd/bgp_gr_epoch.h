// SPDX-License-Identifier: GPL-2.0-or-later
#ifndef _FRR_BGP_GR_EPOCH_H
#define _FRR_BGP_GR_EPOCH_H

#include <stdbool.h>
#include <stdint.h>

/* A refresh-stale path has no GR epoch. Reprocessing the same clearing
 * epoch is harmless. LLGR retains old epochs under RFC 9494 section 4.2.
 */
static inline bool bgp_gr_retain_path(bool gr, bool refresh, bool usable, bool stale,
				      uint64_t path_epoch, uint64_t clearing_epoch, bool llgr)
{
	if (!usable)
		return false;
	if (gr)
		return !stale || !path_epoch || path_epoch == clearing_epoch || llgr;
	return refresh;
}

#endif
