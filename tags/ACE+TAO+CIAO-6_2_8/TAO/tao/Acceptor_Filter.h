// -*- C++ -*-

//=============================================================================
/**
 *  @file    Acceptor_Filter.h
 *
 *  $Id$
 *
 *  @author Marina Spivak (marina@cs.wustl.edu)
 *  @author Carlos O'Ryan (coryan@uci.edu)
 */
//=============================================================================

#ifndef TAO_ACCEPTOR_FILTER_H
#define TAO_ACCEPTOR_FILTER_H

#include /**/ "ace/pre.h"

#include "tao/orbconf.h"
#include /**/ "tao/TAO_Export.h"
#include "tao/Basic_Types.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace TAO
{
  class ObjectKey;
}

class TAO_MProfile;
class TAO_Acceptor;

/**
 * @class TAO_Acceptor_Filter
 *
 * @brief Interface for mprofile creation strategies.
 *
 * Concrete Acceptor_Filter encapsulates a strategy for populating
 * mprofile with profiles: it decides which endpoints are included
 * into mprofile and which ones are not.
 */
class TAO_Export TAO_Acceptor_Filter
{
public:
  virtual ~TAO_Acceptor_Filter (void);

  /**
   * Populate @a mprofile with profiles. Profiles are created for
   * selected acceptors (endpoints) from the array delimited by
   * @a acceptors_begin and @a acceptors_end.
   */
  virtual int fill_profile (const TAO::ObjectKey &object_key,
                            TAO_MProfile &mprofile,
                            TAO_Acceptor **acceptors_begin,
                            TAO_Acceptor **acceptors_end,
                            CORBA::Short priority = TAO_INVALID_PRIORITY) = 0;

  /// Encodes the endpoints in the profiles into the TAO_TAG_ENDPOINTS
  /// tag component of profiles.
  virtual int encode_endpoints (TAO_MProfile &mprofile) = 0;
};

TAO_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"
#endif /* TAO_ACCEPTOR_FILTER_H */
