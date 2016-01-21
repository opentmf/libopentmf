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

#ifndef _UTILS_H_
#define _UTILS_H_

#define LOAD_MAN_FUNC(x) if(!(LOAD_OPT_FUNC(x))) { return OPENTMF_E_FAILED; }
#define LOAD_OPT_FUNC(x) HANDLE_DATA->x = dlsym(driver, "opentmf_" #x)

#endif
