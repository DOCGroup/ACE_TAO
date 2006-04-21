// -*- C++ -*-

//=============================================================================
/**
 *  @file    RT_Acceptor_Filters.h
 *
 *  $Id$
 *
 *  RTCORBA strategies for populating mprofile.
 *
 *  @author  Marina Spivak <marina@cs.wustl.edu>
 */
// ===================================================================

#ifndef TAO_RT_ACCEPTOR_FILTER_H
#define TAO_RT_ACCEPTOR_FILTER_H

#include /**/ "ace/pre.h"

#include "tao/orbconf.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#if defined (TAO_HAS_CORBA_MESSAGING) && TAO_HAS_CORBA_MESSAGING != 0

#include "tao/RTPortableServer/rtportableserver_export.h"
#include "tao/Acceptor_Filter.h"

#define TAO_RTCORBA_SAFE_INCLUDE
#include "tao/RTCORBA/RTCORBAC.h"
#undef TAO_RTCORBA_SAFE_INCLUDE

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

class TAO_Acceptor;

/**
 * @class TAO_Server_Protocol_Acceptor_Filter
 *
 * @brief Populates mprofile with endpoints selected based on the
 * RTCORBA::ServerProtocolPolicy.
 */
class TAO_RTPortableServer_Export TAO_Server_Protocol_Acceptor_Filter :
  public TAO_Acceptor_Filter
{
public:
  /// Constructor
  TAO_Server_Protocol_Acceptor_Filter (RTCORBA::ProtocolList &protocols);

  /// Populate @a mprofile based on what's in <protocols_>.
  int fill_profile (const TAO::ObjectKey &object_key,
                    TAO_MProfile &mprofile,
                    TAO_Acceptor **acceptors_begin,
                    TAO_Acceptor **acceptors_end,
                    CORBA::Short priority);

  /// Encodes the endpoints in the profiles into the TAO_TAG_ENDPOINTS
  /// tag component of profiles.
  int encode_endpoints (TAO_MProfile &mprofile);

private:
  /// Value of the ServerProtocolPolicy used for endpoint
  /// selection.
  RTCORBA::ProtocolList &protocols_;
};

TAO_END_VERSIONED_NAMESPACE_DECL

#endif /* TAO_HAS_CORBA_MESSAGING && TAO_HAS_CORBA_MESSAGING != 0 */

#include /**/ "ace/post.h"

#endif /* TAO_RT_ACCEPTOR_FILTER_H */
