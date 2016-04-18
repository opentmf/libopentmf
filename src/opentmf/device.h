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

#ifndef _OPENTMF_DEVICE_H_
#define _OPENTMF_DEVICE_H_

#include "common.h"

struct opentmf_device_info
{
  const char* name;
  const char* serial;
};

#ifndef OPENTMF_DRIVER
OPENTMF_API const struct opentmf_device_info* opentmf_dev_get_info(struct opentmf_handle* handle);
#endif

#endif
