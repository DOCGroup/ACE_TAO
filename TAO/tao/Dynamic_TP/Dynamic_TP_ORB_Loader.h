// -*- C++ -*-

//=============================================================================
/**
 *  @file    Dynamic_TP_ORB_Loader.h
 *
 *  $Id$
 *
 *   Header file for Loading DynamicTP.
 *
 *
 *  @author  Phil Mesnier <mesnier_p@ociweb.com>
 */
//=============================================================================

#ifndef TAO_DYNAMICTP_ORB_LOADER_H
#define TAO_DYNAMICTP_ORB_LOADER_H
#include /**/ "ace/pre.h"

#include "tao/orbconf.h"

#include "tao/Dynamic_TP/dynamic_tp_export.h"
#include "ace/Service_Object.h"
#include "ace/Service_Config.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

class TAO_ORB_Core;

class TAO_Dynamic_TP_Export TAO_Dynamic_TP_ORB_Loader : public ACE_Service_Object
{
public:

  /// Constructor.
  TAO_Dynamic_TP_ORB_Loader (void);

  /// Destructor.
  virtual ~TAO_Dynamic_TP_ORB_Loader (void);

  /// Initialize the DynamicTP loader hooks.
  virtual int init (int argc, ACE_TCHAR* []);

private:
  /// Set to true after init is called.
  bool initialized_;
};


ACE_STATIC_SVC_DECLARE_EXPORT (TAO_Dynamic_TP_ORB, TAO_Dynamic_TP_ORB_Loader)
ACE_FACTORY_DECLARE (TAO_Dynamic_TP_ORB, TAO_Dynamic_TP_ORB_Loader)

TAO_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"
#endif /* TAO_DYNAMIC_TP_LOADER_H */
