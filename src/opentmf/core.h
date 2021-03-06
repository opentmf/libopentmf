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

#ifndef _OPENTMF_CORE_H_
#define _OPENTMF_CORE_H_

#include "common.h"

#ifndef OPENTMF_DRIVER
OPENTMF_API int opentmf_init(struct opentmf_context** ctx);
OPENTMF_API int opentmf_exit(struct opentmf_context* ctx);
OPENTMF_API const struct opentmf_version* opentmf_get_version();
OPENTMF_API const char* opentmf_get_status_str(int status);

OPENTMF_API int opentmf_get_driver_list(struct opentmf_context* ctx, char*** list);
OPENTMF_API int opentmf_free_driver_list(struct opentmf_context* ctx, char** list);

OPENTMF_API int opentmf_open(struct opentmf_context* ctx, const char* url, struct opentmf_handle** handle);
OPENTMF_API int opentmf_close(struct opentmf_handle* handle);
OPENTMF_API int opentmf_get_handle_type(struct opentmf_handle* handle);
#endif

#endif
