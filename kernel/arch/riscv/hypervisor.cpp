// Copyright 2016 The Fuchsia Authors
// Copyright 2017 Slava Imameev
//
// Use of this source code is governed by a MIT-style
// license that can be found in the LICENSE file or at
// https://opensource.org/licenses/MIT

#include <arch/hypervisor.h>

/* Create a hypervisor context.
 * This setups up the CPUs to allow a hypervisor to be run.
 */
status_t arch_hypervisor_create(mxtl::unique_ptr<HypervisorContext>* context)
{
    return ERR_NOT_SUPPORTED;
}

/* Create a guest context.
 * This creates the structures to allow a guest to be run.
 */
status_t arch_guest_create(mxtl::RefPtr<VmObject> guest_phys_mem,
                           mxtl::unique_ptr<GuestContext>* context)
{
    return ERR_NOT_SUPPORTED;
}

/* Start a guest within a guest context.
 */
status_t arch_guest_start(const mxtl::unique_ptr<GuestContext>& context, uintptr_t guest_entry)
{
    return ERR_NOT_SUPPORTED;
}
