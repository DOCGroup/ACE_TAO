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
  static int open_services (int& argc, char** argv);
  // Initialize the ACE Service Configurator.  This is a one-shot
  // method, i.e., it can be called multiple times but it will only do
  // its work once.  It does, however, track the number of times it's
  // called (see <open_services>).  It is fully thread-safe.  Return 0
  // if successful, -1 with errno set if failure.

  static int close_services (void);
  // The complement to <open_services>, this will perform appropriate
  // ACE Service Configurator closure operations.  It should be called
  // as many times as <open_services>, and will only actually close
  // things down on the last call.  It is fully thread-safe.  Return 0
  // if successful, -1 with errno set if failure.

  static void _svc_conf (const char *resource_factory_args,
                         const char *server_strategy_args,
                         const char *client_strategy_args);
  // Set fake svc.conf content.  This call has no effect if
  // TAO_PLATFORM_SVC_CONF_FILE_NOTSUP is not defined.  This function
  // must be called before first ORB initialization.

private:
  TAO_Internal (void);
  // Private CTOR prevents this class from being instantiated.

  static int service_open_count_;
  // Number of times <open_services> has been called.  Incremented by
  // <open_services>, and decremented by <close_services>.  Access to
  // this is protected via the <ACE_Static_Object_Lock>.

#if defined (TAO_PLATFORM_SVC_CONF_FILE_NOTSUP)
  static char *resource_factory_args_;
  static char *server_strategy_args_;
  static char *client_strategy_args_;
#endif /* TAO_PLATFORM_SVC_CONF_FILE_NOTSUP */
};

#endif /* TAO_INTERNAL_H */
