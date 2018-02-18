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

#ifndef EI_CONFIG_H
#define EI_CONFIG_H

/** Parser for loading host configurations.
 * \file ei_config.h
 * \author Elvic Liang
 */

#include <ei_core.h>
#include <ei_array.h>
#include <ei_platform.h>

typedef struct eiSearchPath {
	char		path[ EI_MAX_FILE_NAME_LEN ];
} eiSearchPath;

/** The host configurations.
 */
typedef struct eiConfig {
	eiInt			nthreads;
	eiInt			texture_memlimit;
	eiInt			texture_openfiles;
	eiBool			low_thread_priority;
	eiBool			shader_profiling;
	eiInt			shader_optlevel;
	eiInt			shader_optpasses;
	ei_array		shader_searchpaths;		/**< shader search paths */
	ei_array		texture_searchpaths;	/**< texture search paths */
	ei_array		scene_searchpaths;		/**< scene search paths */
	ei_array		license_searchpaths;	/**< license search paths */
	eiSearchPath	license_server;			/**< license server name */
	eiSearchPath	log_file;				/**< log file name */
	eiSearchPath	page_file_dir;
} eiConfig;

/** Set configurations to default values.
 */
EI_API void ei_config_init(eiConfig *config);

/** Cleanup configurations.
 */
EI_API void ei_config_exit(eiConfig *config);

/** Load configurations from file.
 */
EI_API eiBool ei_config_load(eiConfig *config, const char *filename);

#endif
