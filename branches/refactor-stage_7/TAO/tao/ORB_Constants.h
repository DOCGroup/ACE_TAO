// -*- C++ -*-

//=============================================================================
/**
 *  @file     ORB_Constants.h
 *
 *  $Id$
 *
 *  Constants needed by various files.
 *
 *  @author  Jeff Parsons
 */
//=============================================================================

#ifndef TAO_ORB_CONSTANTS_H
#define TAO_ORB_CONSTANTS_H

#include /**/ "ace/pre.h"

#include "tao/Basic_Types.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/TAO_Export.h"

// An hash define for the regular two way operation
#define TAO_TWOWAY_RESPONSE_FLAG 255

namespace CORBA
{
  /// The OMG Vendor Minor Codeset ID.
  /**
   * This number is reserved by the OMG as a prefix to all the
   * standard system exception minor codes.  Check the CORBA/IIOP
   * specification for details.
   */
  const ULong OMGVMCID = 0x4f4d0000U;
}

// This number was assigned by the OMG.  Do *NOT* change at random.
// The ASCII representation is TA0xxxx, close enough since they only
// take 20 bits, the first 16 are TA, the next 4 are 0000.  Remember
// that we can only play with the last 12 bits, TAO_MAX_MINOR_CODE is
// there to remind us of that.
const CORBA::ULong TAO_DEFAULT_MINOR_CODE = 0x54410000U;
const CORBA::ULong TAO_MAX_MINOR_CODE = 0x54410fffU;

/// A dummy service context that is inserted in the service context
/// list to preserve the alignment in DSI based gateways, so no
/// marshaling/demarshaling is required.
/**
 * @note This is *extremely* brittle but works.
 */
const CORBA::ULong TAO_SVC_CONTEXT_ALIGN = 0x54414f00U;

/**
 * @name TAO-Specific Profile IDs
 *
 * The TAO @c IOP::ProfileId range 0x54414f00 - 0x54414f0f has been
 * reserved with the OMG.
 *
 * @note It is *NOT* necessary to list your own protocols here.
 *
 * @note The values below are subject to change at any point.
 */
//@{
/// Local IPC (Unix Domain)
const CORBA::ULong TAO_TAG_UIOP_PROFILE = 0x54414f00U;

/// ATM/AAL5
// const CORBA::ULong TAO_TAG_AIOP_PROFILE = 0x54414f01U;

/// Shared memory
const CORBA::ULong TAO_TAG_SHMEM_PROFILE = 0x54414f02U;

/// Message Queue
// const CORBA::ULong TAO_TAG_MSGQ_PROFILE = 0x54414f03U;

/// UDP
const CORBA::ULong TAO_TAG_UDP_PROFILE = 0x54414f04U;

/// Unreliable IP Multicast
const CORBA::ULong TAO_TAG_UIPMC_PROFILE = 0x54414f0CU;

/**
 * @name Minor Code Encoding
 *
 * Encode the location in 5 bits, and the errno in 7 bits:
 *
 * @verbatim
 * 0x   0101 0100   0100 0001   0000   ____ _     ___ ____
 *          T           A        0    location      errno
 * @endverbatim
 **/
//@{
/**
 * @name Location Encoding
 *
 * The location encoding is the 5 bits, after the @ errno encoding.
 */
//@{
const CORBA::ULong TAO_INVOCATION_LOCATION_FORWARD_MINOR_CODE = (0x01U << 7);
const CORBA::ULong TAO_INVOCATION_SEND_REQUEST_MINOR_CODE     = (0x02U << 7);
const CORBA::ULong TAO_POA_DISCARDING                         = (0x03U << 7);
const CORBA::ULong TAO_POA_HOLDING                            = (0x04U << 7);
const CORBA::ULong TAO_UNHANDLED_SERVER_CXX_EXCEPTION         = (0x05U << 7);
const CORBA::ULong TAO_INVOCATION_RECV_REQUEST_MINOR_CODE     = (0x06U << 7);
const CORBA::ULong TAO_CONNECTOR_REGISTRY_NO_USABLE_PROTOCOL  = (0x07U << 7);
const CORBA::ULong TAO_MPROFILE_CREATION_ERROR                = (0x08U << 7);
const CORBA::ULong TAO_TIMEOUT_CONNECT_MINOR_CODE             = (0x09U << 7);
const CORBA::ULong TAO_TIMEOUT_SEND_MINOR_CODE                = (0x0AU << 7);
const CORBA::ULong TAO_TIMEOUT_RECV_MINOR_CODE                = (0x0BU << 7);
const CORBA::ULong TAO_IMPLREPO_MINOR_CODE                    = (0x0CU << 7);
const CORBA::ULong TAO_ACCEPTOR_REGISTRY_OPEN_LOCATION_CODE   = (0x0DU << 7);
const CORBA::ULong TAO_ORB_CORE_INIT_LOCATION_CODE            = (0x0EU << 7);
const CORBA::ULong TAO_POLICY_NARROW_CODE                     = (0x0FU << 7);
const CORBA::ULong TAO_GUARD_FAILURE                          = (0x10U << 7);
const CORBA::ULong TAO_POA_BEING_DESTROYED                    = (0x11U << 7);
const CORBA::ULong TAO_POA_INACTIVE                           = (0x12U << 7);
const CORBA::ULong TAO_CONNECTOR_REGISTRY_INIT_LOCATION_CODE  = (0x13U << 7);
const CORBA::ULong TAO_AMH_REPLY_LOCATION_CODE                = (0x14U << 7);
const CORBA::ULong TAO_RTCORBA_THREAD_CREATION_LOCATION_CODE  = (0x15U << 7);
// *Don't* use TAO_<location>_MINOR_CODE greater than 0x1FU!
//@}
//@}

#include /**/ "ace/post.h"

#endif  /* TAO_ORB_CONSTANTS_H */
