// -*- C++ -*-

//=============================================================================
/**
 *  @file    TAO_Internal.h
 *
 *  Structures and methods completely internal to TAO.
 *
 *  @author Chris Cleeland
 */
//=============================================================================

#ifndef TAO_INTERNAL_H
#define TAO_INTERNAL_H

#include /**/ "ace/pre.h"

#include /**/ "tao/TAO_Export.h"
#include "tao/orbconf.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

ACE_BEGIN_VERSIONED_NAMESPACE_DECL
/// Forward declaration
class ACE_Service_Gestalt;
template <class X> class ACE_Intrusive_Auto_Ptr;
ACE_END_VERSIONED_NAMESPACE_DECL

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace TAO
{
  /**
   * @namespace ORB
   *
   * @brief This @c namespace encapsulates some behaviors and
   *        global data structures used by TAO.
   *        Only default_svc_conf_entries() is exported because it is
   *        documented as a way for applications to setup default svc.conf
   *        settings. The other operations are internal and
   *        application should NEVER use them which is why they are not
   *        exported.
   */
  namespace ORB
  {
    /**
     * If never done it before, extract ACE Service Configurator arguments from the given
     * argument vector, and initialize the _global_ configuration gestalt.
     * @retval 0 means OK,
     * @retval -1 spells major trouble
     */
    int open_global_services (int argc, ACE_TCHAR** argv);

    /**
     * Extract ACE Service Configurator arguments from the given
     * argument vector, and initialize the ACE Service Configurator.
     *
     * @note This method should be called before the ORB Core is
     *       initialized, and before any ORBInitializers are invoked.
     */
    int open_services (ACE_Intrusive_Auto_Ptr<ACE_Service_Gestalt> cfg, int& argc, ACE_TCHAR** argv);

    /**
     * The complement to @c open_services(), this will perform
     * appropriate ACE Service Configurator closure operations.  It
     * should be called as many times as @c open_services(), and will
     * only actually close things down on the last call.  It is fully
     * thread-safe.
     *
     * @retval 0 if successful
     * @retval -1 failure with @c errno set
     */
    int close_services (ACE_Intrusive_Auto_Ptr<ACE_Service_Gestalt> pcfg);

    /**
     * Set default @c `svc.conf' content, only non-null values are set.
     * This function must be called before first ORB initialization.
     * @arg rf_args Resource factory settings
     * @arg ssf_args Server strategy factory settings
     * @arg csf_args Client strategy factory settings
     */
    TAO_Export void default_svc_conf_entries (char const * rf_args,
                                              char const * ssf_args,
                                              char const * csf_args);
  }
}

TAO_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"

#endif  /* TAO_INTERNAL_H */
