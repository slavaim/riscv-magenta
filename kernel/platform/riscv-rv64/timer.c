// Copyright 2016 The Fuchsia Authors
// Copyright (c) 2017 Slava Imameev
//
// Use of this source code is governed by a MIT-style
// license that can be found in the LICENSE file or at
// https://opensource.org/licenses/MIT

#include <platform.h>
#include <sys/types.h>
#include <platform/timer.h>
#include <debug.h>

lk_time_t current_time(void)
{
    PANIC_UNIMPLEMENTED;
}

status_t platform_set_periodic_timer(platform_timer_callback callback, void *arg, lk_time_t interval)
{
    PANIC_UNIMPLEMENTED;
}