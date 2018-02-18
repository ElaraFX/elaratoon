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

#ifndef EI_LICENSE_H
#define EI_LICENSE_H

#include <ei_core.h>

/** Generate unique identifier of current machine, store 
 * the result in the given integer array.
 */
EI_API eiBool ei_generate_machine_id(eiUint id[5]);

/** Print unique identifier of current machine.
 */
EI_API eiBool ei_print_machine_id();

/** Generate unique identifier of currently connected dongle.
 */
EI_API eiBool ei_generate_dongle_id(eiUint id[5], eiInt64 *dongle_time);

/** Print unique identifier of currently connected dongle.
 */
EI_API eiBool ei_print_dongle_id();

/** Run current process as a license server for network 
 * floating license.
 */
EI_API eiBool ei_run_license_server(const char *uuid_str);

/** Set license server address for current rendering.
 */
EI_API void ei_set_license_server(const char *server_addr);

/** Set the connect timeout in milliseconds for render client.
 */
EI_API void ei_set_connect_timeout(eiUint milliseconds);

/** Log into live license account using universally unique 
 * identifier.
 */
EI_API void ei_login_with_uuid(const char *uuid_str);

/** Activate local machine license by code.
 * \param code The license code bound to local machine.
 */
EI_API void ei_local_license_set(const char *code);
/** Release current local machine license.
 */
EI_API void ei_local_license_release();
/** Generate unique identifier of current machine for local license.
 */
EI_API eiBool ei_local_license_generate_id(eiUint id[5]);
/** Print unique identifier of current machine for local license.
 */
EI_API eiBool ei_local_license_print_id();

#endif
