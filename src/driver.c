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
#include <string.h>
#include "core.h"
#include "driver.h"
#include "oscilloscope.h"
#include "datalogger.h"
#include "functiongenerator.h"
#include "utils.h"

#define HANDLE_TYPE OPENTMF_HT_DRIVER
#define HANDLE_DATA ((struct driver_data*)handle->handle_data)

#define DEVICE_PATH_OSCILLOSCOPE "/os/"
#define DEVICE_PATH_DATALOGGER "/dl/"
#define DEVICE_PATH_FUNCTIONGENERATOR "/fg/"

#define PROTO_DEV_FUNC(_prefix) \
  int (*_prefix ## _open)(void* driver_data, const char* id, void** device_data); \
  struct _prefix ## _api* _prefix;

#define LOAD_OPT_FUNC(x) HANDLE_DATA->x = dlsym(driver, "opentmf_" #x)

#define LOAD_DEV_FUNC(_prefix) \
  if(!(LOAD_OPT_FUNC(_prefix ## _open)) || \
     _prefix ##_load(driver, &HANDLE_DATA->_prefix) != OPENTMF_SUCCESS) \
  { \
    HANDLE_DATA->_prefix ## _open = NULL; \
    if(HANDLE_DATA->_prefix) \
    { \
      free(HANDLE_DATA->_prefix); \
      HANDLE_DATA->_prefix = NULL; \
    } \
  }

#define OPEN_DEV(_prefix, _name) \
  if(HANDLE_DATA->_prefix ## _open && \
     strncmp(id, DEVICE_PATH_ ## _name, strlen(DEVICE_PATH_ ## _name)) == 0) \
  { \
    if((r = _prefix ## _open(device_handle, HANDLE_DATA->_prefix) == OPENTMF_SUCCESS) && \
        (r = HANDLE_DATA->_prefix ## _open(handle->private_data, id + strlen(DEVICE_PATH_ ## _name), &device_handle->private_data)) == OPENTMF_SUCCESS) \
    { \
      device_handle->type = OPENTMF_HT_ ## _name; \
    } \
  }

struct driver_data
{
  void* handle;
  int (*drv_init)(void** private_data);
  void (*drv_fini)(void* private_data);
  const struct opentmf_driver_info* (*drv_get_info)();
  int (*drv_get_device_list)(char*** list);
  void (*drv_free_device_list)(char** list);
  PROTO_DEV_FUNC(os);
  PROTO_DEV_FUNC(dl);
  PROTO_DEV_FUNC(fg);
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
  LOAD_MAN_FUNC(drv_get_device_list);
  LOAD_MAN_FUNC(drv_free_device_list);

  LOAD_DEV_FUNC(os);
  LOAD_DEV_FUNC(fg);
  LOAD_DEV_FUNC(dl);

  if(HANDLE_DATA->drv_init)
    return HANDLE_DATA->drv_init(&handle->private_data);
  else
    return OPENTMF_SUCCESS;
}

int drv_open_device(struct opentmf_handle* handle, const char* id, struct opentmf_handle* device_handle)
{
  int r = OPENTMF_E_INVALID_URL;

  OPEN_DEV(os, OSCILLOSCOPE)
  else OPEN_DEV(dl, DATALOGGER)
  else OPEN_DEV(fg, FUNCTIONGENERATOR)

  if(r == OPENTMF_SUCCESS)
    device_handle->driver_handle = handle;

  return r;
}

const struct opentmf_driver_info* opentmf_drv_get_info(struct opentmf_handle* handle)
{
  if(handle && handle->type == HANDLE_TYPE)
    return HANDLE_DATA->drv_get_info();

  return NULL;
}

int opentmf_drv_get_device_list(struct opentmf_handle* handle, char*** list)
{
  if(!handle || handle->type != HANDLE_TYPE || !list)
    return OPENTMF_E_INVALID_PARAM;

  int r = HANDLE_DATA->drv_get_device_list(list);

  if(r == OPENTMF_SUCCESS)
    HANDLE_ADD_REF(handle);

  return r;
}

int opentmf_drv_free_device_list(struct opentmf_handle* handle, char** list)
{
  if(!handle || handle->type != HANDLE_TYPE || !list)
    return OPENTMF_E_INVALID_PARAM;

  HANDLE_DATA->drv_free_device_list(list);

  HANDLE_RELEASE_REF(handle);

  return OPENTMF_SUCCESS;
}
