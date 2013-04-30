/*
 * Cross Context BAR1
 *
 * Copyright (c) 2012-2013 Yusuke Suzuki
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */
#include <stdint.h>
#include "a3.h"
#include "a3_context.h"
#include "a3_pramin.h"
#include "a3_shadow_page_table.h"
#include "a3_barrier.h"
#include "a3_device_bar1.h"
namespace a3 {

void context::write_bar1(const command& cmd) {
    if (in_poll_area(cmd.offset)) {
        A3_SYNCHRONIZED(device::instance()->mutex_handle()) {
            device::instance()->bar1()->write(this, cmd);
        }
        return;
    }

    const uint64_t gphys = bar1_channel()->table()->resolve(cmd.offset, NULL);
    if (gphys != UINT64_MAX) {
        barrier::page_entry* entry = NULL;
        if (barrier()->lookup(gphys, &entry, false)) {
            // found
            write_barrier(gphys, cmd);
        }
        pramin::accessor pramin;
        pramin.write(gphys, cmd.value, cmd.size());
        return;
    }
    // A3_LOG("VM BAR1 invalid write 0x%" PRIX32 " access\n", cmd.offset);
}

void context::read_bar1(const command& cmd) {
    if (in_poll_area(cmd.offset)) {
        A3_SYNCHRONIZED(device::instance()->mutex_handle()) {
            buffer()->value = device::instance()->bar1()->read(this, cmd);
        }
        return;
    }

    const uint64_t gphys = bar1_channel()->table()->resolve(cmd.offset, NULL);
    if (gphys != UINT64_MAX) {
        barrier::page_entry* entry = NULL;
        if (barrier()->lookup(gphys, &entry, false)) {
            // found
            read_barrier(gphys, cmd);
        }
        pramin::accessor pramin;
        const uint32_t ret = pramin.read(gphys, cmd.size());
        buffer()->value = ret;
        // A3_LOG("VM BAR1 read 0x%" PRIX32 " access value 0x%" PRIX32 "\n", cmd.offset, ret);
        return;
    }

    A3_LOG("VM BAR1 invalid read 0x%" PRIX32 " access\n", cmd.offset);
    buffer()->value = 0xFFFFFFFF;
}

}  // namespace a3
/* vim: set sw=4 ts=4 et tw=80 : */