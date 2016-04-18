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

#ifndef _FUNCTIONGENERATOR_H_
#define _FUNCTIONGENERATOR_H_

#include "internal.h"

struct fg_api;

int fg_load(void* driver, struct fg_api** api);
int fg_open(struct opentmf_handle* handle, struct fg_api* api);
const struct opentmf_device_info* fg_get_info(struct opentmf_handle* handle);

#endif
