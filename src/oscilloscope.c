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

#include <dlfcn.h>
#include "oscilloscope.h"
#include "utils.h"

#define API_VERSION_MIN API_VERSION(0,0)

#define LOAD_OPT_FUNC(x) (*api)->x = dlsym(driver, "opentmf_os_" #x)

#define HANDLE_TYPE OPENTMF_HT_OSCILLOSCOPE
#define HANDLE_DATA ((struct os_data*)handle->handle_data)

struct os_api
{
  uint16_t (*get_version)();
  void (*close)(void* device_data);
  const struct opentmf_device_info* (*get_info)(void* device_data);
};

struct os_data
{
  struct os_api* api;
};

int os_load(void* driver, struct os_api** api)
{
  *api = calloc(1, sizeof(**api));

  if(!*api)
    return OPENTMF_E_NO_MEMORY;

  // Get API version:
  LOAD_MAN_FUNC(get_version);
  const uint16_t version = (*api)->get_version();

  if(version <= API_VERSION_MIN)
    return OPENTMF_E_FAILED;

  // Load functions:
  LOAD_MAN_FUNC(close);
  LOAD_MAN_FUNC(get_info);

  return OPENTMF_SUCCESS;
}

void os_close(struct opentmf_handle* handle)
{
  DRIVER_API->close(DEVICE_DATA);
  free(handle->handle_data);
}

int os_open(struct opentmf_handle* handle, struct os_api* api)
{
  handle->handle_data = calloc(1, sizeof(struct os_data));
  if(!handle->handle_data)
    return OPENTMF_E_NO_MEMORY;

  handle->close = os_close;

  HANDLE_DATA->api = api;

  return OPENTMF_SUCCESS;
}

const struct opentmf_device_info* os_get_info(struct opentmf_handle* handle)
{
  return DRIVER_API->get_info(DEVICE_DATA);
}
