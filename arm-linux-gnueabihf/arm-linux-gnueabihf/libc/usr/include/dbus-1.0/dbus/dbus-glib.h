/* -*- mode: C; c-file-style: "gnu" -*- */
/* dbus-glib.h GLib integration
 *
 * Copyright (C) 2002, 2003  CodeFactory AB
 * Copyright (C) 2003, 2004 Red Hat, Inc.
 *
 * Licensed under the Academic Free License version 2.1
 * 
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 *
 */
#ifndef DBUS_GLIB_H
#define DBUS_GLIB_H

#include <glib-object.h>
#include <dbus/dbus-shared.h>

G_BEGIN_DECLS

#define DBUS_INSIDE_DBUS_GLIB_H 1


/*
 * Convert to DBusConnection with dbus_g_connection_get_connection() in dbus-glib-lowlevel.h
 */
typedef struct _DBusGConnection DBusGConnection;
/*
 * Convert to DBusMessage with dbus_g_message_get_message() in dbus-glib-lowlevel.h
 */
typedef struct _DBusGMessage DBusGMessage;


#define DBUS_TYPE_G_CONNECTION   (dbus_g_connection_get_g_type ())
#define DBUS_TYPE_G_MESSAGE      (dbus_g_message_get_g_type ())
GType dbus_g_connection_get_g_type   (void) G_GNUC_CONST;
GType dbus_g_message_get_g_type      (void) G_GNUC_CONST;


DBusGConnection*  dbus_g_connection_ref          (DBusGConnection        *connection);
void              dbus_g_connection_unref        (DBusGConnection        *connection);
DBusGMessage*     dbus_g_message_ref             (DBusGMessage           *message);
void              dbus_g_message_unref           (DBusGMessage           *message);

void              dbus_g_connection_flush        (DBusGConnection        *connection);

GQuark dbus_g_error_quark (void);
#define DBUS_GERROR dbus_g_error_quark ()

typedef enum
{
DBUS_GERROR_FAILED,
DBUS_GERROR_NO_MEMORY,
DBUS_GERROR_SERVICE_UNKNOWN,
DBUS_GERROR_NAME_HAS_NO_OWNER,
DBUS_GERROR_NO_REPLY,
DBUS_GERROR_IO_ERROR,
DBUS_GERROR_BAD_ADDRESS,
DBUS_GERROR_NOT_SUPPORTED,
DBUS_GERROR_LIMITS_EXCEEDED,
DBUS_GERROR_ACCESS_DENIED,
DBUS_GERROR_AUTH_FAILED,
DBUS_GERROR_NO_SERVER,
DBUS_GERROR_TIMEOUT,
DBUS_GERROR_NO_NETWORK,
DBUS_GERROR_ADDRESS_IN_USE,
DBUS_GERROR_DISCONNECTED,
DBUS_GERROR_INVALID_ARGS,
DBUS_GERROR_FILE_NOT_FOUND,
DBUS_GERROR_FILE_EXISTS,
DBUS_GERROR_UNKNOWN_METHOD,
DBUS_GERROR_TIMED_OUT,
DBUS_GERROR_MATCH_RULE_NOT_FOUND,
DBUS_GERROR_MATCH_RULE_INVALID,
DBUS_GERROR_SPAWN_EXEC_FAILED,
DBUS_GERROR_SPAWN_FORK_FAILED,
DBUS_GERROR_SPAWN_CHILD_EXITED,
DBUS_GERROR_SPAWN_CHILD_SIGNALED,
DBUS_GERROR_SPAWN_FAILED,
DBUS_GERROR_UNIX_PROCESS_ID_UNKNOWN,
DBUS_GERROR_INVALID_SIGNATURE,
DBUS_GERROR_INVALID_FILE_CONTENT,
DBUS_GERROR_SELINUX_SECURITY_CONTEXT_UNKNOWN,
DBUS_GERROR_REMOTE_EXCEPTION
} DBusGError;

gboolean         dbus_g_error_has_name    (GError      *error,
					   const char  *name);
const char *     dbus_g_error_get_name    (GError      *error);

void             dbus_g_thread_init (void);

DBusGConnection* dbus_g_connection_open (const gchar  *address,
                                         GError      **error);
DBusGConnection* dbus_g_bus_get         (DBusBusType   type,
                                         GError      **error);
DBusGConnection* dbus_g_bus_get_private (DBusBusType   type,
                                         GMainContext *context,
                                         GError      **error);


typedef struct _DBusGObjectInfo DBusGObjectInfo;
typedef struct _DBusGMethodInfo DBusGMethodInfo;

