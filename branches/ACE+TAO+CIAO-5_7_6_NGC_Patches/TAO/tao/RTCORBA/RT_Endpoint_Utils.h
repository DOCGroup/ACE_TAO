// -*- C++ -*-

//=============================================================================
/**
 *  @file   RT_Endpoint_Utils.h
 *
 *  $Id$
 *
 *  Commonly used utility methods used by RT_Endpoint_Selectors.
 *
 *  @author Angelo Corsaro <corsaro@cs.wustl.edu>
 *  @author Frank Hunleth <fhunleth@cs.wustl.edu>
 */
//=============================================================================


#ifndef RT_ENDPOINT_UTILS_H
#define RT_ENDPOINT_UTILS_H

#include /**/ "ace/pre.h"

#include "tao/RTCORBA/rtcorba_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/Policy_ForwardC.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace TAO
{
  class Profile_Transport_Resolver;
}

/**
 * @class RT_Endpoint_Utils
 *
 * @brief Commonly used utility methods used by the RT_Endpoint_Selectors.
 *
 */
class TAO_RTCORBA_Export TAO_RT_Endpoint_Utils
{
public:
  static
  CORBA::Policy_ptr policy (TAO_Cached_Policy_Type type,
                            TAO::Profile_Transport_Resolver &r);

private:
  /// ctor.
  TAO_RT_Endpoint_Utils ();
};

TAO_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"

#endif  /* RT_ENDPOINT_UTILS_H */
