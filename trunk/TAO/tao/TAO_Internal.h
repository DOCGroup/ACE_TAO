// This may look like C, but it's really -*- C++ -*-
// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO
//
// = FILENAME
//    TAO_Internal.h
//
// = DESCRIPTION
//    Structures and methods completely internal to TAO.
//
// = AUTHOR
//    Chris Cleeland
//
// ============================================================================

#ifndef TAO_INTERNAL_H
#define TAO_INTERNAL_H

#include "tao/corbafwd.h"

class TAO_Export TAO_Internal
  // = TITLE
  //    This class encapsulates some private behaviors and global data
  //    structures used internal to TAO.  No application should EVER
  //    see this class.
{
public:
  static int open_services (int& argc, char** argv, int ignore_default_svc_conf = 0);
  // Initialize the ACE Service Configurator.  This is a one-shot
  // method, i.e., it can be called multiple times but it will only do
  // its work once.  It does, however, track the number of times it's
  // called (see <open_services>).  It is fully thread-safe.  Return 0
  // if successful, -1 with errno set if failure.  You can provide
  // your program a set of default svc.conf entries by setting
  // <ignore_default_svc_conf> to non-zero and use
  // default_svc_conf_entries() before calling open_services().

  static int close_services (void);
  // The complement to <open_services>, this will perform appropriate
  // ACE Service Configurator closure operations.  It should be called
  // as many times as <open_services>, and will only actually close
  // things down on the last call.  It is fully thread-safe.  Return 0
  // if successful, -1 with errno set if failure.

  static void default_svc_conf_entries (const char *resource_factory_args,
                                        const char *server_strategy_args,
                                        const char *client_strategy_args);
  // Set default svc.conf content.  This call has no effect if This
  // function must be called before first ORB initialization.

private:
  TAO_Internal (void);
  // Private CTOR prevents this class from being instantiated.

  static int service_open_count_;
  // Number of times <open_services> has been called.  Incremented by
  // <open_services>, and decremented by <close_services>.  Access to
  // this is protected via the <ACE_Static_Object_Lock>.

  static const char *resource_factory_args_;
  static const char *server_strategy_args_;
  static const char *client_strategy_args_;
};

#endif /* TAO_INTERNAL_H */
