/**
 * This file is part of libopentmf.
 *
 * Copyright (c) 2016 Reinder Feenstra <reinderfeenstra@gmail.com>
 *
 * libopentmf is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * libopentmf is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with libopentmf; If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef _CORE_H_
#define _CORE_H_

#include "internal.h"

#define HANDLE_ADD_REF(handle) { (handle)->ref_count++; }
#define HANDLE_RELEASE_REF(handle) { if(--((handle)->ref_count) == 0){ free_handle(handle); } }

int create_handle(struct opentmf_context* ctx, struct opentmf_handle** handle);
void free_handle(struct opentmf_handle* handle);
int get_driver_handle(struct opentmf_context* ctx, const char** id, struct opentmf_handle** handle);
size_t get_seperator_pos(const char* str, const char seperator);

#endif