// -*- C++ -*-

//=============================================================================
/**
 *  @file    RT_ORB_Loader.h
 *
 *  $Id$
 *
 *   Header file for Loading RT_ORB.
 *
 *
 *  @author  Priyanka Gontla <gontla_p@ociweb.com>
 */
//=============================================================================

#ifndef TAO_RT_ORB_LOADER_H
#define TAO_RT_ORB_LOADER_H
#include /**/ "ace/pre.h"

#include "tao/orbconf.h"

#if defined (TAO_HAS_CORBA_MESSAGING) && TAO_HAS_CORBA_MESSAGING != 0

#include "tao/RTCORBA/rtcorba_export.h"
#include "ace/Service_Object.h"
#include "ace/Service_Config.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

class TAO_ORB_Core;

class TAO_RTCORBA_Export TAO_RT_ORB_Loader : public ACE_Service_Object
{
public:

  /// Destructor.
  virtual ~TAO_RT_ORB_Loader (void);

  /// Initialize the RT ORB loader hooks.
  virtual int init (int argc, ACE_TCHAR* []);
};


ACE_STATIC_SVC_DECLARE_EXPORT (TAO_RTCORBA, TAO_RT_ORB_Loader)
ACE_FACTORY_DECLARE (TAO_RTCORBA, TAO_RT_ORB_Loader)

TAO_END_VERSIONED_NAMESPACE_DECL

#endif /* TAO_HAS_CORBA_MESSAGING && TAO_HAS_CORBA_MESSAGING != 0 */

#include /**/ "ace/post.h"
#endif /* TAO_RT_ORB_LOADER_H */
