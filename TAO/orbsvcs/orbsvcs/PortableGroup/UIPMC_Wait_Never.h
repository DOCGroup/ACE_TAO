// -*- C++ -*-

//=============================================================================
/**
 *  @file    UIPMC_Wait_Never.h
 *
 *  @author  Frank Hunleth <fhunleth@cs.wustl.edu>
 */
//=============================================================================

#ifndef TAO_UIPMC_WAIT_NEVER_H
#define TAO_UIPMC_WAIT_NEVER_H
#include /**/ "ace/pre.h"

#include "tao/Wait_Strategy.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "orbsvcs/PortableGroup/portablegroup_export.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

/**
 * @class TAO_UIPMC_Wait_Never
 *
 * @brief Since two-ways are not supported by UIPMC and more
 * generally, anything that requires waiting isn't supported, so
 * return error if anyone tries to do this.
 */
class TAO_PortableGroup_Export TAO_UIPMC_Wait_Never : public TAO_Wait_Strategy
{
public:
  TAO_UIPMC_Wait_Never (TAO_Transport *transport);
  ~TAO_UIPMC_Wait_Never () override = default;

  // = Documented in TAO_Wait_Strategy.
  int sending_request (TAO_ORB_Core *orb_core,
                       TAO_Message_Semantics msg_semantics) override;
  int wait (ACE_Time_Value *max_wait_time,
            TAO_Synch_Reply_Dispatcher &rd) override;
  int register_handler () override;
  bool non_blocking () const override;
  bool can_process_upcalls () const override;
};

TAO_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"
#endif /* TAO_UIPMC_WAIT_NEVER_H */
