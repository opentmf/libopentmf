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

#include <unistd.h>
#include <string.h>
#include <dlfcn.h>
#include "core.h"
#include "driver.h"

#define PROTOCOL_OPENTMF "opentmf://"

#define ID_LEN_MAX 1024
#define PATH_LEN_MAX 2048

#define URL_SEPERATOR '/'
#define SEARCH_PATH_SEPERATOR ':'

static const struct opentmf_version version = {
  .major = 0,
  .minor = 1,
  .patch = 0,
  .extra = ""
};

int opentmf_init(struct opentmf_context** ctx)
{
  if(!ctx)
    return OPENTMF_E_INVALID_PARAM;

  struct opentmf_context* new_ctx = calloc(1, sizeof(*new_ctx));
  if(!new_ctx)
    return OPENTMF_E_NO_MEMORY;

  new_ctx->ref_count = 1;
  *ctx = new_ctx;

  return OPENTMF_SUCCESS;
}

int opentmf_exit(struct opentmf_context* ctx)
{
  if(!ctx)
    return OPENTMF_E_INVALID_PARAM;
  else if(ctx->ref_count != 1)
    return OPENTMF_E_FAILED;

  free(ctx);

  return OPENTMF_SUCCESS;
}

const struct opentmf_version* opentmf_get_version()
{
  return &version;
}

int opentmf_close(struct opentmf_handle* handle)
{
  if(!handle)
    return OPENTMF_E_INVALID_PARAM;

  if(--handle->ref_count == 0)
  {
    handle->ctx->ref_count--;
    if(handle->close)
      handle->close(handle);
    free(handle);
  }

  return OPENTMF_SUCCESS;
}

int opentmf_open(struct opentmf_context* ctx, const char* url, struct opentmf_handle** handle)
{
  int r = OPENTMF_E_INVALID_URL;
  if(strncmp(url, PROTOCOL_OPENTMF, strlen(PROTOCOL_OPENTMF)) == 0)
  {
    struct opentmf_handle* driver_handle;
    const char* id = url + strlen(PROTOCOL_OPENTMF);

    if((r = get_driver_handle(ctx, &id, &driver_handle)) != OPENTMF_SUCCESS)
      return r;

    if(*id == '\0')
    {
      *handle = driver_handle;
      return OPENTMF_SUCCESS;
    }
    else
      r = OPENTMF_E_INVALID_URL;

    opentmf_close(driver_handle);
  }

  return r;
}

int opentmf_get_handle_type(struct opentmf_handle* handle)
{
  if(handle)
    return handle->type;

  return OPENTMF_HT_INVALID;
}

int create_handle(struct opentmf_context* ctx, struct opentmf_handle** handle, enum opentmf_handle_type type)
{
  struct opentmf_handle* new_handle = calloc(1, sizeof(*new_handle));
  if(!new_handle)
    return OPENTMF_E_NO_MEMORY;

  new_handle->ref_count = 1;
  new_handle->type = type;
  new_handle->ctx = ctx;
  *handle = new_handle;

  ctx->ref_count++;

  return OPENTMF_SUCCESS;
}

int get_driver_handle(struct opentmf_context* ctx, const char** id, struct opentmf_handle** handle)
{
  size_t driver_name_len = get_seperator_pos(*id, URL_SEPERATOR);
  char driver_name[ID_LEN_MAX] = "";

  if(driver_name_len >= ID_LEN_MAX)
    return OPENTMF_E_INVALID_URL;

  strncat(driver_name, *id, driver_name_len);

  // Check if driver is already open:
  struct opentmf_handle* driver_handle = NULL;
  struct driver* driver = ctx->drivers;

  while(driver)
  {
    if(strcmp(driver->name, driver_name) == 0)
    {
      driver->handle->ref_count++;
      driver_handle = driver->handle;
      break;
    }
    driver = driver->next;
  }

  if(!driver_handle) // Try to open.
  {
    const char* search_path = OPENTMF_DRIVER_PATH;
    const char* env_path = getenv("OPENTMF_DRIVER_PATH");
    char path[PATH_LEN_MAX] = "";

    do
    {
      size_t path_len = get_seperator_pos(search_path, SEARCH_PATH_SEPERATOR);

      if(path_len + 1 + driver_name_len + 3 < PATH_LEN_MAX)
      {
        strncat(path, search_path, path_len);
        if(path[path_len - 1] != '/')
          strcat(path, "/");
        strcat(path, driver_name);
        strcat(path, ".so");

        if(access(path, R_OK) != -1)
          break;

        path[0] = '\0';
      }

      search_path += path_len;
      if(search_path[0] == SEARCH_PATH_SEPERATOR)
        search_path++;

      if(search_path[0] == '\0' && env_path)
      {
        search_path = env_path;
        env_path = NULL;
        continue;
      }
    }
    while(search_path[0] != '\0');

    void* dl_handle;
    if(path[0] == '\0' || !(dl_handle = dlopen(path, RTLD_NOW)))
      return OPENTMF_E_INVALID_URL;

    int r = create_handle(ctx, &driver_handle, OPENTMF_HT_DRIVER);
    if(r != OPENTMF_SUCCESS)
    {
      dlclose(dl_handle);
      return r;
    }

    if((r = drv_open(driver_handle, dl_handle)) != OPENTMF_SUCCESS)
    {
      opentmf_close(driver_handle);
      return r;
    }

    driver = malloc(sizeof(*driver));
    if(!driver)
    {
      opentmf_close(driver_handle);
      return OPENTMF_E_NO_MEMORY;
    }

    // Add to linked list of open drivers:
    driver->name = malloc(driver_name_len + 1);
    strcpy(driver->name, driver_name);
    driver->handle = driver_handle;
    driver->next = ctx->drivers;
    ctx->drivers = driver;
  }

  *handle = driver_handle;
  *id += driver_name_len;

  return OPENTMF_SUCCESS;
}

size_t get_seperator_pos(const char* str, const char seperator)
{
  size_t i = 0;
  while(str[i] != seperator && str[i] != '\0')
    i++;
  return i;
}