/**
 * DBusGMethodInfo:
 * @function: C method to invoke
 * @marshaller: Marshaller to invoke method 
 * @data_offset: Offset into the introspection data
 *
 * Object typically generated by #dbus-binding-tool that
 * stores a mapping from introspection data to a
 * function pointer for a C method to be invoked.
 */
struct _DBusGMethodInfo
{
  GCallback                 function;    
  GClosureMarshal           marshaller;  
  int                       data_offset; 
};

/**
 * DBusGObjectInfo:
 * @format_version: Allows us to change the rest of this struct
 *   by adding DBusGObjectInfo2, DBusGObjectInfo3, etc.
 * @method_infos:  Array of method pointers 
 * @n_method_infos: Length of the infos array 
 * @data: Introspection data 
 * @exported_signals: Exported signals
 * @exported_properties: Exported properties 
 *
 * Introspection data for a #GObject, normally autogenerated by
 * a tool such as #dbus-binding-tool.
 */
struct _DBusGObjectInfo
{
  int   format_version;
                       
  const DBusGMethodInfo *method_infos;
  int   n_method_infos;                
  const char *data; 
  const char *exported_signals;  
  const char *exported_properties; 
};

void       dbus_glib_global_set_disable_legacy_property_access (void);

void       dbus_g_object_type_install_info     (GType                 object_type,
                                                const DBusGObjectInfo *info);

void       dbus_g_object_type_register_shadow_property (GType         iface_type,
                                                        const char    *dbus_prop_name,
                                                        const char    *shadow_prop_name);

void       dbus_g_error_domain_register        (GQuark                domain,
						const char *          default_iface,
						GType                 code_enum);

void       dbus_g_connection_register_g_object (DBusGConnection       *connection,
						const char            *at_path,
						GObject               *object);
void       dbus_g_connection_unregister_g_object (DBusGConnection *connection,
                                                  GObject *object);
GObject *  dbus_g_connection_lookup_g_object   (DBusGConnection       *connection,
						const char            *at_path);

#ifdef DBUS_COMPILATION
#include "dbus/dbus-gtype-specialized.h"
#else
#include <dbus/dbus-gtype-specialized.h>
#endif

/* definitions for some basic array types */
#define DBUS_TYPE_G_BOOLEAN_ARRAY  (dbus_g_type_get_collection ("GArray", G_TYPE_BOOLEAN))
#define DBUS_TYPE_G_UCHAR_ARRAY    (dbus_g_type_get_collection ("GArray", G_TYPE_UCHAR))
#define DBUS_TYPE_G_UINT_ARRAY     (dbus_g_type_get_collection ("GArray", G_TYPE_UINT))
#define DBUS_TYPE_G_INT_ARRAY      (dbus_g_type_get_collection ("GArray", G_TYPE_INT))
#define DBUS_TYPE_G_UINT64_ARRAY   (dbus_g_type_get_collection ("GArray", G_TYPE_UINT64))
#define DBUS_TYPE_G_INT64_ARRAY    (dbus_g_type_get_collection ("GArray", G_TYPE_INT64))
#define DBUS_TYPE_G_OBJECT_ARRAY   (dbus_g_type_get_collection ("GPtrArray", G_TYPE_OBJECT))

#define DBUS_TYPE_G_STRING_STRING_HASHTABLE (dbus_g_type_get_map ("GHashTable", G_TYPE_STRING, G_TYPE_STRING))

typedef gchar DBusGObjectPath;
GType        dbus_g_object_path_get_g_type         (void) G_GNUC_CONST;
#define DBUS_TYPE_G_OBJECT_PATH (dbus_g_object_path_get_g_type ())

typedef gchar DBusGSignature;
GType        dbus_g_signature_get_g_type           (void) G_GNUC_CONST;
#define DBUS_TYPE_G_SIGNATURE (dbus_g_signature_get_g_type ())

void         dbus_g_object_register_marshaller      (GClosureMarshal  marshaller,
						     GType            rettype,
						     ...);
void         dbus_g_object_register_marshaller_array(GClosureMarshal  marshaller,
						     GType            rettype,
						     guint            n_types,
						     const GType*     types);

typedef struct _DBusGProxy       DBusGProxy;
typedef struct _DBusGProxyClass  DBusGProxyClass;

#define DBUS_TYPE_G_PROXY              (dbus_g_proxy_get_type ())
#define DBUS_G_PROXY(object)           (G_TYPE_CHECK_INSTANCE_CAST ((object), DBUS_TYPE_G_PROXY, DBusGProxy))
#define DBUS_G_PROXY_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), DBUS_TYPE_G_PROXY, DBusGProxyClass))
#define DBUS_IS_G_PROXY(object)        (G_TYPE_CHECK_INSTANCE_TYPE ((object), DBUS_TYPE_G_PROXY))
#define DBUS_IS_G_PROXY_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), DBUS_TYPE_G_PROXY))
#define DBUS_G_PROXY_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), DBUS_TYPE_G_PROXY, DBusGProxyClass))

