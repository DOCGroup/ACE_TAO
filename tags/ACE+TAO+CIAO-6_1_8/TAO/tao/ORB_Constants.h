// -*- C++ -*-

//=============================================================================
/**
 *  @file  ORB_Constants.h
 *
 *  $Id$
 *
 *  Constants needed by various files.
 *
 *  @author  Jeff Parsons
 *  @author  Ossama Othman
 */
//=============================================================================

#ifndef TAO_ORB_CONSTANTS_H
#define TAO_ORB_CONSTANTS_H

#include /**/ "ace/pre.h"

#include "tao/Basic_Types.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

// A hash define for the regular two way operation.
#define TAO_TWOWAY_RESPONSE_FLAG 255

namespace CORBA
{
  /// The OMG Vendor Minor Codeset ID (VMCID).
  /**
   * This number is reserved by the OMG as a prefix to all the
   * standard system exception minor codes.  Check the CORBA/IIOP
   * specification for details.
   *
   * OMG defined system exception minor codes should be logically
   * OR-ed with this constant when passing such minor codes to a
   * @c CORBA::SystemException constructor.  For example, throwing a
   * @c CORBA::BAD_PARAM exception with an OMG defined minor code of
   * @c 2 is done as follows:
   *
   * @verbatim
   *   throw ::CORBA::BAD_PARAM (CORBA::OMGVMCID | 2, CORBA::COMPLETED_NO);
   * @endverbatim
   *
   * *NOT* as:
   *
   * @verbatim
   *   throw ::CORBA::BAD_PARAM (2, CORBA::COMPLETED_NO);
   * @endverbatim
   */
  const ULong OMGVMCID = 0x4f4d0000U;
}

namespace TAO
{
  /// TAO Vendor Minor Codeset ID (VMCID).
  /**
   * TAO Vendor Minor Codeset ID (VMCID) assigned by the OMG.  Do
   * *NOT* change at random.  The TAO VMCID is an ASCII representation
   * of @c TA0xxxx (close enough since a VMCID only consists of the
   * higher order 20 bits of a 32 bit unsigned long integer).  The
   * first 16 bits are @c TA, and the remaining 4 are @c 0.
   *
   * @note Remember that we can only play with the lower order 12
   *       bits.  @c MAX_MINOR_CODE is there to remind us of that.
   *
   * @see @c MAX_MINOR_CODE
   */
  const CORBA::ULong VMCID = 0x54410000U;

  /// Maximum allowed TAO system exception minor code.
  /**
   * Vendors are only allowed to use the lower order 12 bits of their
   * when defining vendor-specific exception minor codes.
   * @c MAX_MINOR_CODE is the maximum minor code value available to
   * TAO.  Do not exceed it when defining TAO system exception minor
   * codes.
   *
   * @see @c VMCID
   */
  const CORBA::ULong MAX_MINOR_CODE = VMCID | 0xfffU;

  /// TAO Vendor @c PolicyType Valueset ID (VPVID)
  /**
   * TAO Vendor @c PolicyType Valueset ID (VPVID) assigned by the
   * OMG.  This value was automatically assigned when TAO's VMCID was
   * assigned.  Its value is always the same as the VMCID.
   *
   * As with TAO-specific system exception minor codes, the
   * TAO-specific @c PolicyType space occupies the lower order 12 bits
   * of the TAO VPVID, i.e.
   *
   * @verbatim
   *    TAO::VPVID <= TAO PolicyType <= TAO::VPVID | 0xfffU.
   * @endverbatim
   *
   * For example, TAO-specific @c PolicyTypes should be defined by
   * logically "OR-ing" a 12 bit or less (i.e. 0x0U <= value <=
   * 0xfffU) value with @c TAO::VPVID.
   *
   * @see VMCID
   */
  const CORBA::ULong VPVID = VMCID;
}

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
 * The TAO @c IOP::ProfileId range @c 0x54414f00 - @c 0x54414f0f has
 * been reserved with the OMG.
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
const CORBA::ULong TAO_TAG_DIOP_PROFILE = 0x54414f04U;

/// COIOP
const CORBA::ULong TAO_TAG_COIOP_PROFILE = 0x54414f05U;

/// SCIOP
const CORBA::ULong TAO_TAG_SCIOP_PROFILE = 0x54414f0EU;

/// Tandem (HP) File System Protocol
const CORBA::ULong TAO_TAG_NSKFS_PROFILE = 0x00003039U;

/// Tandem (HP) Pathsend Protocol
const CORBA::ULong TAO_TAG_NSKPW_PROFILE = 0x0000303AU;

// Default DiffServ CodePoint.
const int IPDSFIELD_DSCP_DEFAULT = 0x00;

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

TAO_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"

#endif  /* TAO_ORB_CONSTANTS_H */
