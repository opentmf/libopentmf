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

#ifndef _OPENTMF_COMMON_H_
#define _OPENTMF_COMMON_H_

#include <stdint.h>

#if defined(OPENTMF_LIBRARY) || defined(OPENTMF_DRIVER)
  #if defined(_WIN32) || defined(__CYGWIN__)
    #ifdef __GNUC__
      #define DLL_PUBLIC __attribute__ ((dllexport))
    #else
      #define DLL_PUBLIC __declspec(dllexport)
    #endif
  #elif __GNUC__ >= 4
    #define OPENTMF_API __attribute__ ((visibility ("default")))
  #else
    #define OPENTMF_API
  #endif
#else
  #define OPENTMF_API
#endif

enum opentmf_status
{
  OPENTMF_SUCCESS = 0,
  OPENTMF_E_NO_MEMORY = -1,
  OPENTMF_E_NOT_SUPPORTED = -2,
  OPENTMF_E_FAILED = -3,
  OPENTMF_E_INVALID_PARAM = -4,
  OPENTMF_E_INVALID_URL = -5,
};

enum opentmf_handle_type
{
  OPENTMF_HT_INVALID = 0,
  OPENTMF_HT_DRIVER = 1,
};

struct opentmf_version
{
  uint16_t major;
  uint16_t minor;
  uint16_t patch;
  const char* extra;
};

typedef uint8_t opentmf_bool;

#define OPENTMF_FALSE 0
#define OPENTMF_TRUE 1

#ifndef OPENTMF_DRIVER
struct opentmf_context;
struct opentmf_handle;
#endif

#endif