struct _DBusGProxy
{
  /*< private >*/
  GObject parent;
};

struct _DBusGProxyClass
{
  /*< private >*/
  GObjectClass parent_class;  /**< Parent class */
};

typedef struct _DBusGProxyCall DBusGProxyCall;
typedef void (* DBusGProxyCallNotify) (DBusGProxy       *proxy,
				       DBusGProxyCall   *call_id,
				       void             *user_data);

GType             dbus_g_proxy_get_type              (void) G_GNUC_CONST;
DBusGProxy*       dbus_g_proxy_new_for_name          (DBusGConnection   *connection,
                                                      const char        *name,
                                                      const char        *path,
                                                      const char        *iface);
DBusGProxy*       dbus_g_proxy_new_for_name_owner    (DBusGConnection   *connection,
                                                      const char        *name,
                                                      const char        *path,
                                                      const char        *iface,
                                                      GError           **error);
DBusGProxy*       dbus_g_proxy_new_from_proxy        (DBusGProxy        *proxy,
                                                      const char        *iface,
                                                      const char        *path);
DBusGProxy*       dbus_g_proxy_new_for_peer          (DBusGConnection   *connection,
                                                      const char        *path,
                                                      const char        *iface);

void              dbus_g_proxy_set_interface         (DBusGProxy        *proxy,
						      const char        *interface_name);
void              dbus_g_proxy_add_signal            (DBusGProxy        *proxy,
						      const char        *signal_name,
						      GType              first_type, 
						      ...);

void              dbus_g_proxy_connect_signal        (DBusGProxy        *proxy,
                                                      const char        *signal_name,
                                                      GCallback          handler,
                                                      void              *data,
                                                      GClosureNotify     free_data_func);
void              dbus_g_proxy_disconnect_signal     (DBusGProxy        *proxy,
                                                      const char        *signal_name,
                                                      GCallback          handler,
                                                      void              *data);

gboolean          dbus_g_proxy_call                  (DBusGProxy        *proxy,
						      const char        *method,
						      GError           **error,
						      GType              first_arg_type,
						      ...);

gboolean          dbus_g_proxy_call_with_timeout     (DBusGProxy        *proxy,
				                      const char       *method,
	                   			      int timeout,
                   				      GError           **error,
                   				      GType              first_arg_type,
				                      ...);

void              dbus_g_proxy_call_no_reply         (DBusGProxy        *proxy,
                                                      const char        *method,
                                                      GType              first_arg_type,
                                                      ...);

DBusGProxyCall *  dbus_g_proxy_begin_call            (DBusGProxy        *proxy,
                                                      const char        *method,
						      DBusGProxyCallNotify notify,
						      gpointer           user_data,
						      GDestroyNotify     destroy,
                                                      GType              first_arg_type,
                                                      ...);
DBusGProxyCall * dbus_g_proxy_begin_call_with_timeout (DBusGProxy        *proxy,
		                                       const char       *method,
		                                       DBusGProxyCallNotify notify,
                                                       gpointer       user_data,
                                                       GDestroyNotify   destroy,
                                                       int timeout,
                                                       GType             first_arg_type,
				                       ...);

void              dbus_g_proxy_set_default_timeout   (DBusGProxy        *proxy,
                                                      int                timeout);

gboolean          dbus_g_proxy_end_call              (DBusGProxy        *proxy,
                                                      DBusGProxyCall    *call,
                                                      GError           **error,
                                                      GType              first_arg_type,
                                                      ...);
void              dbus_g_proxy_cancel_call           (DBusGProxy        *proxy,
                                                      DBusGProxyCall    *call);

const char*       dbus_g_proxy_get_path              (DBusGProxy        *proxy);

const char*       dbus_g_proxy_get_bus_name          (DBusGProxy        *proxy);

const char*       dbus_g_proxy_get_interface         (DBusGProxy        *proxy);

typedef struct _DBusGMethodInvocation DBusGMethodInvocation;

void              dbus_g_method_return               (DBusGMethodInvocation *context, ...);

void              dbus_g_method_return_error         (DBusGMethodInvocation *context, const GError *error);

/* Probably possible to replace this with a closure */
typedef struct {
  GCallback cb;
  gpointer userdata;
} DBusGAsyncData;

#undef DBUS_INSIDE_DBUS_GLIB_H

#include <dbus/dbus-gvalue-parse-variant.h>

G_END_DECLS

#endif /* DBUS_GLIB_H */
