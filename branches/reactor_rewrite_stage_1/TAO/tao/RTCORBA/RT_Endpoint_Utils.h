// This may look like C, but it's really -*- C++ -*-

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
#include "ace/pre.h"

#include "tao/corbafwd.h"
#include "rtcorba_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

class TAO_GIOP_Invocation;

/**
 * @class RT_Endpoint_Utils
 *
 * @brief Commonly used utility methods used by the RT_Endpoint_Selectors.
 *
 */
class TAO_RTCORBA_Export TAO_RT_Endpoint_Utils
{
public:
  static CORBA::Policy *priority_bands_policy (TAO_GIOP_Invocation *invocation
                                               TAO_ENV_ARG_DECL);

  static CORBA::Policy *client_protocol_policy (TAO_GIOP_Invocation *invocation
                                                TAO_ENV_ARG_DECL);

private:
  /// ctor.
  TAO_RT_Endpoint_Utils ();
};

#include "ace/post.h"
#endif  /* RT_ENDPOINT_UTILS_H */
