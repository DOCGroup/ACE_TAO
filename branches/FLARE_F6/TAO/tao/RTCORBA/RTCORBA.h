// -*- C++ -*-

// $Id$

//=============================================================================
/**
 *  @file RTCORBA.h
 *
 *  $Id$
 *
 *   Takes care of the RT ORB loading on static build.
 *
 *
 *  @author  Angelo Corsaro <corsaro@cs.wustl.edu>
 *  @author  Frank Hunleth <fhunleth@cs.wustl.edu>
 */
//=============================================================================

#ifndef TAO_RTCORBA_H_
#define TAO_RTCORBA_H_

#include /**/ "ace/pre.h"

#include "tao/RTCORBA/rtcorba_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/orbconf.h"

#if TAO_HAS_CORBA_MESSAGING == 1

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

class TAO_RTCORBA_Export TAO_RTCORBA_Initializer
{
public:
  // Constructor.
  TAO_RTCORBA_Initializer (void);
};

static TAO_RTCORBA_Initializer TAO_RTCORBA_initializer;

TAO_END_VERSIONED_NAMESPACE_DECL

#endif /* TAO_HAS_CORBA_MESSAGING != 0 */

#define TAO_RTCORBA_SAFE_INCLUDE
#include "tao/RTCORBA/RTCORBAC.h"
#include "tao/RTCORBA/RT_ProtocolPropertiesC.h"
#undef TAO_RTCORBA_SAFE_INCLUDE

#include "tao/RTCORBA/rtcorba_typedefs.h"

#include /**/ "ace/post.h"

#endif /* TAO_RTCORBA_H_ */
