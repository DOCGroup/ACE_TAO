/* -*- C++ -*- */
//=============================================================================
/**
 *  @file    Local_ESTypes.h
 *
 *  $Id$
 *
 *  @author Tim Harrison (harrison@cs.wustl.edu)
 *
 *  Manual types that would otherwise be defined/implemented via an
 *  IDL compiler.
 *  NOTE: the file is obsolecent, we have TAO now, but we keep it
 *  to speed up the porting.
 *
 *
 */
//=============================================================================


#ifndef ACE_LOCAL_ESTYPES_H
#define ACE_LOCAL_ESTYPES_H
#include /**/ "ace/pre.h"

#include "tao/corba.h"

#include "orbsvcs/Event_Service_Constants.h"

#include "orbsvcs/CosNamingC.h"
#include "orbsvcs/RtecSchedulerC.h"
#include "orbsvcs/RtecSchedulerS.h"
#include "orbsvcs/RtecEventCommC.h"
#include "orbsvcs/RtecEventCommS.h"
#include "orbsvcs/RtecEventChannelAdminC.h"
#include "orbsvcs/RtecEventChannelAdminS.h"

#define ACE_DEFAULT_EVENT_CHANNEL_TYPE 0

// These are to help MSVC++ 4.2 deal with inheritence of nested types.
// Not needed for Sun C++ or MSVC++ 5.0.

typedef RtecEventComm::Event RtecEventComm_Event;

#if defined (__ACE_INLINE__)
#include "Local_ESTypes.i"
#endif /* __ACE_INLINE__ */

#include /**/ "ace/post.h"
#endif /* ACE_LOCAL_ESTYPES_H */
