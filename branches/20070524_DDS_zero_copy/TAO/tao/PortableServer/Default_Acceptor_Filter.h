// -*- C++ -*-

//=============================================================================
/**
 *  @file    Default_Acceptor_Filter.h
 *
 *  $Id$
 *
 *  Default strategy for populating mprofile
 *
 *  @author  Marina Spivak <marina@cs.wustl.edu>
 *  @author  Carlos O'Ryan <coryan@uci.edu>
 */
//=============================================================================


#ifndef TAO_DEFAULT_ACCEPTOR_FILTER_H
#define TAO_DEFAULT_ACCEPTOR_FILTER_H

#include /**/ "ace/pre.h"

#include "tao/PortableServer/portableserver_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/Acceptor_Filter.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

class TAO_Acceptor;

/**
 * @class TAO_Default_Acceptor_Filter
 *
 * @brief Default Acceptor_Filter.
 *
 * Default strategy for populating mprofile: all available
 * endpoints are included.
 */
class TAO_PortableServer_Export TAO_Default_Acceptor_Filter
  : public TAO_Acceptor_Filter
{
public:
  TAO_Default_Acceptor_Filter (void);

  /// Populate @a mprofile with all available endpoints.
  virtual int fill_profile (const TAO::ObjectKey &object_key,
                    TAO_MProfile &mprofile,
                    TAO_Acceptor **acceptors_begin,
                    TAO_Acceptor **acceptors_end,
                    CORBA::Short priority = TAO_INVALID_PRIORITY);

  /// Encodes the endpoints in the profiles into the TAO_TAG_ENDPOINTS
  /// tag component of profiles.
  int encode_endpoints (TAO_MProfile &mprofile);
};

TAO_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"

#endif /* TAO_DEFAULT_ACCEPTOR_FILTER_H */
