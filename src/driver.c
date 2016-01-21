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
#include "driver.h"
#include "utils.h"

#define HANDLE_TYPE OPENTMF_HT_DRIVER
#define HANDLE_DATA ((struct driver_data*)handle->handle_data)

struct driver_data
{
  void* handle;
  int (*drv_init)(void** private_data);
  void (*drv_fini)(void* private_data);
  const struct opentmf_driver_info* (*drv_get_info)();
};

void drv_close(struct opentmf_handle* handle)
{
  if(HANDLE_DATA->drv_fini)
    HANDLE_DATA->drv_fini(handle->private_data);

  dlclose(HANDLE_DATA->handle);
  free(handle->handle_data);

   // Remove from linked list of open drivers:
  struct driver** driver = &handle->ctx->drivers;
  while(*driver)
  {
    if((*driver)->handle == handle)
    {
      free((*driver)->name);
      free(*driver);
      *driver = (*driver)->next;
      break;
    }
    driver = &(*driver)->next;
  }
}

int drv_open(struct opentmf_handle* handle, void* driver)
{
  handle->handle_data = calloc(1, sizeof(struct driver_data));
  if(!handle->handle_data)
  {
    dlclose(driver);
    return OPENTMF_E_NO_MEMORY;
  }

  handle->close = drv_close;
  HANDLE_DATA->handle = driver;

  LOAD_MAN_FUNC(drv_get_info);
  LOAD_OPT_FUNC(drv_init);
  LOAD_OPT_FUNC(drv_fini);

  if(HANDLE_DATA->drv_init)
    return HANDLE_DATA->drv_init(&handle->private_data);
  else
    return OPENTMF_SUCCESS;
}

const struct opentmf_driver_info* opentmf_drv_get_info(struct opentmf_handle* handle)
{
  if(handle && handle->type == HANDLE_TYPE)
    return HANDLE_DATA->drv_get_info();

  return NULL;
}
