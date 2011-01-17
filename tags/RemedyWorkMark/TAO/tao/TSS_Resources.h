// -*- C++ -*-

// ===================================================================
/**
 *  @file   TSS_Resources.h
 *
 *  $Id$
 *
 *  @author DOC Group - ISIS at Vanderbilt University
 *  @author DOC Center - Washington University at St. Louis
 *  @author DOC Laboratory - University of California at Irvine
 */
// ===================================================================

#ifndef TAO_TAO_TSS_RESOURCES_H
#define TAO_TAO_TSS_RESOURCES_H

#include /**/ "ace/pre.h"

#include /**/ "tao/TAO_Export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/Environment.h"

#include "tao/Policy_Current_Impl.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

// Forward declarations
namespace TAO
{
  class GUIResource_Factory;
  class Transport_Selection_Guard;
}
/**
 * @class TAO_TSS_Resources
 *
 * @brief The TSS resoures shared by all the ORBs
 *
 * This class is used by TAO to store the resources that are
 * thread-specific but are *not* ORB specific. The members are public
 * because only the ORB Core is expected to access them.
 */
class TAO_Export TAO_TSS_Resources
{
public:

  /// Constructor
  TAO_TSS_Resources (void);

  /// Destructor
  ~TAO_TSS_Resources (void);

  /// Return a singleton instance of this class.
  static TAO_TSS_Resources * instance (void);

private:

  /// Do not copy TSS resources
  //@{
  TAO_TSS_Resources (const TAO_TSS_Resources&);
  void operator= (const TAO_TSS_Resources&);
  //@}

public:

  /**
   * Points to structure containing state for the current upcall
   * context in this thread.  Note that it does not come from the
   * resource factory because it must always be held in
   * thread-specific storage.  For now, since TAO_ORB_Core instances
   * are TSS singletons, we simply ride along and don't allocate
   * occupy another TSS slot since there are some platforms where
   * those are precious commodities (e.g., NT).
   */
  void * poa_current_impl_;

  void * rtscheduler_current_impl_;

  void * rtscheduler_previous_current_impl_;

  /// The CORBA priority of the thread if set via RTCORBA::Current.
  /// RTCORBA::minPriority - 1 otherwise.
  CORBA::Short rtcorba_current_priority_;

  /// The default environment for the thread.
  CORBA::Environment * default_environment_;

  /// If the user (or library) provides no environment the ORB_Core
  /// still holds one.
  CORBA::Environment tss_environment_;

#if (TAO_HAS_CORBA_MESSAGING == 1)

  /// The initial PolicyCurrent for this thread. Should be a TSS
  /// resource.
  TAO_Policy_Current_Impl initial_policy_current_;

  /// This pointer is reset by the POA on each upcall.
  TAO_Policy_Current_Impl * policy_current_;

#endif /* TAO_HAS_CORBA_MESSAGING == 1 */

   /// A factory for creating GUIReactors.
  /**
   * @c GUIResource_Factory has to be stored in TSS, as GUIReactors
   * are operational only in within the context of GUI event loops.
   */
  TAO::GUIResource_Factory * gui_resource_factory_;

#if TAO_HAS_TRANSPORT_CURRENT == 1

  /// A TSS for a pointer to the current transport guard (see
  /// below). The guard keeps track of the Transport, if any that has
  /// been selected for use by the current thread, in the context of
  /// an upcall or client-side interceptor.

  TAO::Transport_Selection_Guard* tsg_;

#endif  /* TAO_HAS_TRANSPORT_CURRENT == 1 */
};

TAO_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"

#endif /* TAO_TSS_RESOURCES_H */
