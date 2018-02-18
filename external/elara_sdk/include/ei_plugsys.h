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

#ifndef EI_PLUGSYS_H
#define EI_PLUGSYS_H

/** A generic plugin management system which manages the dynamic 
 * loading and unloading of modules and plugins. this system is 
 * designed to be thread-safe, no extra locking is needed.
 * \file ei_plugsys.h
 * \author Elvic Liang
 */

#include <ei_core.h>
#include <ei_platform.h>
#include <ei_btree.h>
#include <ei_array.h>

/** A generic module representation. */
typedef struct eiModule			eiModule;
/** A generic plugin representation. */
typedef struct eiPlugin			eiPlugin;
/** The object instance created by a plugin. */
typedef struct eiPluginObject	eiPluginObject;
/** A generic plugin management system. */
typedef struct eiPluginSystem	eiPluginSystem;

/** The callback to delete the plugin object itself. */
typedef void (*ei_plugin_object_deletethis)(eiPluginObject *object);

/** The base class of plugin object.
 * all objects created by plugins should 
 * derive from this class.
 */
struct eiPluginObject {
	eiPlugin						*plugin;	/**< the plugin that created this object */
	ei_plugin_object_deletethis		deletethis;	/**< this function pointer must be filled by 
												     derived classes to delete the plugin 
												     object itself */
};

/** Initialize the base plugin object.
 */
EI_API void ei_plugin_object_init(eiPluginObject *object);
/** Cleanup the base plugin object.
 */
EI_API void ei_plugin_object_exit(eiPluginObject *object);

/** Get plugin system for the plugin.
 */
EI_API eiPluginSystem *ei_plugin_get_plugin_system(eiPlugin *plugin);

/** Generic plug-in management system */
struct eiPluginSystem {
	ei_btree				m_module_map;		/**< a module map containing all modules managed by 
												     this plugin system */
	ei_btree				m_plugin_map;		/**< a plugin map containing all plugins managed by 
												     this plugin system */
	ei_array				m_search_pathes;	/**< the array of search pathes */
	eiLock					*m_lock;			/**< the lock for performing plugin management operations */
};

/** Initialize a plugin system.
 */
EI_API void ei_plugsys_init(eiPluginSystem *plugsys);

/** Cleanup a plugin system.
 */
EI_API void ei_plugsys_exit(eiPluginSystem *plugsys);

/** Add a search path to the plugin system for 
 * searching modules.
 */
EI_API eiBool ei_plugsys_add_search_path(
	eiPluginSystem *plugsys, 
	const char *search_path);

/** Link a module by name. if the module has been loaded, 
 * no extra loading will be performed. add reference to the 
 * module. you can explicitly call this function to link a module.
 */
EI_API eiModule *ei_plugsys_link(
	eiPluginSystem *plugsys, 
	const char *filename);

/** Create a plugin object by plugin name.
 * \param name The name of the plugin. it can be in format 
 * module_name.plugin_name, so the system will try to search 
 * in module module_name, or it can just be in format 
 * plugin_name, then the system will try to search in all 
 * modules to match plugin_name. if the corresponding module 
 * is not loaded, it will be automatically loaded.
 * \param param The custom parameters which will be passed to 
 * the create function in plugin.
 */
EI_API eiPluginObject *ei_plugsys_create(
	eiPluginSystem *plugsys, 
	const char *name, 
	void *param);

/** Declare a plugin object by plugin name.
 * \param name The name of the plugin. it can be in format 
 * module_name.plugin_name, so the system will try to search 
 * in module module_name, or it can just be in format 
 * plugin_name, then the system will try to search in all 
 * modules to match plugin_name. if the corresponding module 
 * is not loaded, it will be automatically loaded.
 * \param param The custom parameters which will be passed to 
 * the declare function in plugin.
 */
EI_API eiBool ei_plugsys_declare(
	eiPluginSystem *plugsys, 
	const char *name, 
	void *param);

/** Delete a plugin object from the system.
 */
EI_API eiBool ei_plugsys_delete(
	eiPluginSystem *plugsys, 
	eiPluginObject *object);

#endif
