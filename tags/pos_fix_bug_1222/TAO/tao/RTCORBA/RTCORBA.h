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
#include "ace/pre.h"

#include "rtcorba_export.h"
#include "tao/corbafwd.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

class TAO_RTCORBA_Export TAO_RTCORBA_Initializer
{
public:
  // Constructor.
  TAO_RTCORBA_Initializer (void);
};

static TAO_RTCORBA_Initializer TAO_RTCORBA_initializer;

#define TAO_RTCORBA_SAFE_INCLUDE
#include "RTCORBAC.h"
#undef TAO_RTCORBA_SAFE_INCLUDE

#include "ace/post.h"
#endif /* TAO_RTCORBA_H_ */
