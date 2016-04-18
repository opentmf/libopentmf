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

#ifndef _OPENTMF_DRIVER_H_
#define _OPENTMF_DRIVER_H_

#include "common.h"

#define OPENTMF_LICENSE_GNU_LGPL_V3 "GNU LGPL v3"
#define OPENTMF_LICENSE_GNU_LGPL_V2_1 "GNU LGPL v2.1"
#define OPENTMF_LICENSE_MPL_2_0 "MPL-2.0" // Mozilla Public License 2.0
#define OPENTMF_LICENSE_CC_ZERO_1_0 "CC0 1.0"

struct opentmf_driver_info
{
  struct opentmf_version version;
  const char* name;
  const char* description;
  const char* authors;
  const char* license;
  opentmf_bool non_free;
};

#ifdef OPENTMF_DRIVER
OPENTMF_API int opentmf_drv_init(void** context_data);
OPENTMF_API void opentmf_drv_fini(void* context_data);
OPENTMF_API const struct opentmf_driver_info* opentmf_drv_get_info();
OPENTMF_API int opentmf_drv_get_device_list(char*** list);
OPENTMF_API void opentmf_drv_free_device_list(char** list);
#else
OPENTMF_API const struct opentmf_driver_info* opentmf_drv_get_info(struct opentmf_handle* handle);
OPENTMF_API int opentmf_drv_get_device_list(struct opentmf_handle* handle, char*** list);
OPENTMF_API int opentmf_drv_free_device_list(struct opentmf_handle* handle, char** list);
#endif

#endif
