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

#include "internal.h"

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
