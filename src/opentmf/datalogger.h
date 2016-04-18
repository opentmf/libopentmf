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

#ifndef _OPENTMF_DATALOGGER_H_
#define _OPENTMF_DATALOGGER_H_

#include "common.h"

#ifdef OPENTMF_DRIVER

#ifndef OPENTMF_DL_API_VERSION
  #warning "OPENTMF_DL_API_VERSION not defined, defaulting to disabled (0x0000)"
  #define OPENTMF_DL_API_VERSION  0x0000
#endif

#if OPENTMF_DL_API_VERSION > 0x0000
OPENTMF_API uint16_t opentmf_dl_get_version();
OPENTMF_API int opentmf_dl_open(void* driver_data, const char* id, void** device_data);
OPENTMF_API void opentmf_dl_close(void* device_data);
OPENTMF_API const struct opentmf_device_info* opentmf_dl_get_info(void* device_data);
#endif

#else
#endif

#endif
