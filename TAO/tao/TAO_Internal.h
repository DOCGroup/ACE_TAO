// -*- C++ -*-

//=============================================================================
/**
 *  @file    TAO_Internal.h
 *
 *  $Id$
 *
 *  Structures and methods completely internal to TAO.
 *
 *  @author Chris Cleeland
 */
//=============================================================================


#ifndef TAO_INTERNAL_H
#define TAO_INTERNAL_H

#include "ace/pre.h"

#include "TAO_Export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

/**
 * @class TAO_Internal
 *
 * @brief This class encapsulates some private behaviors and global data
 * structures used internal to TAO.  No application should EVER
 * see this class.
 */
class TAO_Export TAO_Internal
{
public:

  /// Extract ACE Service Configurator arguments from the given
  /// argument vector, and initialize the ACE Service Configurator.
  /// This method should be called before the ORB Core is initialized,
  /// and before any ORBInitializers are invoked.
  static int open_services (int &argc, ACE_TCHAR **argv);

  /**
   * The complement to open_services(), this will perform appropriate
   * ACE Service Configurator closure operations.  It should be called
   * as many times as open_services(), and will only actually close
   * things down on the last call.  It is fully thread-safe.  Return 0
   * if successful, -1 with errno set if failure.
   */
  static int close_services (void);

  /// Set default `svc.conf' content.  This call has no effect if This
  /// function must be called before first ORB initialization.
  static void default_svc_conf_entries (const char *resource_factory_args,
                                        const char *server_strategy_factory_args,
                                        const char *client_strategy_factory_args);

private:

  /// Private constructor prevents this class from being
  /// instantiated.
  TAO_Internal (void);

  /**
   * Initialize the ACE Service Configurator.  This is a one-shot
   * method, i.e., it can be called multiple times but it will only do
   * its work once.  It does, however, track the number of times it's
   * called (see open_services()).  It is fully thread-safe.  Return 0
   * if successful, -1 with errno set if failure.  You can provide
   * your program a set of default `svc.conf' entries by setting
   * @a ignore_default_svc_conf_file to non-zero and use
   * default_svc_conf_entries() before calling open_services().
   * In addition, you can @a skip_service_config_open altogether,
   * which is important if the ORB is linked in via the \param
   * ACE_Service_Configurator, which is non-reentrant.
   */
  static int open_services_i (int &argc,
                              char **argv,
                              int ignore_default_svc_conf_file = 0,
                              int skip_service_config_open = 0);

private:


  /// Number of times open_services() has been called.  Incremented by
  /// open_services(), and decremented by close_services().  Access to
  /// this is protected via the \param ACE_Static_Object_Lock.
  static int service_open_count_;


  static const char *resource_factory_args_;
  static const char *server_strategy_factory_args_;
  static const char *client_strategy_factory_args_;
};

#include "ace/post.h"

#endif  /* TAO_INTERNAL_H */
