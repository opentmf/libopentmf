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

#ifndef _INTERNAL_H_
#define _INTERNAL_H_

#include <stdlib.h>
#include "opentmf/common.h"

struct driver
{
  char* name;
  struct opentmf_handle* handle;
  struct driver* next;
};

struct opentmf_context
{
  size_t ref_count;
  struct driver* drivers;
};

struct opentmf_handle
{
  size_t ref_count;
  enum opentmf_handle_type type;
  struct opentmf_context* ctx;
  void* handle_data;
  void* private_data;
  void (*close)(struct opentmf_handle* handle);
};

#endif
