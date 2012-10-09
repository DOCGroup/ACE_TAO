// -*- C++ -*-

// ===================================================================
/**
 *  @file   ORB_Core_TSS_Resources.h
 *
 *  $Id$
 *
 *  @author DOC Center - Washington University at St. Louis
 *  @author DOC Laboratory - University of California at Irvine
 */
// ===================================================================

#ifndef TAO_ORB_CORE_TSS_RESOURCES_H
#define TAO_ORB_CORE_TSS_RESOURCES_H

#include /**/ "ace/pre.h"

#include /**/ "tao/TAO_Export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/orbconf.h"
#include "ace/Array_Base.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

class TAO_ORB_Core;

/**
 * @class TAO_ORB_Core_TSS_Resources
 *
 * @brief The TSS resoures of an ORB core.
 *
 * This class is used by the ORB_Core to store the resources
 * potentially bound to a thread in TSS storage. The members are public
 * because only the ORB Core is expected to access them.
 */
class TAO_Export TAO_ORB_Core_TSS_Resources
{
public:

  /// Constructor
  TAO_ORB_Core_TSS_Resources (void);

  /// destructor
  ~TAO_ORB_Core_TSS_Resources (void);

  /// Cleans TSS resources.
  void fini (void);

private:

  /// The ORB Core TSS resources should not be copied
  TAO_ORB_Core_TSS_Resources (const TAO_ORB_Core_TSS_Resources&);
  void operator= (const TAO_ORB_Core_TSS_Resources&);

public:

  /**
   * @todo
   * The rest of the resources are not currently in use, just a plan
   * for the future...
   */
  /// Counter for how (nested) calls this thread has made to run the
  /// event loop.
  int event_loop_thread_;

  /// Counter for how many times this thread has become a client
  /// leader.
  int client_leader_thread_;

  /// Lane for this thread.
  void *lane_;

  /// Generic container for thread-specific objects.
  ACE_Array_Base<void *> ts_objects_;

  // Set to true by the wait_on_lf_no_nested_upcall wait strategy
  // @CJC@  maybe we should use allocate_tss_slot_id() instead?
  bool upcalls_temporarily_suspended_on_this_thread_;

  /// Pointer to the ORB core.  Needed to get access to the TSS
  /// cleanup functions for the TSS objects stored in the TSS object
  /// array in this class.
  TAO_ORB_Core *orb_core_;
};

TAO_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"
#endif /* TAO_ORB_CORE_H */
