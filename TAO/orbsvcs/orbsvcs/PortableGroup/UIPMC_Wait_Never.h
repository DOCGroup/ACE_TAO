// This may look like C, but it's really -*- C++ -*-

//=============================================================================
/**
 *  @file    UIPMC_Wait_Never.h
 *
 *  $Id$
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

#include "portablegroup_export.h"

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
  /// Constructor.
  TAO_UIPMC_Wait_Never (TAO_Transport *transport);

  /// Destructor.
  virtual ~TAO_UIPMC_Wait_Never (void);

  // = Documented in TAO_Wait_Strategy.
  virtual int sending_request (TAO_ORB_Core *orb_core,
                               int two_way);
  virtual int wait (ACE_Time_Value *max_wait_time,
                    TAO_Synch_Reply_Dispatcher &rd);
  virtual int register_handler (void);
  virtual int non_blocking (void);
};

#include /**/ "ace/post.h"
#endif /* TAO_UIPMC_WAIT_NEVER_H */
