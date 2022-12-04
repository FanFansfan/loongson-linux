// SPDX-License-Identifier: MIT
/*
 * Copyright © 2019 Intel Corporation
 */

#include "intel_llc.h"


void intel_llc_enable(struct intel_llc *)
{
}

void intel_llc_disable(struct intel_llc *)
{
	/* Currently there is no HW configuration to be done to disable. */
}

#if IS_ENABLED(CONFIG_DRM_I915_SELFTEST)
#include "selftest_llc.c"
#endif
