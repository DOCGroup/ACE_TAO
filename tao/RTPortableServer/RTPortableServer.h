// -*- C++ -*-

//=============================================================================
/**
 *  @file    RTPortableServer.h
 *
 *  $Id$
 *
 *  @author  Frank Hunleth  <fhuntleth@cs.wustl.edu>
 */
//=============================================================================

#ifndef TAO_RT_PORTABLESERVER_H
#define TAO_RT_PORTABLESERVER_H

#include /**/ "ace/pre.h"

#include "tao/RTPortableServer/rtportableserver_export.h"
#include "tao/orbconf.h"

#if defined (TAO_HAS_CORBA_MESSAGING) && TAO_HAS_CORBA_MESSAGING != 0

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/RTCORBA/RTCORBA.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

class TAO_RTPortableServer_Export TAO_RTPortableServer_Initializer
{
public:
  /// Constructor.
  TAO_RTPortableServer_Initializer (void);
};

static TAO_RTPortableServer_Initializer TAO_RTPortableServer_initializer;

TAO_END_VERSIONED_NAMESPACE_DECL

#endif /* TAO_HAS_CORBA_MESSAGING && TAO_HAS_CORBA_MESSAGING != 0 */

#define TAO_RT_PORTABLESERVER_SAFE_INCLUDE
#include "tao/RTPortableServer/RTPortableServerC.h"
#undef TAO_RT_PORTABLESERVER_SAFE_INCLUDE

#include /**/ "ace/post.h"

#endif /* TAO_RT_PORTABLESERVER_H */
