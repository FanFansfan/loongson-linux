/* SPDX-License-Identifier: MIT */
/*
 * Copyright © 2022 Intel Corporation
 */

#ifndef _XE_GUC_CT_H_
#define _XE_GUC_CT_H_

#include "xe_guc_ct_types.h"

struct drm_printer;

int xe_guc_ct_init(struct xe_guc_ct *ct);
int xe_guc_ct_enable(struct xe_guc_ct *ct);
void xe_guc_ct_disable(struct xe_guc_ct *ct);
void xe_guc_ct_fast_path(struct xe_guc_ct *ct);

struct xe_guc_ct_snapshot *
xe_guc_ct_snapshot_capture(struct xe_guc_ct *ct, bool atomic);
void xe_guc_ct_snapshot_print(struct xe_guc_ct_snapshot *snapshot,
			      struct drm_printer *p);
void xe_guc_ct_snapshot_free(struct xe_guc_ct_snapshot *snapshot);
void xe_guc_ct_print(struct xe_guc_ct *ct, struct drm_printer *p, bool atomic);

static inline void xe_guc_ct_irq_handler(struct xe_guc_ct *ct)
{
	if (!ct->enabled)
		return;

	wake_up_all(&ct->wq);
	queue_work(system_unbound_wq, &ct->g2h_worker);
	xe_guc_ct_fast_path(ct);
}

/* Basic CT send / receives */
int xe_guc_ct_send(struct xe_guc_ct *ct, const u32 *action, u32 len,
		   u32 g2h_len, u32 num_g2h);
int xe_guc_ct_send_locked(struct xe_guc_ct *ct, const u32 *action, u32 len,
			  u32 g2h_len, u32 num_g2h);
int xe_guc_ct_send_recv(struct xe_guc_ct *ct, const u32 *action, u32 len,
			u32 *response_buffer);
static inline int
xe_guc_ct_send_block(struct xe_guc_ct *ct, const u32 *action, u32 len)
{
	return xe_guc_ct_send_recv(ct, action, len, NULL);
}

/* This is only version of the send CT you can call from a G2H handler */
int xe_guc_ct_send_g2h_handler(struct xe_guc_ct *ct, const u32 *action,
			       u32 len);

/* Can't fail because a GT reset is in progress */
int xe_guc_ct_send_recv_no_fail(struct xe_guc_ct *ct, const u32 *action,
				u32 len, u32 *response_buffer);
static inline int
xe_guc_ct_send_block_no_fail(struct xe_guc_ct *ct, const u32 *action, u32 len)
{
	return xe_guc_ct_send_recv_no_fail(ct, action, len, NULL);
}

#endif
