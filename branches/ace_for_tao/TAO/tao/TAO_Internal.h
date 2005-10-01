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

#include /**/ "ace/pre.h"

#include "ace/config-all.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */


namespace TAO
{
  /**
   * @namespace ORB
   *
   * @brief This @c namespace encapsulates some private behaviors and
   *        global data structures used internal to TAO.  No
   *        application should EVER see them which is why they are not
   *        exported.
   */
  namespace ORB
  {
    /// Extract ACE Service Configurator arguments from the given
    /// argument vector, and initialize the ACE Service Configurator.
    /**
     * @note This method should be called before the ORB Core is
     *       initialized, and before any ORBInitializers are invoked.
     */
    int open_services (int& argc, ACE_TCHAR** argv);

    /**
     * The complement to @c open_services(), this will perform
     * appropriate ACE Service Configurator closure operations.  It
     * should be called as many times as @c open_services(), and will
     * only actually close things down on the last call.  It is fully
     * thread-safe.
     * @return @c 0 if successful, @c -1 with @c errno set if
     *         failure.
     */
    int close_services (void);

    /// Set default @c `svc.conf' content.  This call has no effect if
    /// This function must be called before first ORB initialization.
    void default_svc_conf_entries (char const * rf_args,
                                   char const * ssf_args,
                                   char const * csf_args);

  }
}

#include /**/ "ace/post.h"

#endif  /* TAO_INTERNAL_H */
