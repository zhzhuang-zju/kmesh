/* Generated by the protocol buffer compiler.  DO NOT EDIT! */
/* Generated from: api/admin/config_dump.proto */

#ifndef PROTOBUF_C_api_2fadmin_2fconfig_5fdump_2eproto__INCLUDED
#define PROTOBUF_C_api_2fadmin_2fconfig_5fdump_2eproto__INCLUDED

#include <protobuf-c/protobuf-c.h>

PROTOBUF_C__BEGIN_DECLS

#if PROTOBUF_C_VERSION_NUMBER < 1003000
# error This file was generated by a newer version of protoc-c which is incompatible with your libprotobuf-c headers. Please update your headers.
#elif 1003002 < PROTOBUF_C_MIN_COMPILER_VERSION
# error This file was generated by an older version of protoc-c which is incompatible with your libprotobuf-c headers. Please regenerate this file with a newer version of protoc-c.
#endif

#include "listener/listener.pb-c.h"
#include "route/route.pb-c.h"
#include "cluster/cluster.pb-c.h"

typedef struct _Admin__ConfigDump Admin__ConfigDump;


/* --- enums --- */


/* --- messages --- */

struct  _Admin__ConfigDump
{
  ProtobufCMessage base;
  size_t n_listener_configs;
  Listener__Listener **listener_configs;
  size_t n_route_configs;
  Route__RouteConfiguration **route_configs;
  size_t n_cluster_configs;
  Cluster__Cluster **cluster_configs;
};
#define ADMIN__CONFIG_DUMP__INIT \
 { PROTOBUF_C_MESSAGE_INIT (&admin__config_dump__descriptor) \
    , 0,NULL, 0,NULL, 0,NULL }


/* Admin__ConfigDump methods */
void   admin__config_dump__init
                     (Admin__ConfigDump         *message);
size_t admin__config_dump__get_packed_size
                     (const Admin__ConfigDump   *message);
size_t admin__config_dump__pack
                     (const Admin__ConfigDump   *message,
                      uint8_t             *out);
size_t admin__config_dump__pack_to_buffer
                     (const Admin__ConfigDump   *message,
                      ProtobufCBuffer     *buffer);
Admin__ConfigDump *
       admin__config_dump__unpack
                     (ProtobufCAllocator  *allocator,
                      size_t               len,
                      const uint8_t       *data);
void   admin__config_dump__free_unpacked
                     (Admin__ConfigDump *message,
                      ProtobufCAllocator *allocator);
/* --- per-message closures --- */

typedef void (*Admin__ConfigDump_Closure)
                 (const Admin__ConfigDump *message,
                  void *closure_data);

/* --- services --- */


/* --- descriptors --- */

extern const ProtobufCMessageDescriptor admin__config_dump__descriptor;

PROTOBUF_C__END_DECLS


#endif  /* PROTOBUF_C_api_2fadmin_2fconfig_5fdump_2eproto__INCLUDED */
