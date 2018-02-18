/**************************************************************************
 * Copyright (C) 2015 Rendease Co., Ltd.
 * All rights reserved.
 *
 * This program is commercial software: you must not redistribute it 
 * and/or modify it without written permission from Rendease Co., Ltd.
 *
 * This program is distributed in the hope that it will be useful, 
 * but WITHOUT ANY WARRANTY; without even the implied warranty of 
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the 
 * End User License Agreement for more details.
 *
 * You should have received a copy of the End User License Agreement along 
 * with this program.  If not, see <http://www.rendease.com/licensing/>
 *************************************************************************/

#ifndef EI_VERBOSE_H
#define EI_VERBOSE_H

/** Routines for outputing messages based on verbosity levels.
 * \file ei_verbose.h
 */

#include <ei_core.h>

/** Verbosity levels, the default value is EI_VERBOSE_WARNING.
 */
enum {
	EI_VERBOSE_NONE = 0, 
	EI_VERBOSE_FATAL, 
	EI_VERBOSE_ERROR, 
	EI_VERBOSE_WARNING, 
	EI_VERBOSE_INFO, 
	EI_VERBOSE_DEBUG, 
	EI_VERBOSE_ALL, 
};

/** Initialize verbosity.
 */
EI_API void ei_verbose_init();

/** Exit verbosity.
 */
EI_API void ei_verbose_exit();

EI_API void ei_fatal(const char* format, ...);
EI_API void ei_error(const char* format, ...);
EI_API void ei_warning(const char* format, ...);
EI_API void ei_info(const char* format, ...);
EI_API void ei_debug(const char* format, ...);

/** Start logging messages to a specified text file.
 */
EI_API void ei_logging(const char *filename);

/** Stop logging messages to a specified text file.
 */
EI_API void ei_end_logging();

/** Set current verbose level.
 */
EI_API void ei_verbose_set(const eiInt level);

/** Get current verbose level.
 */
EI_API eiInt ei_verbose_get();

/** The callback which can be specified by user to receive 
 * messages in verbose system.
 * \param level One value in the verbosity level enum.
 * \param format The message format.
 * \param args The varying argument list of the message.
 */
typedef void (*eiVerboseCallback)(eiInt level, const char *format, va_list args);

/** Get current verbose callback.
 */
EI_API eiVerboseCallback ei_verbose_get_callback();
/** Set current verbose callback.
 */
EI_API void ei_verbose_set_callback(eiVerboseCallback cb);

#endif
