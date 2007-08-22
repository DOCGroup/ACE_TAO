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

#include "tao/orbconf.h"
#include /**/ "tao/Versioned_Namespace.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

ACE_BEGIN_VERSIONED_NAMESPACE_DECL
/// Forward declaration
class ACE_Service_Gestalt;
ACE_END_VERSIONED_NAMESPACE_DECL

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

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
    /**
     * Extract ACE Service Configurator arguments from the given
     * argument vector, and initialize the ACE Service Configurator.
     *
     * @note This method should be called before the ORB Core is
     *       initialized, and before any ORBInitializers are invoked.
     */
    int open_services (ACE_Service_Gestalt* cfg, int& argc, ACE_TCHAR** argv);

    /**
     * The complement to @c open_services(), this will perform
     * appropriate ACE Service Configurator closure operations.  It
     * should be called as many times as @c open_services(), and will
     * only actually close things down on the last call.  It is fully
     * thread-safe.
     *
     * @return @c 0 if successful, @c -1 with @c errno set if
     *         failure.
     */
    int close_services (ACE_Service_Gestalt* pcfg);

    /**
     * Set default @c `svc.conf' content.
     * This function must be called before first ORB initialization.
     */
    void default_svc_conf_entries (char const * rf_args,
                                   char const * ssf_args,
                                   char const * csf_args);

  }
}
TAO_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"

#endif  /* TAO_INTERNAL_H */
